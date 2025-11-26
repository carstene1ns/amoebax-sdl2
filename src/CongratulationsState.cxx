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
#include "File.h"
#include "Font.h"
#include "NewHighScoreState.h"
#include "Options.h"
#include "System.h"
#include "CongratulationsState.h"

using namespace Amoebax;

/// The position of the player's feet on the podium.
static uint16_t k_PlayerVerticalPosition = 524;

///
/// \brief Default constructor.
///
/// \param playerName The name of the player to congratulate.
/// \param playerScore The score of the player.
///
CongratulationsState::CongratulationsState (const std::string &playerName,
                                            uint32_t playerScore):
    m_Background (0),
    m_BackgroundMusic (Music::fromFile (File::getMusicFilePath ("Congratulations.ogg"))),
    m_End (false),
    m_PlayerName (playerName),
    m_PlayerScore (playerScore)
{
    loadGraphicResources ();
}

void
CongratulationsState::activate (void)
{
    if ( Music::isPaused () )
    {
        Music::resume ();
    }
    else
    {
        m_BackgroundMusic->play ();
    }
}

///
/// \brief Signals that the versus screen must end.
///
/// This is used when a key or joystick is pressed or when
/// the time elapsed is greater than the time to wait to tell
/// to the CongratulationsState class that the next update() it must remove
/// itself from the System. See mustEnd().
///
inline void
CongratulationsState::endCongratulationsState (void)
{
    m_End = true;
}

///
/// \brief Gets the name of the player to congratulate.
///
/// \return The name of the player to congratulate.
///
inline const std::string &
CongratulationsState::getPlayerName (void) const
{
    return m_PlayerName;
}

///
/// \brief Gets the score of the player to congratulate.
///
/// \return The score of the player congratulate.
///
inline uint32_t
CongratulationsState::getPlayerScore (void) const
{
    return m_PlayerScore;
}

void
CongratulationsState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
CongratulationsState::joyDown (uint8_t joystick, uint8_t button)
{
    endCongratulationsState ();
}

void
CongratulationsState::joyUp (uint8_t joystick, uint8_t button)
{
}

#if !defined (IS_GP2X_HOST)
void
CongratulationsState::keyDown (uint32_t key)
{
    endCongratulationsState ();
}

void
CongratulationsState::keyUp (uint32_t key)
{
}
#endif // !IS_GP2X_HOST

///
/// \brief Loads all graphical resources.
///
void
CongratulationsState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
#if defined (IS_GP2X_HOST)
    const float originalScale = screenScale;
#else // !IS_GP2X_HOST
    const float originalScale = 1.0f;
#endif // IS_GP2X_HOST

    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("menuBackground.png")));
    {
        std::auto_ptr<Surface> podium (
                Surface::fromFile (File::getGraphicsFilePath ("congratulationsPodium.png")));
        podium->blit (m_Background->getWidth () / 2 - podium->getWidth () / 2,
                      m_Background->getHeight () - podium->getHeight (),
                      m_Background->toSDLSurface ());

        std::auto_ptr<Surface> player (
                Surface::fromFile (File::getGraphicsFilePath (getPlayerName () + ".png")));
        player->blit (m_Background->getWidth () / 2 - player->getWidth () / 2,
                      static_cast<uint16_t> (k_PlayerVerticalPosition *
                                             originalScale) -
                      player->getHeight (),
                      m_Background->toSDLSurface ());

        std::auto_ptr<Surface> congratulations (
                Surface::fromFile (File::getGraphicsFilePath ("congratulations.png")));
        congratulations->blit (m_Background->getWidth () / 2 -
                              congratulations->getWidth () / 2, 0,
                              m_Background->toSDLSurface ());
    }
    m_Background->resize (screenScale);
}

///
/// \brief Tells if the versus screen should end.
///
/// \return \a true if the versus screen should end, \a false otherwise.
///
inline bool
CongratulationsState::mustEnd (void) const
{
    return m_End;
}

void
CongratulationsState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
CongratulationsState::render (SDL_Surface *screen)
{
}

void
CongratulationsState::update (uint32_t elapsedTime)
{
    if ( mustEnd () )
    {
        System::getInstance ().removeActiveState ();
        if ( Options::getInstance ().isHighScore (getPlayerScore ()) )
        {
            System::getInstance ().setActiveState (
                    new NewHighScoreState (getPlayerScore ()));
        }
    }
}

void
CongratulationsState::videoModeChanged (void)
{
}
