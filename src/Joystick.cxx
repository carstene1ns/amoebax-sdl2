//
// Cross-platform free Puyo-Puyo clone.
// Copyright (C) 2007 Emma's software
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
#include <cstdlib>
#include "Joystick.h"

using namespace Amoebax;

// The possible axis' positions.
static const int8_t k_AxisLeft = -1;
static const int8_t k_AxisRight = 1;
static const int8_t k_AxisBottom = 1;
static const int8_t k_AxisCenter = 0;

///
/// \brief Default constructor.
///
Joystick::Joystick (void):
    m_AxisXDirection (k_AxisCenter),
    m_AxisYDirection (k_AxisCenter),
    m_HasConfiguration (false),
    m_PlayerControls ()
{
}

///
/// \brief Checks the action for the specified joystick's pressed button.
///
/// \param joystick The joystick index that had a button pressed.
/// \param button The button pressed.
/// \return The action that happened from the game's POV.
///
Joystick::Action
Joystick::down (uint8_t joystick, SDL_GameControllerButton button)
{
    Action action = Nothing;

    if ( hasConfiguration () &&
         Options::JoystickControls == m_PlayerControls.controlsType &&
         m_PlayerControls.joystick.index == joystick )
    {
        if ( m_PlayerControls.joystick.rotateClockwise == button )
        {
            action = RotateCWPressed;
        }
        else if ( m_PlayerControls.joystick.rotateCounterClockwise == button )
        {
            action = RotateCCWPressed;
        }
    }

    return action;
}

///
/// \brief Gets the current direction of joystick's X axis.
///
/// \return -1 if the axis is at left, 0 if at center or 1 if at right.
///
inline int8_t
Joystick::getAxisXDirection (void) const
{
    return m_AxisXDirection;
}

///
/// \brief Gets the current direction of joystick's Y axis.
///
/// \return -1 if the axis is at top, 0 if at center or 1 if at bottom.
///
inline int8_t
Joystick::getAxisYDirection (void) const
{
    return m_AxisYDirection;
}

///
/// \brief Tells if the controls were set.
///
/// \return \a true if the player's controls were set by calling setControls().
///         \a false otherwise.
///
inline bool
Joystick::hasConfiguration (void) const
{
    return m_HasConfiguration;
}

///
/// \brief Checks the action for the specified joystick's motion.
///
/// \param joystick The joystick index that had a motion event.
/// \param axis The axis index that had a motion.
/// \param value The motion's value.
/// \return The action that happened from the game's POV.
///
Joystick::Action
Joystick::motion (uint8_t joystick, uint8_t axis, int16_t value)
{
    Action action = Nothing;
    if ( hasConfiguration () &&
         Options::JoystickControls == m_PlayerControls.controlsType )
    {
        if ( m_PlayerControls.joystick.index == joystick )
        {
            if ( std::abs (m_PlayerControls.joystick.moveLeft) - 1 == axis ||
                 std::abs (m_PlayerControls.joystick.moveRight) - 1 == axis )
            {
                if ( std::abs (value) > k_DeadZone )
                {
                    if ( (value < 0 && m_PlayerControls.joystick.moveLeft < 0 ||
                          value > 0 && m_PlayerControls.joystick.moveLeft > 0 ) &&
                         k_AxisLeft != getAxisXDirection () )
                    {
                        setAxisXDirection (k_AxisLeft);
                        action = LeftPressed;
                    }
                    else if ( (value > 0 && m_PlayerControls.joystick.moveRight > 0 ||
                                value < 0 && m_PlayerControls.joystick.moveRight < 0 ) &&
                            k_AxisRight != getAxisXDirection () )
                    {
                        setAxisXDirection (k_AxisRight);
                        action = RightPressed;
                    }
                }
                else
                {
                    if ( k_AxisRight == getAxisXDirection () )
                    {
                        action = RightReleased;
                    }
                    else if ( k_AxisLeft == getAxisXDirection () )
                    {
                        action = LeftReleased;
                    }
                    setAxisXDirection (k_AxisCenter);
                }
            }
            else if ( std::abs (m_PlayerControls.joystick.pushDown) - 1 == axis )
            {
                if ( (m_PlayerControls.joystick.pushDown > 0 && value > k_DeadZone) ||
                     (m_PlayerControls.joystick.pushDown < 0 && value < -k_DeadZone) )
                {
                    if ( k_AxisBottom != getAxisYDirection () )
                    {
                        setAxisYDirection (k_AxisBottom);
                        action = DownPressed;
                    }
                }
                else
                {
                    if ( k_AxisBottom == getAxisYDirection () )
                    {
                        action = DownReleased;
                    }
                    setAxisYDirection (k_AxisCenter);
                }
            }
        }
    }

    return action;
}

///
/// \brief Checks the action for the specified joystick's released button.
///
/// \param joystick The joystick index that had a button pressed.
/// \param button The button released.
/// \return The action that happened from the game's POV.
///
Joystick::Action
Joystick::up (uint8_t joystick, SDL_GameControllerButton button)
{
    Action action = Nothing;

    if ( hasConfiguration () &&
         Options::JoystickControls == m_PlayerControls.controlsType &&
         m_PlayerControls.joystick.index == joystick )
    {
        if ( m_PlayerControls.joystick.rotateClockwise == button )
        {
            action = RotateCWReleased;
        }
        else if ( m_PlayerControls.joystick.rotateCounterClockwise == button )
        {
            action = RotateCCWReleased;
        }
    }

    return action;
}

///
/// \brief Sets the current joystick's X direction.
///
/// \param direction The current direction of the axis: -1 if it's at left,
///                  0 if it's at center, 1 if it's at right.
///
inline void
Joystick::setAxisXDirection (int8_t direction)
{
    m_AxisXDirection = direction;
}

///
/// \brief Sets the current joystick's Y direction.
///
/// \param direction The current direction of the axis: -1 if it's at top,
///                  0 at center, 1 at bottom.
///
inline void
Joystick::setAxisYDirection (int8_t direction)
{
    m_AxisYDirection = direction;
}

///
/// \brief Sets the player's controls.
///
/// \param controls The player's controls.
///
void
Joystick::setControls (const Options::PlayerControls &controls)
{
    m_PlayerControls = controls;
    m_HasConfiguration = true;
}
