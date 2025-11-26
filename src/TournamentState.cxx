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
#include <cassert>
#include <sstream>
#include "AIPlayerFactory.h"
#include "CongratulationsState.h"
#include "File.h"
#include "HumanPlayer.h"
#include "System.h"
#include "TournamentState.h"
#include "TwoComputerPlayersState.h"
#include "TwoPlayersState.h"
#include "VersusState.h"

using namespace Amoebax;

static const int32_t k_BlinkTime = 400;
static const uint8_t k_InvalidCharacterIndex = 255;
static const uint8_t k_FaceSize = 128;
static const uint8_t k_FacesPerRow = 7;
static const uint16_t k_InitialHorizontalPosition = 1280 / 2 - k_FaceSize / 2;
static const int32_t k_MatchStartTime = k_BlinkTime * 8;

///
/// \brief Constructor.
///
/// \param players The list of players going to compete in the tournament.
///
TournamentState::TournamentState (const std::vector<TournamentState::Player> &players):
    IState (),
    m_CurrentMatch (0),
    m_Background (nullptr),
    m_BackgroundMusic (nullptr),
    m_CurrentMatchBlinkTime (k_BlinkTime),
    m_CurrentMatchStartTime (k_MatchStartTime),
    m_Faces (nullptr),
    m_HorizontalOffset (),
    m_Matches (),
    m_NumPlayers (players.size ()),
    m_ShowCurrentOpponents (true),
    m_SoundLose (Sound::fromFile (File::getSoundFilePath ("youlose.wav"))),
    m_SoundWin (Sound::fromFile (File::getSoundFilePath ("youwin.wav"))),
    m_VerticalPosition ()
{
    loadGraphicResources ();
    createMatchTree (players);
}

///
/// \brief Destructor.
///
TournamentState::~TournamentState (void)
{
    deleteMatch (m_Matches.children.first);
    deleteMatch (m_Matches.children.second);
}

void
TournamentState::activate (void)
{
    if ( Music::isPaused () )
    {
        Music::resume ();
    }
    else if ( !Music::isPlaying () )
    {
        if ( 0 == m_BackgroundMusic.get () )
        {
            m_BackgroundMusic.reset (Music::fromFile
                    (File::getMusicFilePath ("menu.ogg")));
        }
        m_BackgroundMusic->play ();
    }
}

///
/// \brief Creates the tree of matches.
///
/// Based on the number of players, this function creates a tree
/// of matches and initializes it.  It also initializes the vertical
/// positions and horizontal offsets of each level of the tree.
///
/// \param players The vector of players that will play at the tournament.
///
void
TournamentState::createMatchTree (const std::vector<Player> &players)
{
    assert ( (2 == players.size () || 4 == players.size ()) &&
             "Currently only tournaments of 2 or 4 players are supported." );

    // This is a sentinel player used to represent unused entries.
    Player sentinelPlayer;
    sentinelPlayer.characterIndex = k_InvalidCharacterIndex;

    // The first "match" is the winner.
    m_Matches.parent = 0;
    m_Matches.opponents.first = sentinelPlayer;
    m_Matches.opponents.second = sentinelPlayer;
    m_Matches.children.second = 0;

    Match *semifinal = new Match;
    semifinal->parent = &m_Matches;
    if ( 2 == players.size () )
    {
        m_HorizontalOffset.push_back (0);
        m_HorizontalOffset.push_back (192);
        m_VerticalPosition.push_back (288);
        m_VerticalPosition.push_back (544);

        semifinal->opponents.first = players[0];
        semifinal->opponents.first.score = 0;
        semifinal->opponents.second = players[1];
        semifinal->opponents.second.score = 0;
        semifinal->children.first = 0;
        semifinal->children.second = 0;
        m_Matches.children.first = semifinal;

        m_CurrentMatch = semifinal;
    }
    else
    {
        m_HorizontalOffset.push_back (0);
        m_HorizontalOffset.push_back (321);
        m_HorizontalOffset.push_back (192);
        m_VerticalPosition.push_back (160);
        m_VerticalPosition.push_back (416);
        m_VerticalPosition.push_back (672);

        semifinal->opponents.first = sentinelPlayer;
        semifinal->opponents.second = sentinelPlayer;
        m_Matches.children.first = semifinal;

        Match *leftMatch = new Match;
        leftMatch->children.first = 0;
        leftMatch->children.second = 0;
        leftMatch->parent = semifinal;
        leftMatch->opponents.first = players[0];
        leftMatch->opponents.first.score = 0;
        leftMatch->opponents.second = players[1];
        leftMatch->opponents.second.score = 0;

        Match *rightMatch = new Match;
        rightMatch->children.first = 0;
        rightMatch->children.second = 0;
        rightMatch->parent = semifinal;
        rightMatch->opponents.first = players[2];
        rightMatch->opponents.first.score = 0;
        rightMatch->opponents.second = players[3];
        rightMatch->opponents.second.score = 0;

        semifinal->children.first = leftMatch;
        semifinal->children.second = rightMatch;

        m_CurrentMatch = leftMatch;
    }
}

///
/// \brief Recursively deletes a match and all its children.
///
/// \param match The match to delete, and all its children.
///
void
TournamentState::deleteMatch (Match *match)
{
    if ( 0 != match )
    {
        deleteMatch (match->children.first);
        deleteMatch (match->children.second);

        delete match;
    }
}

void
TournamentState::endOfMatch (IPlayer::PlayerSide winner,
                             uint32_t leftPlayerScore,
                             uint32_t rightPlayerScore)
{
    // Check if both players are computers, to play the correct sound later.
    bool bothComputer = getCurrentMatch ()->opponents.first.isComputerPlayer &&
                        getCurrentMatch ()->opponents.second.isComputerPlayer;
    // Accumulate the scores.
    getCurrentMatch ()->opponents.first.score += leftPlayerScore;
    getCurrentMatch ()->opponents.second.score += rightPlayerScore;

    Player winnerPlayer;
    if ( IPlayer::LeftSide == winner )
    {
        winnerPlayer = getCurrentMatch ()->opponents.first;
        getCurrentMatch ()->opponents.first.characterIndex =
            k_InvalidCharacterIndex;
    }
    else
    {
        winnerPlayer = getCurrentMatch ()->opponents.second;
        getCurrentMatch ()->opponents.second.characterIndex =
            k_InvalidCharacterIndex;
    }

    if ( getCurrentMatch()->parent == &m_Matches )
    {
        setCurrentMatch (&m_Matches);
        m_Matches.opponents.first = winnerPlayer;
    }
    else if ( getCurrentMatch () ==
              getCurrentMatch ()->parent->children.first )
    {
        getCurrentMatch ()->parent->opponents.first = winnerPlayer;
        if ( 0 != getCurrentMatch ()->parent->children.second )
        {
            setCurrentMatch (getCurrentMatch ()->parent->children.second);
        }
        else
        {
            setCurrentMatch (getCurrentMatch ()->parent);
        }
    }
    else
    {
        getCurrentMatch ()->parent->opponents.second = winnerPlayer;
        setCurrentMatch (getCurrentMatch ()->parent);
    }

    // Play the "winner" sound effect if the winner is human, otherwise
    // play the "lose" sound effect.
    if ( winnerPlayer.isComputerPlayer )
    {
        if ( bothComputer )
        {
            m_SoundWin->play ();
        }
        else
        {
            m_SoundLose->play ();
        }
    }
    else
    {
        m_SoundWin->play ();
    }
}

///
/// \brief Gets the best two players states based on the characters.
///
/// \param leftCharacter The information about the left player's character.
/// \param rightCharacter The information about the right player's character.
/// \return The best two players state based on \p leftCharacter and
///         \p rightCharacter fully initialized and ready to be set as
///         active.
///
IState *
TournamentState::getBestTwoPlayersState (const Player &leftCharacter,
                                         const Player &rightCharacter)
{
    IState *twoPlayersState = 0;
    if ( leftCharacter.isComputerPlayer && rightCharacter.isComputerPlayer )
    {
        twoPlayersState =
            new TwoComputerPlayersState (leftCharacter.computerPlayerLevel,
                                         rightCharacter.computerPlayerLevel,
                                         this);
    }
    else
    {
        IPlayer *leftPlayer = getMatchPlayer (leftCharacter, IPlayer::LeftSide);
        IPlayer *rightPlayer = getMatchPlayer (rightCharacter, IPlayer::RightSide);
        twoPlayersState =
            new TwoPlayersState (leftPlayer, rightPlayer,
                                 AIPlayerFactory::getRandomBackgroundFileName (),
                                 leftCharacter.score, rightCharacter.score,
                                 this);
    }

    return twoPlayersState;
}

///
/// \brief Gets the current match.
///
/// \return The pointer to the current match.
///
inline TournamentState::Match *
TournamentState::getCurrentMatch (void)
{
    return m_CurrentMatch;
}

///
/// \brief Gets the time remaining to change the current match blink state.
///
/// \return The time remaining until the application should change the
///         blinking state of the current match's opponents.
///
inline int32_t
TournamentState::getCurrentMatchBlinkTime (void) const
{
    return m_CurrentMatchBlinkTime;
}

///
/// \brief Gets the time remaining to start the current match.
///
/// \return The time remaining until the current match starts.
///
inline int32_t
TournamentState::getCurrentMatchStartTime (void) const
{
    return m_CurrentMatchStartTime;
}

///
/// \brief Gets a player of a match.
///
/// \param player The definition of the match's player to create the object
///               from.
/// \param side The side the player will play on.
/// \return The IPlayer created from the definition of \p player.
///
IPlayer *
TournamentState::getMatchPlayer (const Player &player,
                                 IPlayer::PlayerSide side) const
{
    IPlayer *matchPlayer = 0;
    if ( player.isComputerPlayer )
    {
        matchPlayer = AIPlayerFactory::create (player.computerPlayerLevel, side);
    }
    else
    {
        matchPlayer = new HumanPlayer (side);
    }

    return matchPlayer;
}

void
TournamentState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
TournamentState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    switch (button)
    {
        case SDL_CONTROLLER_BUTTON_A:
        case SDL_CONTROLLER_BUTTON_X:
            startCurrentMatch ();
            break;

        case SDL_CONTROLLER_BUTTON_START:
        case SDL_CONTROLLER_BUTTON_B:
            System::getInstance ().pause ();
            break;
    }
}

void
TournamentState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
TournamentState::keyDown (uint32_t key)
{
    switch ( key )
    {
        case SDLK_ESCAPE:
            System::getInstance ().pause ();
            break;

        case SDLK_RETURN:
            startCurrentMatch ();
            break;
    }
}

void
TournamentState::keyUp (uint32_t key)
{
}

///
/// \brief Loads graphic resources.
///
void
TournamentState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("menuBackground.png")));
    {
        std::ostringstream fileName;
        fileName << "Tournament";
        fileName << m_NumPlayers;
        fileName << "p.png";
        std::unique_ptr<Surface> grid (
                Surface::fromFile (File::getGraphicsFilePath (fileName.str ())));
        grid->blit (m_Background->getWidth () / 2 - grid->getWidth () / 2,
                    m_Background->getHeight () / 2 - grid->getHeight () / 2,
                    m_Background->toSDLSurface ());
    }
    m_Background->resize (screenScale);

    m_Faces.reset (
            Surface::fromFile (File::getGraphicsFilePath ("faces.png")));
    m_Faces->resize (screenScale);
}

void
TournamentState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
TournamentState::render (SDL_Surface *screen)
{
    renderMatch (&m_Matches, screen, k_InitialHorizontalPosition);
}

///
/// \brief Recursively draw a match and all its children.
///
/// Renders a match and all its children in their appropriate position.
///
/// \param match The match to render along with all its children.
/// \param screen The screen surface to render the matches to.
/// \param initialX The central X where the matches should be rendered to.
///                 This function takes into account the horizontal offset
///                 between the position of the opponents.
/// \param level The current recursive level.  The first call it must be 0.
///
void
TournamentState::renderMatch (Match *match, SDL_Surface *screen,
                              uint16_t initialX, uint8_t level)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    const uint16_t faceSize = static_cast<uint16_t> (k_FaceSize * screenScale);

    if ( 0 != match )
    {
        if ( getCurrentMatch () != match || showCurrentOpponents () )
        {
            if ( match->opponents.first.characterIndex != k_InvalidCharacterIndex )
            {
                uint8_t character = match->opponents.first.characterIndex;
                uint16_t x = static_cast<uint16_t> ((initialX
                                                     - m_HorizontalOffset[level]) *
                                                    screenScale);
                uint16_t y = static_cast<uint16_t> (m_VerticalPosition[level] *
                                                    screenScale);

                m_Faces->blit (character % k_FacesPerRow * faceSize,
                               character / k_FacesPerRow * faceSize,
                               faceSize, faceSize, x, y, screen);
            }
            if ( match->opponents.second.characterIndex != k_InvalidCharacterIndex )
            {
                uint8_t character = match->opponents.second.characterIndex;
                uint16_t x = static_cast<uint16_t> ((initialX
                                                     + m_HorizontalOffset[level]) *
                                                    screenScale);
                uint16_t y = static_cast<uint16_t> (m_VerticalPosition[level] *
                                                    screenScale);

                m_Faces->blit (character % k_FacesPerRow * faceSize,
                               character / k_FacesPerRow * faceSize,
                               faceSize, faceSize, x, y, screen);
            }
        }
        renderMatch (match->children.first, screen,
                     initialX - m_HorizontalOffset[level], level + 1);
        renderMatch (match->children.second, screen,
                     initialX + m_HorizontalOffset[level], level + 1);
    }
}

///
/// \brief Tells if the current match's opponents should be shown.
///
/// \return \a true if the current match's opponents should be rendered,
///         \a false otherwise.
///
inline bool
TournamentState::showCurrentOpponents (void) const
{
    return m_ShowCurrentOpponents;
}

///
/// \brief Changes the current match.
///
/// \param match The pointer to the match to set as current.
///
inline void
TournamentState::setCurrentMatch (Match *match)
{
    m_CurrentMatch = match;
}

///
/// \brief Sets the time until the blink state should change.
///
/// \param blinkTime The time to wait until the blink state of the current
///                  match's opponents should change.
///
inline void
TournamentState::setCurrentMatchBlinkTime (int32_t blinkTime)
{
    m_CurrentMatchBlinkTime = blinkTime;
}

///
/// \brief Sets the time until the current match starts.
///
/// \param startTime The time to wait until the current match starts.
///
inline void
TournamentState::setCurrentMatchStartTime (int32_t startTime)
{
    m_CurrentMatchStartTime = startTime;
}

///
/// \brief Sets if show the current match's opponents or not.
///
/// \param showCurrentOpponents set to \a true if the match's opponents should
///                             be shown.  Otherwise set to \a false.
///
inline void
TournamentState::setShowCurrentOpponents (bool showCurrentOpponents)
{
    m_ShowCurrentOpponents = showCurrentOpponents;
}

///
/// \brief Starts the current match.
///
void
TournamentState::startCurrentMatch (void)
{
    if ( getCurrentMatch () == &m_Matches )
    {
        System::getInstance ().removeActiveState (false);
        Player winner = m_Matches.opponents.first;
        // We don't want a computer player to enter as high score.
        uint32_t score = winner.isComputerPlayer ? 0 : winner.score;
        System::getInstance ().setActiveState (
                new CongratulationsState (winner.name, score));
    }
    else
    {
        System::getInstance ().setActiveState (
                getBestTwoPlayersState (getCurrentMatch ()->opponents.first,
                                        getCurrentMatch ()->opponents.second),
                                        System::FadeIn);
        System::getInstance ().setActiveState (
                new VersusState (getCurrentMatch ()->opponents.second.name,
                                 getCurrentMatch ()->opponents.first.name));

        setCurrentMatchStartTime (k_MatchStartTime);
    }
}

void
TournamentState::update (uint32_t elapsedTime)
{
    setCurrentMatchBlinkTime (getCurrentMatchBlinkTime () - elapsedTime);
    if ( 0 >= getCurrentMatchBlinkTime () )
    {
        setCurrentMatchBlinkTime (getCurrentMatchBlinkTime () + k_BlinkTime);
        setShowCurrentOpponents (!showCurrentOpponents ());
    }

    setCurrentMatchStartTime (getCurrentMatchStartTime () - elapsedTime);
    if ( 0 >= getCurrentMatchStartTime () )
    {
        startCurrentMatch ();
    }
}

void
TournamentState::videoModeChanged (void)
{
    loadGraphicResources ();
}
