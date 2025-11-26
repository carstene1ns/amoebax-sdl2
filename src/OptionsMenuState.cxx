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
#include <algorithm>
#include "ControlSetupState.h"
#include "File.h"
#include "Font.h"
#include "OptionsMenuState.h"
#include "Options.h"
#include "System.h"
#include "TrainingState.h"

using namespace Amoebax;

///
/// \brief Default constructor.
///
OptionsMenuState::OptionsMenuState (void):
    IState (),
    m_Background (nullptr),
    m_Font (nullptr),
    m_FontSelected (nullptr),
    m_LeftControls (Options::getInstance ().getPlayerControls (IPlayer::LeftSide)),
    m_MenuOptions (),
    m_RightControls (Options::getInstance ().getPlayerControls (IPlayer::RightSide)),
    m_SelectedOption ()
{
    loadGraphicResources ();

    // Initialize menu options option.
    m_MenuOptions.push_back (new ScreenResolutionOption ("screen resolution"));
    m_MenuOptions.push_back (new ScreenModeOption ("screen mode"));
    m_MenuOptions.push_back (new VolumeOption ("volume"));
    m_MenuOptions.push_back (new SpaceOption ());
    m_MenuOptions.push_back (new PlayerControlsOption ("left controls",
                                                       IPlayer::LeftSide,
                                                       m_LeftControls));
    m_MenuOptions.push_back (new PlayerControlsOption ("right controls",
                                                       IPlayer::RightSide,
                                                       m_RightControls));
    m_MenuOptions.push_back (new ChangeControlKeysOption ("players control setup",
                m_LeftControls, m_RightControls));
    m_MenuOptions.push_back (new SpaceOption ());
    m_MenuOptions.push_back (new ApplyOption ("apply", m_MenuOptions));
    m_MenuOptions.push_back (new BackOption ("back"));
    m_SelectedOption = m_MenuOptions.begin();
}

///
/// \brief Destructor.
///
OptionsMenuState::~OptionsMenuState (void)
{
    std::for_each (m_MenuOptions.begin (), m_MenuOptions.end (),
                   DeleteObject<IOption> ());
}

///
/// \brief Actives the currently selected option.
///
void
OptionsMenuState::activateMenuOption (void)
{
    (*(*m_SelectedOption)) ();
}

void
OptionsMenuState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
OptionsMenuState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
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

        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            (*m_SelectedOption)->previous ();
        break;

        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            (*m_SelectedOption)->next ();
        break;

        case SDL_CONTROLLER_BUTTON_DPAD_UP:
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            selectPreviousMenuOption ();
        break;

        case SDL_CONTROLLER_BUTTON_B:
            selectBackOption ();
        break;
    }
}

void
OptionsMenuState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
OptionsMenuState::keyDown (uint32_t key)
{
    switch (key)
    {
        case SDLK_DOWN:
            selectNextMenuOption ();
        break;

        case SDLK_ESCAPE:
            selectBackOption ();
        break;

        case SDLK_LEFT:
            (*m_SelectedOption)->previous ();
        break;

        case SDLK_RETURN:
            activateMenuOption ();
        break;

        case SDLK_RIGHT:
            (*m_SelectedOption)->next ();
        break;

        case SDLK_UP:
            selectPreviousMenuOption ();
        break;
    }
}

void
OptionsMenuState::keyUp (uint32_t key)
{
}

///
/// \brief Loads all graphic resources.
///
void
OptionsMenuState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();

    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("menuBackground.png")));
    {
        std::unique_ptr<Surface> title (Surface::fromFile (
                    File::getGraphicsFilePath ("options.png")));
        title->blit (m_Background->getWidth () / 2 - title->getWidth () / 2,
                     0, m_Background->toSDLSurface ());
    }
    m_Background->resize (screenScale);

    // Load fonts.
    m_Font.reset (Font::fromFile (File::getFontFilePath ("fontMenu")));
    m_FontSelected.reset (
            Font::fromFile (File::getFontFilePath ("fontMenuSelected")));
}

///
/// \brief Selects the back menu options.
///
/// If the back menu option is not selected, it gets selected. If the
/// back options is already selected, then it gets activated.
///
/// \note This code assumes the back options is the last one.
///
void
OptionsMenuState::selectBackOption (void)
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
OptionsMenuState::selectNextMenuOption (void)
{
    ++m_SelectedOption;
    if ( m_SelectedOption == m_MenuOptions.end () )
    {
        m_SelectedOption = m_MenuOptions.begin ();
    }

    if ( !(*m_SelectedOption)->isSelectable () )
    {
        selectNextMenuOption ();
    }
}

///
/// \brief Selects the previous menu's option.
///
/// If the current selected option is the first option, the
/// next selected option will be the last.
///
void
OptionsMenuState::selectPreviousMenuOption (void)
{
    if ( m_SelectedOption == m_MenuOptions.begin () )
    {
        m_SelectedOption = m_MenuOptions.end () - 1;
    }
    else
    {
        --m_SelectedOption;
    }

    if ( !(*m_SelectedOption)->isSelectable () )
    {
        selectPreviousMenuOption ();
    }
}

void
OptionsMenuState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
OptionsMenuState::render (SDL_Surface *screen)
{
    const uint16_t fontHeight = m_Font->getHeight ();
    const uint16_t initialY =
        static_cast<uint16_t>(Options::getInstance ().getScreenHeight () / 2 -
                              fontHeight * (m_MenuOptions.size () - 3) / 2);
    std::vector<IOption *>::iterator currentOption = m_MenuOptions.begin ();
    for ( uint16_t y = initialY ;
          currentOption != m_MenuOptions.end () ;
          ++currentOption, y += fontHeight)
    {
        if ( currentOption == m_SelectedOption )
        {
            (*currentOption)->render (y, m_FontSelected.get (), screen);
        }
        else
        {
            (*currentOption)->render (y, m_Font.get (), screen);
        }
    }
}

void
OptionsMenuState::update (uint32_t elapsedTime)
{
}

void
OptionsMenuState::videoModeChanged (void)
{
    loadGraphicResources ();
}

////////////////////////////////////////////////////////////////
// Menu Options.
////////////////////////////////////////////////////////////////
void
OptionsMenuState::ApplyOption::operator () (void)
{
    std::for_each (m_Options.begin (), m_Options.end (),
                   std::mem_fn (&IOption::apply));
    System::getInstance ().applyVideoMode ();
}

void
OptionsMenuState::ApplyOption::render (uint16_t y, Font *font, SDL_Surface *screen)
{
    font->write (getTitle (), y, screen);
}

void
OptionsMenuState::BackOption::operator () (void)
{
    System::getInstance ().removeActiveState ();
}

void
OptionsMenuState::BackOption::render (uint16_t y, Font *font, SDL_Surface *screen)
{
    font->write (getTitle (), y, screen);
}

void
OptionsMenuState::ChangeControlKeysOption::operator () (void)
{
    System::getInstance ().setActiveState (
            new ControlSetupState (m_LeftControls, m_RightControls));
}

void
OptionsMenuState::PlayerControlsOption::apply (void)
{
    Options::getInstance ().setPlayerControls (m_Side, m_Controls);
}

void
OptionsMenuState::PlayerControlsOption::next (void)
{
    if ( Options::KeyboardControls != m_Controls.controlsType )
    {
        if ( m_Controls.joystick.index < 3 )
        {
            ++m_Controls.joystick.index;
        }
        else
        {
            m_Controls.controlsType = Options::KeyboardControls;
        }
    }
    else
    {
        m_Controls.controlsType = Options::JoystickControls;
        m_Controls.joystick.index = 0;
    }
}

void
OptionsMenuState::PlayerControlsOption::previous (void)
{
    if ( Options::KeyboardControls != m_Controls.controlsType )
    {
        if ( m_Controls.joystick.index > 0 )
        {
            --m_Controls.joystick.index;
        }
        else
        {
            m_Controls.controlsType = Options::KeyboardControls;
        }
    }
    else
    {
        m_Controls.controlsType = Options::JoystickControls;
        m_Controls.joystick.index = 3;
    }
}

void
OptionsMenuState::PlayerControlsOption::render (uint16_t y, Font *font, SDL_Surface *screen)
{
    std::string controls = "keyboard";
    if ( m_Controls.controlsType != Options::KeyboardControls )
    {
        std::ostringstream joystick;
        joystick << "joystick " << m_Controls.joystick.index + 1;
        controls = joystick.str ();
    }
    font->write (getTitle () + std::string(": ") + controls, y, screen);
}

void
OptionsMenuState::ScreenModeOption::apply (void)
{
    Options::getInstance ().setFullScreen (m_FullScreen);
}

void
OptionsMenuState::ChangeControlKeysOption::render (uint16_t y, Font *font,
                                              SDL_Surface *screen)
{
    font->write (getTitle (), y, screen);
}

void
OptionsMenuState::ScreenModeOption::next (void)
{
    m_FullScreen = !m_FullScreen;
}

void
OptionsMenuState::ScreenModeOption::previous (void)
{
    // since this is a boolean option selecting the previous option is
    // the same a selecting the next option.
    next ();
}

void
OptionsMenuState::ScreenModeOption::render (uint16_t y, Font *font,
                                       SDL_Surface *screen)
{
    font->write (getTitle () + std::string (": ") +
                 (m_FullScreen ? std::string ("full screen") :
                                 std::string ("windowed")),
                 y, screen);
}

void
OptionsMenuState::ScreenResolutionOption::apply (void)
{
    Options::getInstance ().setScreenWidth (m_CurrentMode->first);
    Options::getInstance ().setScreenHeight (m_CurrentMode->second);
}

OptionsMenuState::ScreenResolutionOption::ScreenResolutionOption (const std::string &title):
    OptionsMenuState::IOption (title)
{
    m_Modes.push_back (std::make_pair (1280, 960));
    m_Modes.push_back (std::make_pair (1024, 768));
    m_Modes.push_back (std::make_pair (800, 600));
    m_Modes.push_back (std::make_pair (640, 480));

    unsigned int height = Options::getInstance ().getScreenHeight ();
    unsigned int width = Options::getInstance ().getScreenWidth ();
    m_CurrentMode = m_Modes.begin ();
    while ( m_CurrentMode != m_Modes.end () &&
            m_CurrentMode->first != width && m_CurrentMode->second != height )
    {
        ++m_CurrentMode;
    }

    if ( m_CurrentMode == m_Modes.end () )
    {
        --m_CurrentMode;
    }
}

void
OptionsMenuState::ScreenResolutionOption::next (void)
{
    if ( m_CurrentMode != m_Modes.begin () )
    {
        --m_CurrentMode;
    }

}

void
OptionsMenuState::ScreenResolutionOption::previous (void)
{
    ++m_CurrentMode;
    if ( m_CurrentMode == m_Modes.end () )
    {
        --m_CurrentMode;
    }
}

void
OptionsMenuState::ScreenResolutionOption::render (uint16_t y, Font *font,
                                             SDL_Surface *screen)
{
    std::stringstream resolution;
    resolution << m_CurrentMode->first << "x" << m_CurrentMode->second;
    font->write (getTitle () + std::string(": ") + resolution.str(),
                 y, screen);
}

void
OptionsMenuState::VolumeOption::next (void)
{
    Options::getInstance ().incrementVolume ();
    System::getInstance ().applyVolumeLevel ();
}

void
OptionsMenuState::VolumeOption::previous (void)
{
    Options::getInstance ().decrementVolume ();
    System::getInstance ().applyVolumeLevel ();
}

void
OptionsMenuState::VolumeOption::render (uint16_t y, Font *font, SDL_Surface *screen)
{
    std::ostringstream volume;
    volume << getTitle () << ": ";
    volume << 100 * Options::getInstance ().getVolumeLevel () / Options::getMaxVolumeLevel ();
    volume << "%";
    font->write (volume.str (), y, screen);
}
