//
// Cross-platform free Puyo-Puyo clone.
// Copyright (C) 2006, 2007 Emma's software
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
#include <sstream>
#include <cstdlib>
#include "ControlSetupState.h"
#include "File.h"
#include "Joystick.h"
#include "System.h"

using namespace Amoebax;

//
// Constants.
//
/// The X position of the controls' title.
static const uint16_t k_ControlTitlesX = 80;
/// The Y position of the first control's position.
static const uint16_t k_FirstControlY = 250;
/// The first control index which is a joystick button control.
static const uint8_t k_FirstJoystickButtonControlIndex = 3;
/// The number of controls each player has.
static const uint16_t k_NumControls = 5;
/// The X position of the left player's controls.
static const uint16_t k_LeftPlayerControlsX = 700;
/// The X position of the right player's controls.
static const uint16_t k_RightPlayerControlsX = 1140;
/// The Y position of the player's title.
static const uint16_t k_PlayersTitleY = 150;
/// The default time the current control is visible when waiting input (ms.)
static const int32_t k_VisibleTime = 200;


/// The index of the defaults "control"...
static const uint16_t k_DefaultsControlIndex = k_NumControls * 2;
/// The index of the accept "control"...
static const uint16_t k_AcceptControlIndex = k_DefaultsControlIndex + 1;
/// The index of the cancel "control"...
static const uint16_t k_CancelControlIndex = k_AcceptControlIndex + 1;

///
/// \brief Default constructor.
///
/// \param leftControls The current left player's controls.
/// \param rightControls The current right player's controls.
///
ControlSetupState::ControlSetupState (Options::PlayerControls &leftControls,
                                      Options::PlayerControls &rightControls):
    IState (),
    m_Background (nullptr),
    m_CurrentControlVisible (true),
    m_Font (nullptr),
    m_FontSelected (nullptr),
    m_JoyControls (0),
    m_KeyControls (0),
    m_LeftPlayerControls (leftControls),
    m_LeftPlayerControlsOriginal (leftControls),
    m_RightPlayerControls (rightControls),
    m_RightPlayerControlsOriginal (rightControls),
    m_SelectedControl (0),
    m_SelectedPlayer (0),
    m_StateRemoved (false),
    m_VisibleTime (k_VisibleTime),
    m_WaitingForInput (false)
{
    loadGraphicResources ();

    m_KeyControls.push_back (&m_LeftPlayerControls.keyboard.moveRight);
    m_KeyControls.push_back (&m_LeftPlayerControls.keyboard.moveLeft);
    m_KeyControls.push_back (&m_LeftPlayerControls.keyboard.pushDown);
    m_KeyControls.push_back (&m_LeftPlayerControls.keyboard.rotateClockwise);
    m_KeyControls.push_back (&m_LeftPlayerControls.keyboard.rotateCounterClockwise);
    m_KeyControls.push_back (&m_RightPlayerControls.keyboard.moveRight);
    m_KeyControls.push_back (&m_RightPlayerControls.keyboard.moveLeft);
    m_KeyControls.push_back (&m_RightPlayerControls.keyboard.pushDown);
    m_KeyControls.push_back (&m_RightPlayerControls.keyboard.rotateClockwise);
    m_KeyControls.push_back (&m_RightPlayerControls.keyboard.rotateCounterClockwise);

    m_JoyControls.push_back (&m_LeftPlayerControls.joystick.moveRight);
    m_JoyControls.push_back (&m_LeftPlayerControls.joystick.moveLeft);
    m_JoyControls.push_back (&m_LeftPlayerControls.joystick.pushDown);
    m_JoyControls.push_back (&m_LeftPlayerControls.joystick.rotateClockwise);
    m_JoyControls.push_back (&m_LeftPlayerControls.joystick.rotateCounterClockwise);
    m_JoyControls.push_back (&m_RightPlayerControls.joystick.moveRight);
    m_JoyControls.push_back (&m_RightPlayerControls.joystick.moveLeft);
    m_JoyControls.push_back (&m_RightPlayerControls.joystick.pushDown);
    m_JoyControls.push_back (&m_RightPlayerControls.joystick.rotateClockwise);
    m_JoyControls.push_back (&m_RightPlayerControls.joystick.rotateCounterClockwise);
}

///
/// \brief Selects the current selected control to wait for input.
///
void
ControlSetupState::activateCurrentControl (void)
{
    switch ( getSelectedGlobalControl () )
    {
        case k_AcceptControlIndex:
            applyPlayersControls ();
            removeState ();
            break;

        case k_CancelControlIndex:
            removeState ();
            break;

        case k_DefaultsControlIndex:
            Options::getInstance ().setDefaultPlayersControls ();
            readPlayersControls ();
            break;

        default:
            setWaitingForInput (true);
            setVisibleTime (k_VisibleTime);
            break;
    }
}

///
/// \brief Sets the current players' controls to options.
///
void
ControlSetupState::applyPlayersControls (void)
{
    m_LeftPlayerControlsOriginal = m_LeftPlayerControls;
    m_RightPlayerControlsOriginal = m_RightPlayerControls;
}

///
/// \brief Assigns a joystick event to the currently selected control.
///
/// \param joystick The joystick event to assign to the current
///                 selected control.
///
inline void
ControlSetupState::assignJoystickToCurrentControl (int16_t joystick)
{
    *(m_JoyControls[getSelectedGlobalControl ()]) = joystick;
    cancelWaitingForInput ();
}

///
/// \brief Assigns a key to the currently selected control.
///
/// \param key The key to assign to the current selected control.
///
inline void
ControlSetupState::assignKeyToCurrentControl (uint32_t key)
{
    *(m_KeyControls[getSelectedGlobalControl ()]) = key;
    cancelWaitingForInput ();
}

///
/// \brief Cancels the waiting for the control's input.
///
void
ControlSetupState::cancelWaitingForInput (void)
{
    setWaitingForInput (false);
    setCurrentControlVisible (true);
}

///
/// \brief Draws a player's controls.
///
/// \param keyboard Set to \a true if the controls are for keyboard, \a false
///                 if they are for joystick.
/// \param offset The offset of the player's controls in the list.
/// \param x The x position to draw the controls to.
/// \param screenScale The current screen scale factor.
/// \param screen The destination surface to draw the controls to.
///
void
ControlSetupState::drawControls (bool keyboard, uint16_t offset, uint16_t x,
                                 float screenScale, SDL_Surface *screen)
{
    for ( uint16_t currentControl = 0 ; currentControl < 5 ; ++currentControl )
    {
        uint16_t globalControlIndex = currentControl + offset;
        Font *font = m_Font.get ();
        bool visible = true;
        if ( getSelectedGlobalControl () == globalControlIndex )
        {
            font = m_FontSelected.get ();
            visible = isCurrentControlVisible ();
        }
        if ( visible )
        {
            if ( keyboard )
            {
                drawKeyboardControl (*(m_KeyControls[globalControlIndex]), x,
                        currentControl, screenScale, font, screen);
            }
            else
            {
                drawJoystickControl (*(m_JoyControls[globalControlIndex]), x,
                        currentControl, screenScale, font, screen);
            }
        }
    }

}

///
/// \brief Draw a player's joystick control.
///
/// \param control The joystick command to draw.
/// \param x The X position to draw the control's names right aligned.
/// \param controlIndex The index of the control, starting by 0 and incrementing
///                     at each control.
/// \param scale The current screen's scale factor.
/// \param font The pointer to the font to use to draw the control.
/// \param screen The destination surface to draw the control to.
///
void
ControlSetupState::drawJoystickControl (int16_t control, uint16_t x,
                                        uint16_t controlIndex, float scale,
                                        Font *font, SDL_Surface *screen)
{
    std::string joyName;
    if ( controlIndex < k_FirstJoystickButtonControlIndex )
    {
        // Is an axis.
        std::ostringstream name;
        name << (control < 0 ? "-" : "+") << "Axis " << std::abs (control) - 1;
        joyName = name.str ();
    }
    else
    {
        // Is a button.
        std::ostringstream name;
        name << "button " << control;
        joyName = name.str ();
    }
    uint16_t textWidth = font->getTextWidth (joyName);
    uint16_t y = static_cast<uint16_t> (k_FirstControlY * scale) +
                 font->getHeight () * controlIndex;
    font->write (joyName, x - textWidth, y, screen);
}


///
/// \brief Draw a player's keyboard control.
///
/// \param control The key id to draw.
/// \param x The X position to draw the control's names right aligned.
/// \param controlIndex The index of the control, starting by 0 and incrementing
///                     at each control.
/// \param scale The current screen's scale factor.
/// \param font The pointer to the font to use to draw the control.
/// \param screen The destination surface to draw the control to.
///
void
ControlSetupState::drawKeyboardControl (uint32_t control, uint16_t x,
                                        uint16_t controlIndex, float scale,
                                        Font *font, SDL_Surface *screen)
{
    std::string keyName (SDL_GetKeyName (static_cast<SDL_Keycode> (control)));
    uint16_t textWidth = font->getTextWidth (keyName);
    uint16_t y = static_cast<uint16_t> (k_FirstControlY * scale) +
                 font->getHeight () * controlIndex;
    font->write (keyName, x - textWidth, y, screen);
}

///
/// \brief Gets the index of the currently selected control.
///
/// \return The index of the selected control, from 0 to 4.
///
inline uint16_t
ControlSetupState::getSelectedControl (void) const
{
    return m_SelectedControl;
}


///
/// \brief Get the time the selected control is visible.
///
/// \return The time in ms. the selected control should be visible when
///         waiting for input.
///
inline int32_t
ControlSetupState::getVisibleTime (void) const
{
    return m_VisibleTime;
}

///
/// \brief Gets the index of the currently selected control, in global.
///
/// \return The index of the selected control. From 0 to 4 is a control of
///         the left player. From 5 to 9 is a control of the right player.
///         Anything else is just another menu option.
///
inline uint16_t
ControlSetupState::getSelectedGlobalControl (void) const
{
    return getSelectedControl () + k_NumControls * getSelectedPlayer ();
}

///
/// \brief Gets the currently selected player.
///
/// \return 0 if the left player is selected, 1 if the right player is
///         selected.
///
inline uint16_t
ControlSetupState::getSelectedPlayer (void) const
{
    return m_SelectedPlayer;
}

///
/// \brief Tells if the current selected control is visible.
///
/// \return \a true if the current selected control is visible, \a false
///         otherwise.
///
inline bool
ControlSetupState::isCurrentControlVisible (void) const
{
    return m_CurrentControlVisible;
}

///
/// \brief Tells if a joystick index is valid for the currently selected player.
///
/// \param joystick The joystick index.
/// \return \a true if the joystick index is the correct for the currently
///         selected player, \a false otherwise.
///
inline bool
ControlSetupState::isJoystickIndexValid (uint8_t joystick) const
{
    return ( 0 == getSelectedPlayer () &&
             m_LeftPlayerControls.joystick.index == joystick ) ||
           ( 1 == getSelectedPlayer () &&
             m_RightPlayerControls.joystick.index == joystick );
}

///
/// \brief Tells if a key is valid for a control.
///
/// The ESCAPE key is not a valid key for a control, as well as keys
/// already defined in other controls.
///
/// \param key The key to check if its valid or not.
/// \return \a true if the key is valid for a control, \a false otherwise.
///
bool
ControlSetupState::isKeyValidForControl (uint32_t key) const
{
    bool valid = true;
    if ( SDLK_ESCAPE == key )
    {
        valid = false;
    }
    else
    {
        uint32_t *selectedControl = m_KeyControls[getSelectedGlobalControl ()];
        for (std::vector<uint32_t *>::const_iterator currentControl = m_KeyControls.begin () ;
             currentControl != m_KeyControls.end () ; ++currentControl )
        {
            if ( *currentControl != selectedControl &&
                 key == *(*(currentControl)) )
            {
                valid = false;
            }
        }
    }

    return valid;
}

///
/// \brief Tells if the "regular" menu, instead of controls, is selected.
///
/// \return \a true if either the accept or cancel elements are selected.
///         \a false otherwise.
///
inline bool
ControlSetupState::isRegularMenuSelected (void) const
{
    return getSelectedGlobalControl () >= k_DefaultsControlIndex;
}

///
/// \brief Tells if the control is waiting for input.
///
/// \return \a true if the control is waiting for input, \a false otherwise.
///
inline bool
ControlSetupState::isWaitingForInput (void) const
{
    return m_WaitingForInput;
}

///
/// \brief Tells if the waiting input if for joystick.
///
/// \return \a true if we are waiting input for joystick, \a false otherwise.
///
inline bool
ControlSetupState::isWaitingJoystickInput (void) const
{
    return isWaitingForInput () && (
           ( 0 == getSelectedPlayer () &&
             Options::JoystickControls == m_LeftPlayerControls.controlsType ) ||
           ( 1 == getSelectedPlayer () &&
             Options::JoystickControls == m_RightPlayerControls.controlsType));
}

///
/// \brief Tells if the waiting input if for joystick.
///
/// \return \a true if we are waiting input for joystick, \a false otherwise.
///
inline bool
ControlSetupState::isWaitingKeyboardInput (void) const
{
    return isWaitingForInput () && (
           ( 0 == getSelectedPlayer () &&
             Options::KeyboardControls == m_LeftPlayerControls.controlsType ) ||
           ( 1 == getSelectedPlayer () &&
             Options::KeyboardControls == m_RightPlayerControls.controlsType));
}

void
ControlSetupState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
    if ( (value > Joystick::k_DeadZone || value < -Joystick::k_DeadZone) &&
         isWaitingForInput () && isWaitingJoystickInput () &&
         getSelectedControl () < k_FirstJoystickButtonControlIndex &&
         isJoystickIndexValid (joystick) )
    {
        assignJoystickToCurrentControl (value < 0 ? -axis - 1 : axis + 1);
    }
}

void
ControlSetupState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    if ( isWaitingForInput () && isWaitingJoystickInput () &&
         getSelectedControl () >= k_FirstJoystickButtonControlIndex &&
         isJoystickIndexValid (joystick) )
    {
        assignJoystickToCurrentControl (button);
    }
}

void
ControlSetupState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
ControlSetupState::keyDown (uint32_t key)
{
    if ( isWaitingForInput () )
    {
        if ( isWaitingKeyboardInput () && isKeyValidForControl (key) )
        {
            assignKeyToCurrentControl (key);
        }
        else if ( SDLK_ESCAPE == key )
        {
            cancelWaitingForInput ();
        }
    }
    else
    {
        switch (key)
        {
            case SDLK_DOWN:
                selectNextControl ();
                break;

            case SDLK_ESCAPE:
                selectCancelOrExit ();
                break;

            case SDLK_LEFT:
                selectPreviousPlayer ();
                break;

            case SDLK_RETURN:
                activateCurrentControl ();
                break;

            case SDLK_RIGHT:
                selectNextPlayer ();
                break;

            case SDLK_UP:
                selectPreviousControl ();
                break;
        }
    }
}

void
ControlSetupState::keyUp (uint32_t key)
{
}

///
/// \brief Loads all graphic resources.
///
void
ControlSetupState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    m_Background.reset (Surface::fromFile (
                File::getGraphicsFilePath ("menuBackground.png")));
    {
        std::unique_ptr<Surface> title (Surface::fromFile (
                    File::getGraphicsFilePath ("playerscontrolsetup.png")));
        title->blit (m_Background->getWidth () / 2 - title->getWidth () / 2,
                     0, m_Background->toSDLSurface ());

        std::unique_ptr<Surface> controls (Surface::fromFile (
                    File::getGraphicsFilePath ("controls.png")));
        controls->blit (k_ControlTitlesX, k_FirstControlY,
                        m_Background->toSDLSurface ());

        std::unique_ptr<Surface> leftPlayer (Surface::fromFile (
                    File::getGraphicsFilePath ("leftplayer.png")));
        leftPlayer->blit (k_LeftPlayerControlsX - leftPlayer->getWidth (),
                          k_PlayersTitleY, m_Background->toSDLSurface ());

        std::unique_ptr<Surface> rightPlayer (Surface::fromFile (
                    File::getGraphicsFilePath ("rightplayer.png")));
        rightPlayer->blit (k_RightPlayerControlsX - rightPlayer->getWidth (),
                          k_PlayersTitleY, m_Background->toSDLSurface ());
    }
    m_Background->resize (screenScale);

    m_Font.reset (Font::fromFile (File::getFontFilePath ("fontMenu")));
    m_FontSelected.reset (Font::fromFile (
                File::getFontFilePath ("fontMenuSelected")));
}

void
ControlSetupState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

///
/// \brief Reads the players' controls from the options.
///
void
ControlSetupState::readPlayersControls (void)
{
    m_LeftPlayerControls =
        Options::getInstance ().getPlayerControls (IPlayer::LeftSide);
    m_LeftPlayerControls.controlsType =
        m_LeftPlayerControlsOriginal.controlsType;
    m_LeftPlayerControls.joystick.index =
        m_LeftPlayerControlsOriginal.joystick.index;

    m_RightPlayerControls =
        Options::getInstance ().getPlayerControls (IPlayer::RightSide);
    m_RightPlayerControls.controlsType =
        m_RightPlayerControlsOriginal.controlsType;
    m_RightPlayerControls.joystick.index =
        m_RightPlayerControlsOriginal.joystick.index;
}

///
/// \brief Removes the state from the system, if not already removed.
///
void
ControlSetupState::removeState (void)
{
    if ( !m_StateRemoved )
    {
        System::getInstance ().removeActiveState ();
        m_StateRemoved = true;
    }
}

void
ControlSetupState::render (SDL_Surface *screen)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    // Draw left player's current control.
   drawControls (Options::KeyboardControls == m_LeftPlayerControls.controlsType,
                 0,
                 static_cast<uint16_t> (screenScale * k_LeftPlayerControlsX),
                 screenScale, screen);
    // Draw right player's current control.
   drawControls (Options::KeyboardControls == m_RightPlayerControls.controlsType,
                 k_NumControls,
                 static_cast<uint16_t> (screenScale * k_RightPlayerControlsX),
                 screenScale, screen);

   // Draw the accept and cancel options.
   uint16_t y = static_cast<uint16_t> (k_FirstControlY * screenScale) +
                (k_NumControls + 1) * m_Font->getHeight ();

   Font *font = m_Font.get ();
   if ( getSelectedGlobalControl () == k_DefaultsControlIndex )
   {
       font = m_FontSelected.get ();
   }
   font->write ("set defaults", y, screen);
   y += font->getHeight ();

   font = m_Font.get ();
   if ( getSelectedGlobalControl () == k_AcceptControlIndex )
   {
       font = m_FontSelected.get ();
   }
   font->write ("accept", y, screen);
   y += font->getHeight ();

   font = m_Font.get ();
   if ( getSelectedGlobalControl () == k_CancelControlIndex )
   {
       font = m_FontSelected.get ();
   }
   font->write ("cancel", y, screen);
}

///
/// \brief Selects the cancel control or exits, if it's already selected.
///
void
ControlSetupState::selectCancelOrExit (void)
{
    if ( k_CancelControlIndex == getSelectedGlobalControl () )
    {
        removeState ();
    }
    else
    {
        setSelectedPlayer (0);
        setSelectedControl (k_CancelControlIndex);
    }
}

///
/// \brief Selects the next control.
///
void
ControlSetupState::selectNextControl (void)
{
    if ( isRegularMenuSelected () )
    {
        if ( getSelectedControl () < k_CancelControlIndex )
        {
            setSelectedControl (getSelectedControl () + 1);
        }
    }
    else
    {
        if ( getSelectedControl () < (k_NumControls - 1) )
        {
            setSelectedControl (getSelectedControl () + 1);
        }
        else
        {
            setSelectedControl (k_DefaultsControlIndex);
            setSelectedPlayer (0);
        }
    }
}

///
/// \brief Selects the next player.
///
void
ControlSetupState::selectNextPlayer (void)
{
    if ( !isRegularMenuSelected () )
    {
        if ( 0 == getSelectedPlayer () )
        {
            setSelectedPlayer (getSelectedPlayer () + 1);
        }
    }
}

///
/// \brief Selectes the previous control.
///
void
ControlSetupState::selectPreviousControl (void)
{
    if ( isRegularMenuSelected () )
    {
        if ( getSelectedControl () > k_DefaultsControlIndex )
        {
            setSelectedControl (getSelectedControl () - 1);
        }
        else
        {
            setSelectedControl (k_NumControls - 1);
        }
    }
    else
    {
        if ( 0 < getSelectedControl () )
        {
            setSelectedControl (getSelectedControl () - 1);
        }
    }
}

///
/// \brief Selects the previous player.
///
void
ControlSetupState::selectPreviousPlayer (void)
{
    if ( !isRegularMenuSelected () )
    {
        if ( 0 < getSelectedPlayer () )
        {
            setSelectedPlayer (getSelectedPlayer () - 1);
        }
    }
}

///
/// \brief Sets the visibility of the current selected control.
///
/// \param visible \a true if the current selected control must be visible,
///                \a false otherwise.
///
inline void
ControlSetupState::setCurrentControlVisible (bool visible)
{
    m_CurrentControlVisible = visible;
}

///
/// \brief Sets the selected control.
///
/// \param control The index of the selected control, from 0 to
///                k_NumControls - 1.
///
inline void
ControlSetupState::setSelectedControl (uint16_t control)
{
    m_SelectedControl = control;
}

///
/// \brief Sets the selected player.
///
/// \param player The index of the selected player: 0 left player,
///               1 right player.
///
inline void
ControlSetupState::setSelectedPlayer (uint16_t player)
{
    m_SelectedPlayer = player;
}

///
/// \brief Sets the time the selected control is visible.
///
/// \param time The time the selected control is visible while waiting
///             for input.
///
inline void
ControlSetupState::setVisibleTime (int32_t time)
{
    m_VisibleTime = time;
}

///
/// \brief Sets if a control is waiting for input.
///
/// \param waiting Tells if the selected control is waiting for input
///
inline void
ControlSetupState::setWaitingForInput (bool waiting)
{
    m_WaitingForInput = waiting;
}

void
ControlSetupState::update (uint32_t elapsedTime)
{
    if ( isWaitingForInput () )
    {
        setVisibleTime (getVisibleTime () - elapsedTime);
        if ( 0 > getVisibleTime () )
        {
            setCurrentControlVisible (!isCurrentControlVisible ());
            setVisibleTime (k_VisibleTime);
        }
    }
}

void
ControlSetupState::videoModeChanged (void)
{
    loadGraphicResources ();
}
