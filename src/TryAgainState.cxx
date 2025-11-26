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
#include "File.h"
#include "Font.h"
#include "Options.h"
#include "TryAgainState.h"
#include "System.h"

using namespace Amoebax;

// The Y position of the try again title.
static const uint16_t k_TryAgainYPos = 100;

///
/// \brief Default constructor.
///
TryAgainState::TryAgainState (void):
    IState ()
{
    loadGraphicResources ();

    // Initialize menu options.
    m_MenuOptions.push_back (new YesOption ("Yes"));
    m_MenuOptions.push_back (new NoOption ("No"));
    m_SelectedOption = m_MenuOptions.begin ();
}

void
TryAgainState::activate (void)
{
    Music::pause ();
}

///
/// \brief Activates the currently selected menu option.
///
void
TryAgainState::activateMenuOption (void)
{
    (*(*m_SelectedOption)) ();
}

void
TryAgainState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
TryAgainState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
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

        // The X button resumes the game.
        case SDL_CONTROLLER_BUTTON_B:
            System::getInstance ().removeActiveState ();
        break;
    }
}

void
TryAgainState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
TryAgainState::keyDown (uint32_t key)
{
    switch (key)
    {
        case SDLK_DOWN:
            selectNextMenuOption ();
        break;

        // The escape key resumes the game.
        case SDLK_ESCAPE:
            System::getInstance ().removeActiveState ();
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
TryAgainState::keyUp (uint32_t key)
{
}

///
/// \brief Loads all graphic resources.
///
void
TryAgainState::loadGraphicResources (void)
{
    // Capture the background from the screen, and blit an alpha blended
    // black surface.
    m_Background.reset (Surface::fromScreen ());
    SDL_Surface *blackBox =
        SDL_CreateRGBSurface (0,
                              m_Background->getWidth (),
                              m_Background->getHeight (),
                              m_Background->toSDLSurface ()->format->BitsPerPixel,
                              m_Background->toSDLSurface ()->format->Rmask,
                              m_Background->toSDLSurface ()->format->Gmask,
                              m_Background->toSDLSurface ()->format->Bmask,
                              m_Background->toSDLSurface ()->format->Amask);
    SDL_FillRect (blackBox, NULL, SDL_MapRGB (blackBox->format, 0, 0, 0));
    SDL_SetSurfaceAlphaMod (blackBox, 128);
    SDL_BlitSurface (blackBox, NULL, m_Background->toSDLSurface (), NULL);
    {
        std::unique_ptr<Surface> title (
                Surface::fromFile (File::getGraphicsFilePath ("TryAgain.png")));
        title->blit (m_Background->getWidth () / 2 - title->getWidth () / 2,
                    static_cast<uint16_t>(k_TryAgainYPos *
                    System::getInstance ().getScreenScaleFactor ()),
                    m_Background->toSDLSurface ());
    }

    // Load fonts.
    m_Font.reset (Font::fromFile (File::getFontFilePath ("fontMenu")));
    m_FontSelected.reset (
            Font::fromFile (File::getFontFilePath ("fontMenuSelected")));
}

///
/// \brief Selects the next menu's option.
///
/// If the currently selected option is the last one, the next
/// selected option will be the first.
///
void
TryAgainState::selectNextMenuOption (void)
{
    ++m_SelectedOption;
    if ( m_SelectedOption == m_MenuOptions.end () )
    {
        m_SelectedOption = m_MenuOptions.begin ();
    }
}

///
/// \brief Selects the previous menu's option.
///
/// If the currently selected option if the first option,
/// the next selected option will be the last.
///
void
TryAgainState::selectPreviousMenuOption (void)
{
    if ( m_SelectedOption == m_MenuOptions.begin () )
    {
        m_SelectedOption = m_MenuOptions.end () - 1;
    }
    else
    {
        --m_SelectedOption;
    }
}

void
TryAgainState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
TryAgainState::render (SDL_Surface *screen)
{
    const uint16_t fontHeight = m_Font->getHeight ();
    const uint16_t initialY =
        static_cast<uint16_t>(Options::getInstance ().getScreenHeight () / 2 -
                              fontHeight * m_MenuOptions.size () / 2);

    std::vector<IOption *>::iterator currentOption = m_MenuOptions.begin ();
    for ( uint16_t y = initialY ;
          currentOption != m_MenuOptions.end () ;
          ++currentOption, y += fontHeight )
    {
        if ( currentOption == m_SelectedOption )
        {
            m_FontSelected->write ((*currentOption)->getTitle (), y, screen);
        }
        else
        {
            m_Font->write ((*currentOption)->getTitle (), y, screen);
        }
    }
}

void
TryAgainState::update (uint32_t elapsedTime)
{
}

void
TryAgainState::videoModeChanged (void)
{
    loadGraphicResources ();
}

////////////////////////////////////////////////////////////////
// Menu options.
////////////////////////////////////////////////////////////////
void
TryAgainState::YesOption::operator() (void)
{
    System::getInstance ().removeActiveState ();
}

void
TryAgainState::NoOption::operator() (void)
{
    System::getInstance ().returnToMainMenu ();
}
