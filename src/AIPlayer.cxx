//
// Cross-platform free Puyo-Puyo clone.
// Copyright (C) 2007 Emma's software
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
#include <cassert>
#include <limits>
#include "AIPlayer.h"
#include "FrameManager.h"

using namespace Amoebax;

///
/// \brief Default constructor.
///
/// \param side The player's grid side.
/// \param timeToWaitForNextMove The average time the player
///                              waits between moves, in ms.
/// \param timeDeviation The max. number of milliseconds to add or
///                      substracts to the average time to move
///                      (minimum is 1ms.)
///
AIPlayer::AIPlayer (IPlayer::PlayerSide side,
                    uint32_t timeToWaitForNextMove,
                    uint32_t timeDeviation):
    IPlayer (side),
    m_BestMove (),
    m_BestScore (std::numeric_limits<int32_t>::min ()),
    m_FallingPairAtPosition (false),
    m_HaveFinalMove (false),
    m_PairToCheck (CheckingCurrentFallingPair),
    m_TimeDeviation (timeDeviation),
    m_TimeOfNextMove (0),
    m_TimeToWaitForNextMove (timeToWaitForNextMove),
    m_WaitingNextPair (true)
{
    assert ( 0 <= m_TimeDeviation &&
            "The time deviation must be greater than 0!" );
}

///
/// \brief Tells if the AI player can move.
///
/// Checks if the current time is past the time of the next move,
/// letting the player move.
///
/// \return \a true if the player can make the next move,
///         \a false otherwise.
///
bool
AIPlayer::canMove (void) const
{
    return FrameManager::getCurrentTime () > m_TimeOfNextMove;
}

///
/// \brief Checks all possible possitions of an state.
///
/// \param state The state to check all its positions.
/// \param gridState The current grid state to make the check with.
/// \param parentScore The score of the parent's state.
///
void
AIPlayer::checkAllPositionsOf (State &state, const GridStatus &gridState,
                               int32_t parentScore)
{
    bool initialPosition = true;
    state.move.rotation = RotationLeft;
    do
    {
        if ( initialPosition )
        {
            initialPosition = false;
        }
        else
        {
            // Next rotation.
            switch ( state.move.rotation )
            {
                case RotationLeft:
                    state.move.rotation = RotationTop;
                    break;

                case RotationTop:
                    state.move.rotation = RotationRight;
                    break;

                case RotationRight:
                default:
                    state.move.rotation = RotationBottom;
                    break;
            }
        }

        switch ( state.move.rotation )
        {
            case RotationBottom:
                state.move.main.x = 0;
                state.move.satellite.x = 0;
                state.endX = Grid::k_GridWidth;
                // swap the satellite and main amoebas, so they will
                // be exactly the same case as RotationTop, but in reverse
                // order.
                std::swap (state.move.main, state.move.satellite);
                break;

            case RotationLeft:
                state.move.main.x = 1;
                state.move.satellite.x = 0;
                state.endX = Grid::k_GridWidth - 1;
                break;

            case RotationRight:
                state.move.main.x = 0;
                state.move.satellite.x = 1;
                state.endX = Grid::k_GridWidth - 1;
                break;

            case RotationTop:
                state.move.main.x = 0;
                state.move.satellite.x = 0;
                state.endX = Grid::k_GridWidth;
                break;

            default:
                state.move.rotation = RotationLeft;
                state.move.main.x = 1;
                state.move.satellite.x = 0;
                state.endX = Grid::k_GridWidth - 1;
                break;
        }
        // Check all positions.
        for ( state.currentX = 0 ; state.currentX < state.endX ; ++state.currentX )
        {
            state.gridState = gridState;
            GridStatus::PositionResult result;
            state.gridState.checkPositions (state.move.main,
                                            state.move.satellite, result);
            state.score = parentScore + computeScore (result);
            if ( state.score > getBestScore () )
            {
                // Take into account that satellite and main are
                // reversed when the satellite is at bottom.
                if ( RotationBottom == m_BestMove.rotation )
                {
                    std::swap (m_PairState[0].move.main,
                               m_PairState[0].move.satellite);
                }
                setBestMove (m_PairState[0].move, state.score);
                if ( RotationBottom == m_BestMove.rotation )
                {
                    std::swap (m_PairState[0].move.main,
                               m_PairState[0].move.satellite);
                }
            }
            ++state.move.main.x;
            ++state.move.satellite.x;
        }
    }
    while ( RotationBottom != state.move.rotation );
}

///
/// \brief Checks the next position of the current falling pair.
///
void
AIPlayer::checkCurrentFallingPair (void)
{
    if ( shouldCheckNextFallingPair () )
    {
        if ( m_PairState[0].isAtLastPosition () )
        {
            setHasFinalMove ();
        }
        else
        {
            checkNextPositionOf (m_PairState[0], getGrid ()->getState ());
            initializeState (m_PairState[1], m_PairState[0].gridState,
                             m_PairState[0].score);
            setPairToCheck (CheckingNextFallingPair);
        }
    }
    else
    {
        checkAllPositionsOf (m_PairState[0], getGrid ()->getState (), 0);
        setHasFinalMove ();
    }
}

///
/// \brief Checks all the position of the following (i.e., last) pair.
///
void
AIPlayer::checkFollowingFallingPair (void)
{
    checkAllPositionsOf (m_PairState[2], m_PairState[1].gridState,
                         m_PairState[1].score);
    setPairToCheck (CheckingNextFallingPair);
}



///
/// \brief Checks if a pair is starting to fall into the grid.
///
/// If the pair has started to fall into the grid then initializes
/// the first movement and computes its best score. Then sets the
/// AI player in a non-waiting state.
///
void
AIPlayer::checkIfPairIsAvailable (void)
{
    if ( getGrid ()->hasNewFallingPair () )
    {
        // Get the current position of the falling pair,
        // which should be the center of the grid. This also gets
        m_PairState[0].move.main = getGrid ()->getFallingMainAmoeba ();
        m_PairState[0].move.satellite = getGrid ()->getFallingSatelliteAmoeba ();
        // Also get the next one or two pairs.
        if ( shouldCheckNextFallingPair () )
        {
            m_PairState[1].move.main = getGrid ()->getNextFallingMainAmoeba ();
            m_PairState[1].move.satellite = getGrid ()->getNextFallingSatelliteAmoeba ();
            if ( shouldCheckFollowingFallingPair () )
            {
                m_PairState[2].move.main = getGrid ()->getFollowingFallingMainAmoeba ();
                m_PairState[2].move.satellite = getGrid ()->getFollowingFallingSatelliteAmoeba ();
            }
        }
        // The best movement is also the initial position, as if we
        // don't have a better movement, why waste time moving the falling
        m_PairState[0].move.rotation = RotationTop;
        setBestMove (m_PairState[0].move, std::numeric_limits<int32_t>::min ());
        initializeState (m_PairState[0], getGrid ()->getState ());
        setPairToCheck (CheckingCurrentFallingPair);
        m_FallingPairAtPosition = false;
        m_HaveFinalMove = false;
        m_WaitingNextPair = false;
    }
}

///
/// \brief Checks the next position or all position of the next pair.
///
void
AIPlayer::checkNextFallingPair (void)
{
    if ( shouldCheckFollowingFallingPair () )
    {
        if ( m_PairState[1].isAtLastPosition () )
        {
            setPairToCheck (CheckingCurrentFallingPair);
        }
        else
        {
            checkNextPositionOf (m_PairState[1], m_PairState[0].gridState,
                                 m_PairState[0].score);
            setPairToCheck (CheckingFollowingFallingPair);
            checkFollowingFallingPair ();
            checkNextFallingPair ();
        }
    }
    else
    {
        checkAllPositionsOf (m_PairState[1], m_PairState[0].gridState,
                             m_PairState[0].score);
        setPairToCheck (CheckingCurrentFallingPair);
    }
}

///
/// \brief Changes the position of the state and computes its score.
///
/// \param state The state to change and to check its score.
/// \param gridState The current state of the grid to check the
///                  position to.
/// \param parentScore The score of the parent state.
///
void
AIPlayer::checkNextPositionOf (State &state, const GridStatus &gridState,
                               int32_t parentScore)
{
    // If we are not at the end of the current rotation.
    if ( state.currentX < state.endX )
    {
        ++state.currentX;
        ++state.move.main.x;
        ++state.move.satellite.x;
    }
    // Otherwise go to the next rotation and start again.
    else
    {
        state.currentX = 1;
        switch ( state.move.rotation )
        {
            case RotationLeft:
                state.move.rotation = RotationTop;
                state.move.main.x = 0;
                state.move.satellite.x = 0;
                state.endX = Grid::k_GridWidth;
                break;

            case RotationTop:
                state.move.rotation = RotationRight;
                state.move.main.x = 0;
                state.move.satellite.x = 1;
                state.endX = Grid::k_GridWidth - 1;
                break;

            case RotationRight:
            default:
                state.move.rotation = RotationBottom;
                state.move.main.x = 0;
                state.move.satellite.x = 0;
                state.endX = Grid::k_GridWidth;
                // swap the satellite and main amoebas, so they will
                // be exactly the same case as RotationTop, but in reverse
                // order.
                std::swap (state.move.main, state.move.satellite);
                break;
        }
    }
    state.gridState = gridState;
    GridStatus::PositionResult result;
    state.gridState.checkPositions (state.move.main, state.move.satellite,
                                    result);
    state.score = parentScore + computeScore (result);
}

///
/// \brief Computes the score of the next move.
///
void
AIPlayer::computeNextMove (void)
{
    switch ( getWhichPairToCheck () )
    {
        case CheckingCurrentFallingPair:
            checkCurrentFallingPair ();
            break;

        case CheckingNextFallingPair:
            checkNextFallingPair ();
            break;

        case CheckingFollowingFallingPair:
            checkFollowingFallingPair ();
            break;

        default:
            setPairToCheck (CheckingCurrentFallingPair);
            break;
    }
}

///
/// \brief Gets the found best move.
///
/// \return The best move found.
///
inline AIPlayer::Move
AIPlayer::getBestMove (void) const
{
    return m_BestMove;
}

///
/// \brief Gets the score of the best move.
///
/// \return The score of the best move.
///
inline int32_t
AIPlayer::getBestScore (void) const
{
    return m_BestScore;
}

///
/// \brief Tells if the player has the final move computed.
///
/// \return \a true if the player has the move computed, \a false otherwise.
///
inline bool
AIPlayer::hasFinalMove (void) const
{
    return m_HaveFinalMove;
}

///
/// \brief Tells if the pair is already at the final position.
///
/// \return \a true if the pair is at its final position, \a false otherwise.
///
inline bool
AIPlayer::hasPairAtFinalPosition (void) const
{
    return m_FallingPairAtPosition;
}

///
/// \brief Initializes an state to a left rotation.
///
/// \param state The state to initialize.
/// \param gridState The initial grid state to use to compute the score.
/// \param parentScore The score of the parent state.
///
void
AIPlayer::initializeState (State &state, const GridStatus &gridState,
                           int32_t parentScore) const
{
    state.move.rotation = RotationLeft;
    state.move.main.x = 0;
    state.move.satellite.x = -1;
    state.currentX = 0;
    state.endX = Grid::k_GridWidth - 1;

    state.gridState = gridState;
    GridStatus::PositionResult result;
    state.gridState.checkPositions (state.move.main, state.move.satellite,
                                    result);
    state.score = parentScore + computeScore (result);
}

///
/// \brief Tells if the player is still for a pair to fall into the grid.
///
/// \return \a true if the player is waiting, \a false otherwise.
///
inline bool
AIPlayer::isWaitingNextPair (void) const
{
    return m_WaitingNextPair;
}

void
AIPlayer::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
    // Computer players don't have joystick. Nothing to do.
}

void
AIPlayer::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    // Computer players don't have joystick. Nothing to do.
}

void
AIPlayer::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
    // Computer players don't have joystick. Nothing to do.
}

void
AIPlayer::keyDown (uint32_t key)
{
    // Computer players don't have keyboard. Nothing to do.
}

void
AIPlayer::keyUp (uint32_t key)
{
    // Computer players don't have keyboard. Nothing to do.
}

void
AIPlayer::loadOptions (void)
{
    // Nothing to do.
}

///
/// \brief Moves Grid's falling pair to the left.
///
void
AIPlayer::moveLeft (void)
{
    if ( canMove () )
    {
        getGrid ()->moveLeft ();
        updateTimeOfNextMove ();
    }
}

///
/// \brief Moves the falling pair an step closer to its final position.
///
void
AIPlayer::movePairToPosition (void)
{
    Move currentPosition;
    currentPosition.main = getGrid ()->getFallingMainAmoeba ();
    currentPosition.satellite = getGrid ()->getFallingSatelliteAmoeba ();

    if ( shouldSatelliteBeVisible () && currentPosition.satellite.y >= Grid::k_FirstVisibleHeight ||
         !shouldSatelliteBeVisible () && currentPosition.main.y >= Grid::k_FirstVisibleHeight )
    {
        if ( RotationLeft == getBestMove ().rotation &&
             currentPosition.main.x <= currentPosition.satellite.x )
        {
            rotateCounterClockwise ();
        }
        else if ( RotationRight == getBestMove ().rotation &&
                  currentPosition.main.x >= currentPosition.satellite.x )
        {
            rotateClockwise ();
        }
        else if ( RotationTop == getBestMove ().rotation &&
                  currentPosition.main.y <= currentPosition.satellite.y ||
                  RotationBottom == getBestMove ().rotation &&
                  currentPosition.main.y >= currentPosition.satellite.y )
        {
            rotateCounterClockwise ();
        }
        else
        {
            if ( getBestMove ().main.x == currentPosition.main.x &&
                 getBestMove ().satellite.x == currentPosition.satellite.x )
            {
                pairIsAtPosition ();
            }
            else if ( getBestMove ().main.x < currentPosition.main.x )
            {
                moveLeft ();
            }
            else
            {
                moveRight ();
            }
        }
    }
}

///
/// \brief Moves Grid's falling pair to the right.
///
void
AIPlayer::moveRight (void)
{
    if ( canMove () )
    {
        getGrid ()->moveRight ();
        updateTimeOfNextMove ();
    }
}

///
/// \brief Tells the player that the pair is already at its position.
///
inline void
AIPlayer::pairIsAtPosition (void)
{
    m_FallingPairAtPosition = true;
}

///
/// \brief Rotates Grid's falling pair clockwise.
///
void
AIPlayer::rotateClockwise (void)
{
    if ( canMove () )
    {
        getGrid ()->rotateClockwise ();
        updateTimeOfNextMove ();
    }
}

///
/// \brief Rotates Grid's falling pair counter clockwise.
///
void
AIPlayer::rotateCounterClockwise (void)
{
    if ( canMove () )
    {
        getGrid ()->rotateCounterClockwise ();
        updateTimeOfNextMove ();
    }
}

///
/// \brief Sets the best move and its score.
///
/// \param bestMove The best move to set.
/// \param bestScore The score of \p bestMove.
///
inline void
AIPlayer::setBestMove (const Move &bestMove, int32_t bestScore)
{
    m_BestMove = bestMove;
    m_BestScore = bestScore;
}

///
/// \brief Tells to the player that it has the final move.
///
inline void
AIPlayer::setHasFinalMove (void)
{
    m_HaveFinalMove = true;
}

///
/// \brief Sets the pair to check its moves.
///
/// \param pairToCheck The pair to check its moves.
///
inline void
AIPlayer::setPairToCheck (Checking pairToCheck)
{
    m_PairToCheck = pairToCheck;
}

///
/// \brief Sets the player into waiting the next falling pair mode.
///
inline void
AIPlayer::setToWaitingNextPair (void)
{
    m_WaitingNextPair = true;
}

void
AIPlayer::update (uint32_t elapsedTime)
{
    IPlayer::update (elapsedTime);
    if ( isWaitingNextPair () )
    {
        checkIfPairIsAvailable ();
    }
    else if ( !hasFinalMove () )
    {
        computeNextMove ();
    }
    else if ( !hasPairAtFinalPosition () && canMove () )
    {
        movePairToPosition ();
    }
    else if ( hasPairAtFinalPosition () && canMove () )
    {
        if ( shouldThePairFall () )
        {
            getGrid ()->setMaxFallingSpeed ();
        }
        setToWaitingNextPair ();
    }
}

///
/// \brief Updates the time of the next move.
///
void
AIPlayer::updateTimeOfNextMove (void)
{
    m_TimeOfNextMove = FrameManager::getCurrentTime () +
                       m_TimeToWaitForNextMove -
                       ((rand () % (m_TimeDeviation << 1)) -
                       m_TimeDeviation);
}

///
/// \brief Tells if the state is at the last position.
///
/// \return \a true if the state is at the last position, \a false otherwise.
///
bool
AIPlayer::State::isAtLastPosition (void) const
{
    return RotationBottom == move.rotation && currentX >= endX;
}
