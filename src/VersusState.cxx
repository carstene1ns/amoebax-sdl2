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
#include "File.h"
#include "Font.h"
#include "System.h"
#include "VersusState.h"

using namespace Amoebax;

/// The time to wait, in ms., until the versus state ends.
static const uint32_t k_TimeToWait = 5000;

// The separation in pixels from the center to a player in the versus screen.
static const uint16_t k_PlayersSeparation = 200;

///
/// \brief Constructor.
///
/// \param player The name of the player's avatar.
/// \param opponent The name of the opponent player.
///
VersusState::VersusState(const std::string &player,
                         const std::string &opponent):
    m_Background (nullptr),
    m_BackgroundMusic (nullptr),
    m_End (false),
    m_Opponent (opponent),
    m_Player (player),
    m_TimeElapsed (0)
{
    loadGraphicResources ();
}

void
VersusState::activate (void)
{
    if ( !Music::isPlaying () )
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
/// \brief Signals that the versus screen must end.
///
/// This is used when a key or joystick is pressed or when
/// the time elapsed is greater than the time to wait to tell
/// to the VersusState class that the next update() it must remove
/// itself from the System. See mustEnd().
///
inline void
VersusState::endVersusState (void)
{
    m_End = true;
}

///
/// \brief Gets the opponent's name.
///
/// \return The opponent's name.
///
inline const std::string &
VersusState::getOpponentName (void) const
{
    return m_Opponent;
}

///
/// \brief Gets the player's avatar name.
///
/// \return The player's avatar name.
///
inline const std::string &
VersusState::getPlayerName (void) const
{
    return m_Player;
}

void
VersusState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
VersusState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    endVersusState ();
}

void
VersusState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
VersusState::keyDown (uint32_t key)
{
    endVersusState ();
}

void
VersusState::keyUp (uint32_t key)
{
}

///
/// \brief Loads all graphical resources.
///
void
VersusState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    uint16_t playersSeparation = k_PlayersSeparation;

    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("menuBackground.png")));

    std::unique_ptr<Font> font (
            Font::fromFile (File::getFontFilePath ("fontMenu")));
    {
        std::unique_ptr<Surface> player (
                Surface::fromFile (
                    File::getGraphicsFilePath (getPlayerName () + ".png")));
        int16_t x = m_Background->getWidth () / 2 + playersSeparation;
        int16_t y = m_Background->getHeight () / 2 - player->getHeight () / 2;
        player->blit (x, y, m_Background->toSDLSurface ());

        x += player->getWidth () / 2 - font->getTextWidth (getPlayerName ()) / 2;
        y += player->getHeight () + font->getHeight ();
        font->write (getPlayerName (), x, y, m_Background->toSDLSurface ());
    }
    {
        std::unique_ptr<Surface> opponent (
                Surface::fromFile (
                    File::getGraphicsFilePath (getOpponentName () + ".png")));
        int16_t x = m_Background->getWidth () / 2 - playersSeparation -
                    opponent->getWidth ();
        int16_t y = m_Background->getHeight () / 2 - opponent->getHeight () / 2;
        opponent->blit (x, y, m_Background->toSDLSurface ());

        x += opponent->getWidth () / 2 -
             font->getTextWidth (getOpponentName ()) / 2;
        y += opponent->getHeight () + font->getHeight ();
        font->write (getOpponentName (), x, y, m_Background->toSDLSurface ());
    }
    {
        std::unique_ptr<Surface> versus (
                Surface::fromFile (File::getGraphicsFilePath ("versus.png")));
        versus->blit (m_Background->getWidth () / 2 - versus->getWidth () / 2,
                      m_Background->getHeight () / 2 - versus->getHeight () / 2,
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
VersusState::mustEnd (void) const
{
    return m_End;
}

void
VersusState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
VersusState::render (SDL_Surface *screen)
{
}

void
VersusState::update (uint32_t elapsedTime)
{
    if ( mustEnd () )
    {
        System::getInstance ().removeActiveState ();
    }
    else
    {
        m_TimeElapsed += elapsedTime;
        if ( m_TimeElapsed >= k_TimeToWait )
        {
            endVersusState ();
        }
    }
}

void
VersusState::videoModeChanged (void)
{
}
