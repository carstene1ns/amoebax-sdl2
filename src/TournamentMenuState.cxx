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
#include <sstream>
#include "File.h"
#include "Options.h"
#include "TournamentMenuState.h"
#include "TournamentSetupState.h"
#include "System.h"

using namespace Amoebax;

///
/// \brief Default constructor.
///
TournamentMenuState::TournamentMenuState (void):
    IState ()
{
    loadGraphicResources ();

    m_Players.push_back (2);
    m_Players.push_back (4);
    m_SelectedOption = m_Players.begin ();
}

void
TournamentMenuState::activate (void)
{
}

///
/// \brief Activates the currently selected menu option.
///
void
TournamentMenuState::activateMenuOption (void)
{
    System::getInstance ().removeActiveState (false);
    System::getInstance ().setActiveState (
            new TournamentSetupState (*m_SelectedOption));
}

void
TournamentMenuState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
TournamentMenuState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    switch (button)
    {
        case SDL_CONTROLLER_BUTTON_A:
        case SDL_CONTROLLER_BUTTON_X:
        case SDL_CONTROLLER_BUTTON_START:
            activateMenuOption ();
        break;

        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            selectNextMenuOption ();
        break;

        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            selectPreviousMenuOption ();
        break;

        case SDL_CONTROLLER_BUTTON_B:
            System::getInstance ().returnToMainMenu ();
        break;

        default:
            break;
    }
}

void
TournamentMenuState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
TournamentMenuState::keyDown (uint32_t key)
{
    switch (key)
    {
        case SDLK_DOWN:
            selectNextMenuOption ();
        break;

        case SDLK_ESCAPE:
            System::getInstance ().returnToMainMenu ();
        break;

        case SDLK_RETURN:
            activateMenuOption ();
        break;

        case SDLK_UP:
            selectPreviousMenuOption ();
        break;
    }
}

void
TournamentMenuState::keyUp (uint32_t key)
{
}

///
/// \brief Loads all graphic resources.
///
void
TournamentMenuState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("menuBackground.png")));
    {
        std::unique_ptr<Surface> title (
                Surface::fromFile (File::getGraphicsFilePath ("tournament.png")));
        title->blit (m_Background->getWidth () / 2 -
                     title->getWidth () / 2, 0, m_Background->toSDLSurface ());
    }
    m_Background->resize (screenScale);

    m_Font.reset (Font::fromFile (File::getFontFilePath ("fontMenu")));
    m_FontSelected.reset (
            Font::fromFile (File::getFontFilePath ("fontMenuSelected")));
}

///
/// \brief Selects the next menu's option.
///
/// If the currently select option is the last one, the next selected
/// option will be the first.
///
void
TournamentMenuState::selectNextMenuOption (void)
{
    ++m_SelectedOption;
    if ( m_SelectedOption == m_Players.end () )
    {
        m_SelectedOption = m_Players.begin ();
    }
}

///
/// \brief Selects the previous menu's option.
///
/// If the currently selected option is the first option,
/// the next selected option will be the last.
///
void
TournamentMenuState::selectPreviousMenuOption (void)
{
    if ( m_SelectedOption == m_Players.begin () )
    {
        m_SelectedOption = m_Players.end () - 1;
    }
    else
    {
        --m_SelectedOption;
    }
}

void
TournamentMenuState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
TournamentMenuState::render (SDL_Surface *screen)
{
    const uint16_t fontHeight = m_Font->getHeight ();
    const uint16_t initialY =
        static_cast<uint16_t>(Options::getInstance ().getScreenHeight () / 2 -
                              fontHeight * m_Players.size () / 2);

    std::vector<uint8_t>::iterator currentOption = m_Players.begin ();
    for ( uint16_t y = initialY ; currentOption != m_Players.end () ;
          ++currentOption, y += fontHeight )
    {
        std::ostringstream title;

        title << int(*currentOption) << " players";
        if ( currentOption == m_SelectedOption )
        {
            m_FontSelected->write (title.str (), y, screen);
        }
        else
        {
            m_Font->write (title.str (), y, screen);
        }
    }
}

void
TournamentMenuState::update (uint32_t elapsedTime)
{
}

void
TournamentMenuState::videoModeChanged (void)
{
    loadGraphicResources ();
}
