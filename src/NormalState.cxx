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
#if defined (HAVE_CONFIG_H)
#include <config.h>
#endif // HAVE_CONFIG_H
#include <SDL.h>
#include "AIPlayerFactory.h"
#include "CongratulationsState.h"
#include "File.h"
#include "HumanPlayer.h"
#include "NormalState.h"
#include "System.h"
#include "TryAgainState.h"
#include "TwoPlayersState.h"
#include "VersusState.h"

using namespace Amoebax;

// The human player's side.
IPlayer::PlayerSide k_PlayerSide = IPlayer::RightSide;
// The opponent's player side.
IPlayer::PlayerSide k_OpponentSide = IPlayer::LeftSide;


///
/// \brief Default constructor.
///
/// \param characterName The name to use as the player's character name.
///
NormalState::NormalState (const std::string &characterName):
    IState (),
    m_CharacterName (characterName),
    m_CurrentOpponentLevel (0),
    m_CurrentScore (0),
    m_SoundLose (Sound::fromFile (File::getSoundFilePath ("youlose.wav"))),
    m_SoundWin (Sound::fromFile (File::getSoundFilePath ("youwin.wav"))),
    m_TryAgain (false),
    m_Winner (IPlayer::RightSide)
{
    loadGraphicsResources ();
}

void
NormalState::activate (void)
{
}

///
/// \brief Tells the state to ask the user to try again on next update.
///
inline void
NormalState::askTryAgain (void)
{
    m_TryAgain = true;
}

void
NormalState::endOfMatch (IPlayer::PlayerSide winner,
                          uint32_t leftPlayerScore, uint32_t rightPlayerScore)
{
    m_Winner = winner;
    if ( k_PlayerSide == winner )
    {
        m_SoundWin->play ();
        incrementOpponentLevel ();
        // Only store the player's score if it wins. See ticket #25.
        m_CurrentScore = k_PlayerSide == IPlayer::LeftSide ? leftPlayerScore :
                                                             rightPlayerScore;
    }
    else
    {
        m_SoundLose->play ();
        askTryAgain ();
    }
}

///
/// \brief Gets the name of the player's character.
///
/// \return The name of the player's character.
///
inline const std::string &
NormalState::getCharacterName (void) const
{
    return m_CharacterName;
}

///
/// \brief Gets the current opponent's level.
///
/// \return The current opponent's level.
///
uint8_t
NormalState::getCurrentOpponentLevel (void) const
{
    return m_CurrentOpponentLevel;
}

///
/// \brief Gets the current player's score.
///
/// \return The current player's score.
///
inline uint32_t
NormalState::getCurrentScore (void) const
{
    return m_CurrentScore;
}

///
/// \brief Increments the opponent's AI level.
///
inline void
NormalState::incrementOpponentLevel (void)
{
    ++m_CurrentOpponentLevel;
    m_TryAgain = false;
}

void
NormalState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
NormalState::joyDown (uint8_t joystick, uint8_t button)
{
}

void
NormalState::joyUp (uint8_t joystick, uint8_t button)
{
}

#if !defined (IS_GP2X_HOST)
void
NormalState::keyDown (uint32_t key)
{
}

void
NormalState::keyUp (uint32_t key)
{
}
#endif // !IS_GP2X_HOST

///
/// \brief Loads all graphical resources.
///
void
NormalState::loadGraphicsResources (void)
{
}

///
/// \brief Tells if must ask to try again or not.
///
/// \return \a true if we should ask the user to try again, \a false if not.
///
inline bool
NormalState::mustAskTryAgain (void) const
{
    return m_TryAgain;
}

void
NormalState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
}

void
NormalState::render (SDL_Surface *screen)
{
}

void
NormalState::update (uint32_t elapsedTime)
{
    if ( getCurrentOpponentLevel () < AIPlayerFactory::k_MaxPlayerLevel )
    {
        AIPlayer *opponent = AIPlayerFactory::create (getCurrentOpponentLevel (),
                                                      k_OpponentSide);
        std::string backgroundFileName =
                AIPlayerFactory::getBackgroundFileName (getCurrentOpponentLevel ());
        uint32_t leftScore = k_PlayerSide == IPlayer::LeftSide ? getCurrentScore () :
                                                                 0;
        uint32_t rightScore = k_PlayerSide == IPlayer::RightSide ? getCurrentScore () :
                                                                   0;

        System::getInstance ().setActiveState(
                new TwoPlayersState (opponent, new HumanPlayer (k_PlayerSide),
                                     backgroundFileName,
                                     leftScore, rightScore,
                                     this), System::FadeIn);
        if ( mustAskTryAgain () )
        {
            System::getInstance ().setActiveState (new TryAgainState (),
                                                   System::FadeNone);
        }
        else
        {
            System::getInstance ().setActiveState (
                new VersusState (getCharacterName (),
                                 AIPlayerFactory::getPlayerName (getCurrentOpponentLevel ())),
                                 System::FadeIn);
        }
    }
    else
    {
        System::getInstance ().removeActiveState (false);
        System::getInstance ().setActiveState (
                new CongratulationsState (getCharacterName (),
                                          getCurrentScore ()));
    }

}

void
NormalState::videoModeChanged (void)
{
}
