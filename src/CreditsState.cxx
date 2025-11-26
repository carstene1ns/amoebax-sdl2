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
#include <SDL.h>
#include <sstream>
#include "CreditsState.h"
#include "File.h"
#include "Font.h"
#include "Options.h"
#include "OptionsMenuState.h"
#include "System.h"
#include "TrainingState.h"

using namespace Amoebax;

///
/// \brief Default constructor.
///
CreditsState::CreditsState (void):
    IState (),
    m_Background (nullptr),
    m_NamesFont (nullptr),
    m_SectionsFont (nullptr),
    m_StateRemoved (false)
{
    loadGraphicResources ();
}

void
CreditsState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
CreditsState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    removeCreditsState ();
}

void
CreditsState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
CreditsState::keyDown (uint32_t key)
{
    removeCreditsState ();
}

void
CreditsState::keyUp (uint32_t key)
{
}

///
/// \brief Loads all graphic resources.
///
void
CreditsState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("menuBackground.png")));
    {
        std::unique_ptr<Surface> title (
                Surface::fromFile (File::getGraphicsFilePath("credits.png")));
        title->blit (m_Background->getWidth () / 2 -
                     title->getWidth () / 2, 0, m_Background->toSDLSurface ());
    }
    m_Background->resize (screenScale);

    // Load fonts.
    m_SectionsFont.reset (Font::fromFile (File::getFontFilePath ("fontMenu")));
    m_NamesFont.reset (Font::fromFile (File::getFontFilePath ("fontMenuSelected")));

}

///
/// \brief Removes the credits state if it's the active state.
///
void
CreditsState::removeCreditsState ()
{
    // if we call the removeActiveState() function twice (i.e.,press two keys
    // at once) not only the credits state would be removed, but the main
    // menu state as well, and the game would quit.  To prevent that, and since
    // we have no method to check which state is active, we only allow the
    // removeActiveState() function to be called once.
    if ( !m_StateRemoved )
    {
        System::getInstance ().removeActiveState ();
        m_StateRemoved = true;
    }
}

void
CreditsState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);

    // The credits won't change, so draw as if they were part of the
    // background.
    const uint16_t fontHeight = m_SectionsFont->getHeight ();
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    // Get the horizontal possition for the sections and the names.
    const uint16_t titleX = static_cast<uint16_t>(240 * screenScale);
    const uint16_t nameX = static_cast<uint16_t>(400 * screenScale);
    // Get the initial vertical possition to start to write.
    uint16_t y = static_cast<uint16_t>((225 + fontHeight / 2.0) * screenScale);

    // Draw coders' name.
    m_SectionsFont->write (std::string ("Code"), titleX, y, screen);
    y += fontHeight;
    m_NamesFont->write (std::string ("Jordi Fita"), nameX, y, screen);
    y += static_cast<uint16_t> (fontHeight * 1.5);

    // Draw graphists' name.
    m_SectionsFont->write (std::string ("Graphics"), titleX, y, screen);
    y += fontHeight;
    m_NamesFont->write (std::string ("Safareig Creatiu"), nameX, y, screen);
    y += static_cast<uint16_t> (fontHeight * 1.5);

    // Draw musicians' name.
    m_SectionsFont->write (std::string ("Music & Sound"), titleX, y, screen);
    y += fontHeight;
    m_NamesFont->write (std::string ("Alex Almarza"), nameX, y, screen);
    y += static_cast<uint16_t> (fontHeight * 1.5);

    // Draw web artists' name.
    m_SectionsFont->write (std::string ("Web Page"), titleX, y, screen);
    y += fontHeight;
    m_NamesFont->write (std::string ("Ferran Brugat"), nameX, y, screen);
}

void
CreditsState::render (SDL_Surface *screen)
{
}

void
CreditsState::update (uint32_t elapsedTime)
{
}

void
CreditsState::videoModeChanged (void)
{
    loadGraphicResources ();
}
