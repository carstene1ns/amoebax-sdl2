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
#include <algorithm>
#include "AdvancedAIPlayer.h"
#include "AnticipatoryAIPlayer.h"
#include "CreditsState.h"
#include "DemoState.h"
#include "DumbAIPlayer.h"
#include "File.h"
#include "Font.h"
#include "HighScoreState.h"
#include "HumanPlayer.h"
#include "MainMenuState.h"
#include "NormalSetupState.h"
#include "OptionsMenuState.h"
#include "Options.h"
#include "SimpleAIPlayer.h"
#include "System.h"
#include "TournamentMenuState.h"
#include "TrainingState.h"
#include "TwoPlayersState.h"

using namespace Amoebax;

///
/// \brief Default constructor.
///
MainMenuState::MainMenuState (void):
    IState (),
    m_Background (nullptr),
    m_BackgroundMusic (Music::fromFile (File::getMusicFilePath ("menu.ogg"))),
    m_Font (nullptr),
    m_FontSelected (nullptr),
    m_MenuOptions (),
    m_SelectedOption (m_MenuOptions.begin ()),
    m_TimeToDemo (k_TimeToDemo)
{
    loadGraphicResources ();

    // Initialize menu option.
    m_MenuOptions.push_back (new TrainingOption ("training"));
    m_MenuOptions.push_back (new NormalOption ("normal"));
    m_MenuOptions.push_back (new TournamentOption ("tournament"));
    m_MenuOptions.push_back (new HighScoresOption ("high scores"));
    m_MenuOptions.push_back (new OptionsOption ("options"));
    m_MenuOptions.push_back (new CreditsOption ("credits"));
    m_MenuOptions.push_back (new ExitOption ("exit"));
    m_SelectedOption = m_MenuOptions.begin ();
}

///
/// \brief Destructor.
///
MainMenuState::~MainMenuState (void)
{
    std::for_each (m_MenuOptions.begin (), m_MenuOptions.end (),
                   DeleteObject<IOption> ());
}

void
MainMenuState::activate (void)
{
    resetTimeToDemo ();
    if ( !Music::isPlaying () )
    {
        m_BackgroundMusic->play ();
    }
}

///
/// \brief Activates the currently selected menu option.
///
void
MainMenuState::activateMenuOption (void)
{
    (*(*m_SelectedOption)) ();
}

///
/// \brief Gets the time until the demo is activated.
///
inline int32_t
MainMenuState::getTimeToDemo (void) const
{
    return m_TimeToDemo;
}

void
MainMenuState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
    resetTimeToDemo ();
}

void
MainMenuState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    resetTimeToDemo ();
    switch (button)
    {
        case SDL_CONTROLLER_BUTTON_A:
        case SDL_CONTROLLER_BUTTON_X:
        case SDL_CONTROLLER_BUTTON_START:
            activateMenuOption ();
        break;

        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            selectNextMenuOption ();
        break;

        case SDL_CONTROLLER_BUTTON_DPAD_UP:
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            selectPreviousMenuOption ();
        break;

        case SDL_CONTROLLER_BUTTON_B:
            selectExitOption ();
        break;
    }
}

void
MainMenuState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
MainMenuState::keyDown (uint32_t key)
{
    resetTimeToDemo ();
    switch (key)
    {
        case SDLK_DOWN:
            selectNextMenuOption ();
        break;

        case SDLK_ESCAPE:
            selectExitOption ();
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
MainMenuState::keyUp (uint32_t key)
{
}

///
/// \brief Loads all graphic resources.
///
void
MainMenuState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();

    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("menuBackground.png")));
    m_Background->resize (screenScale);
    {
        std::unique_ptr<Surface> title (
                Surface::fromFile (File::getGraphicsFilePath ("amoebax.png")));
        title->resize (screenScale);
        title->blit (Options::getInstance ().getScreenWidth () / 2 -
                     title->getWidth () / 2, 0, m_Background->toSDLSurface ());
    }

    // Load fonts.
    m_Font.reset (Font::fromFile (File::getFontFilePath ("fontMenu")));
    m_FontSelected.reset (
            Font::fromFile (File::getFontFilePath ("fontMenuSelected")));
}

///
/// \brief Selects the exit menu options.
///
/// If the exit menu option is not selected, it gets selected. If the
/// exit options is already selected, then it gets activated.
///
/// \note This code assumes the exit options is the last one.
///
void
MainMenuState::selectExitOption (void)
{
    if ( m_SelectedOption == m_MenuOptions.end () - 1 )
    {
        activateMenuOption ();
    }
    else
    {
        m_SelectedOption = m_MenuOptions.end () - 1;
    }
}

///
/// \brief Selects the next menu's option.
///
/// If the current selected option is the last one, the next
/// selected option will be the first.
///
void
MainMenuState::selectNextMenuOption (void)
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
/// If the current selected option is the first option, the
/// next selected option will be the last.
///
void
MainMenuState::selectPreviousMenuOption (void)
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
MainMenuState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
MainMenuState::render (SDL_Surface *screen)
{
    const uint16_t fontHeight = m_Font->getHeight ();
    const uint16_t initialY =
        static_cast<uint16_t>(340 *
                              System::getInstance ().getScreenScaleFactor ());
    std::vector<IOption *>::iterator currentOption = m_MenuOptions.begin ();
    for ( uint16_t y = initialY ;
          currentOption != m_MenuOptions.end () ;
          ++currentOption, y += fontHeight)
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

///
/// \brief Sets the time to demo at its default value.
///
void
MainMenuState::resetTimeToDemo (void)
{
    setTimeToDemo (k_TimeToDemo);
}

///
/// \brief Sets a new time to activate the demo.
///
/// \param time The time to wait to activate the demo.
///
inline void
MainMenuState::setTimeToDemo (int32_t time)
{
    m_TimeToDemo = time;
}

void
MainMenuState::update (uint32_t elapsedTime)
{
    setTimeToDemo (getTimeToDemo () - elapsedTime);
    if ( getTimeToDemo () <= 0 )
    {
        resetTimeToDemo ();
        System::getInstance ().setActiveState (new DemoState ());
    }
}

void
MainMenuState::videoModeChanged (void)
{
    loadGraphicResources ();
}

////////////////////////////////////////////////////////////////
// Menu options.
////////////////////////////////////////////////////////////////
void
MainMenuState::CreditsOption::operator() (void)
{
    System::getInstance ().setActiveState (new CreditsState ());
}

void
MainMenuState::ExitOption::operator() (void)
{
    // Since this is the first state, poping it from
    // the stack will end the game.
    System::getInstance ().removeActiveState ();
}

void
MainMenuState::HighScoresOption::operator() (void)
{
    System::getInstance ().setActiveState (new HighScoreState ());
}

void
MainMenuState::NormalOption::operator() (void)
{
    System::getInstance ().setActiveState (new NormalSetupState ());
}

void
MainMenuState::OptionsOption::operator() (void)
{
    System::getInstance ().setActiveState (new OptionsMenuState ());
}

void
MainMenuState::TournamentOption::operator() (void)
{
    System::getInstance ().setActiveState (new TournamentMenuState ());
}

void
MainMenuState::TrainingOption::operator() (void)
{
    System::getInstance ().setActiveState (new TrainingState ());
}
