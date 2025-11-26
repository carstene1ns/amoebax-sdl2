//
// Cross-platform free Puyo-Puyo clone.
// Copyright (C) 2006, 2007 Emma's Software
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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#if !defined (AMOEBAX_AI_PLAYER_H)
#define AMOEBAX_AI_PLAYER_H

#include "IPlayer.h"
#include "Grid.h"
#include "GridStatus.h"

namespace Amoebax
{
    ///
    /// \class AIPlayer
    /// \brief Base class for all computer controlled players.
    ///
    class AIPlayer: public IPlayer
    {
        public:
            explicit AIPlayer (IPlayer::PlayerSide side,
                               uint32_t timeToWaitForNextMove,
                               uint32_t timeDeviation);

            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value);
            virtual void joyDown (uint8_t joystick, uint8_t button);
            virtual void joyUp (uint8_t joystick, uint8_t button);
#if !defined (IS_GP2X_HOST)
            virtual void keyDown (uint32_t key);
            virtual void keyUp (uint32_t key);
#endif // !IS_GP2X_HOST
            virtual void loadOptions (void);
            virtual void update (uint32_t elapsedTime);

        protected:
            ///
            /// \enum Checking
            /// \brief Tells which pair are we checking for scores.
            ///
            enum Checking
            {
                /// We are checking the current falling pair.
                CheckingCurrentFallingPair,
                /// We are checking the next falling pair.
                CheckingNextFallingPair,
                /// We are checking the following falling pair.
                CheckingFollowingFallingPair
            };

            Checking getWhichPairToCheck (void) const;

            ///
            /// \brief Computes the score of a move.
            ///
            /// \param result The position result of a move.
            /// \return The score of that position result.
            ///
            virtual int32_t computeScore (const GridStatus::PositionResult &result) const = 0;

            ///
            /// \brief Tells if the following falling pair should be checked.
            ///
            /// \return \a true if the following falling pair should be checked.
            ///
            virtual bool shouldCheckFollowingFallingPair (void) const = 0;

            ///
            /// \brief Tells if the next falling pair should be checked.
            ///
            /// \return \a true if the next falling pair should be checked.
            ///
            virtual bool shouldCheckNextFallingPair (void) const = 0;

            ///
            /// \brief Tells if the falling pair should fall at max speed.
            ///
            /// \return \a true if the falling pair should fall at max speed
            ///         when it's on the final position.
            ///
            virtual bool shouldThePairFall (void) const = 0;

            ///
            /// \brief Tells if the satellite should be visible before moving.
            ///
            /// \return \a true if the satellite amoeba should be visible
            ///         before the player tries to move the falling pair.
            ///
            virtual bool shouldSatelliteBeVisible (void) const = 0;

        private:
            ///
            /// \enum Rotation
            /// \brief The rotation of the falling pair.
            ///
            enum Rotation
            {
                /// The satellite is at bottom of main.
                RotationBottom,
                /// The satellite is at left of main, same Y as main.
                RotationLeft,
                /// The satellite is at right of main, same Y as main.
                RotationRight,
                /// The satellite is at top of main.
                RotationTop
            };

            ///
            /// \struct Move
            /// \brief A representation of a possible falling pair position.
            ///
            struct Move
            {
                /// The position of the main falling amoeba.
                Grid::FallingAmoeba main;
                /// The position of the satellite falling amoeba.
                Grid::FallingAmoeba satellite;
                /// The rotation.
                Rotation rotation;
            };

            ///
            /// \struct State
            /// \brief The current movement and grid's state of a pair.
            ///
            struct State
            {
                /// The current movement.
                Move move;
                /// The score of the current position.
                int32_t score;
                /// The current grid's state.
                GridStatus gridState;
                /// The current position of X to check the positions to.
                int16_t currentX;
                /// The end position of X to check the positions to.
                int16_t endX;

                bool isAtLastPosition (void) const;
            };

            bool canMove (void) const;
            void checkAllPositionsOf (State &state, const GridStatus &gridState,
                                      int32_t parentScore);
            void checkCurrentFallingPair (void);
            void checkFollowingFallingPair (void);
            void checkIfPairIsAvailable (void);
            void checkNextFallingPair (void);
            void checkNextPositionOf (State &state, const GridStatus &gridState,
                                      int32_t parentScore = 0);
            void computeNextMove (void);
            Move getBestMove (void) const;
            int32_t getBestScore (void) const;
            bool hasFinalMove (void) const;
            bool hasPairAtFinalPosition (void) const;
            void initializeState (State &state, const GridStatus &gridState,
                                  int32_t parentScore = 0) const;
            bool isWaitingNextPair (void) const;
            void moveLeft (void);
            void movePairToPosition (void);
            void moveRight (void);
            void pairIsAtPosition (void);
            void rotateClockwise (void);
            void rotateCounterClockwise (void);
            void setBestMove (const Move &bestMove, int32_t bestScore);
            void setHasFinalMove (void);
            void setPairToCheck (Checking pairToCheck);
            void setToWaitingNextPair (void);
            void updateTimeOfNextMove (void);

            /// The best movement the player has found.
            Move m_BestMove;
            /// The score of the best movement.
            int32_t m_BestScore;
            /// The falling pair is at the computed position.
            bool m_FallingPairAtPosition;
            /// The player has chosen its final movement.
            bool m_HaveFinalMove;
            /// The current state of the three falling pairs.
            State m_PairState[3];
            /// Tells which pair are we checking right now for its best move.
            Checking m_PairToCheck;
            /// The max. number of milliseconds to add/subtract to the average.
            uint32_t m_TimeDeviation;
            /// The time we can make the next movement.
            uint32_t m_TimeOfNextMove;
            /// The average time the player waits between moves, in ms.
            uint32_t m_TimeToWaitForNextMove;
            /// Tells if we are waiting for the next grid's falling pair.
            bool m_WaitingNextPair;
    };

    ///
    /// \brief Tells which pair to check for its best move.
    ///
    /// \return The pair to check for its best move.
    ///
    inline AIPlayer::Checking
    AIPlayer::getWhichPairToCheck (void) const
    {
        return m_PairToCheck;
    }
}

#endif // AMOEBAX_AI_PLAYER_H
