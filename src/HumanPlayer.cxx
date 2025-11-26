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
#include "HumanPlayer.h"

using namespace Amoebax;


///
/// \brief Default constructor.
///
/// \param side The side the player is controlling.
///
HumanPlayer::HumanPlayer (IPlayer::PlayerSide side):
    IPlayer (side),
    m_Joystick (),
    m_PlayerControls ()
{
    loadOptions ();
}


void
HumanPlayer::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
    switch (m_Joystick.motion (joystick, axis, value))
    {
        case Joystick::DownPressed:
            getGrid ()->setMaxFallingSpeed ();
            break;

        case Joystick::DownReleased:
            getGrid ()->setNormalFallingSpeed ();
            break;

        case Joystick::LeftPressed:
            getGrid ()->moveLeft ();
            break;

        case Joystick::RightPressed:
            getGrid ()->moveRight ();
            break;

        default:
            // Do nothing.
            break;
    }
}

void
HumanPlayer::joyDown (uint8_t joystick, uint8_t button)
{
#if defined (IS_GP2X_HOST)
    switch (button)
    {
        case GP2X_BUTTON_A:
        case GP2X_BUTTON_L:
            getGrid ()->rotateCounterClockwise ();
        break;

        case GP2X_BUTTON_B:
        case GP2X_BUTTON_R:
            getGrid ()->rotateClockwise ();
        break;

        case GP2X_BUTTON_DOWN:
            getGrid ()->setMaxFallingSpeed ();
        break;

        case GP2X_BUTTON_LEFT:
        case GP2X_BUTTON_UPLEFT:
            getGrid ()->moveLeft ();
        break;

        case GP2X_BUTTON_RIGHT:
        case GP2X_BUTTON_UPRIGHT:
            getGrid ()->moveRight ();
        break;
    }
#else // !IS_GP2X_HOST
    switch ( m_Joystick.down (joystick, button) )
    {
        case Joystick::RotateCWPressed:
            getGrid ()->rotateClockwise ();
            break;

        case Joystick::RotateCCWPressed:
            getGrid ()->rotateCounterClockwise ();
            break;

        default:
            // Do nothing.
            break;
    }
#endif // IS_GP2X_HOST
}

void
HumanPlayer::joyUp (uint8_t joystick, uint8_t button)
{
#if defined (IS_GP2X_HOST)
    if ( GP2X_BUTTON_DOWN == button )
    {
        getGrid ()->setNormalFallingSpeed ();
    }
#endif // IS_GP2X_HOST
}

#if !defined (IS_GP2X_HOST)
void
HumanPlayer::keyDown (uint32_t key)
{
    if ( Options::KeyboardControls == m_PlayerControls.controlsType )
    {
        if ( m_PlayerControls.keyboard.moveLeft == key )
        {
            getGrid ()->moveLeft ();
        }
        else if ( m_PlayerControls.keyboard.moveRight == key )
        {
            getGrid ()->moveRight ();
        }
        else if ( m_PlayerControls.keyboard.pushDown == key )
        {
            getGrid ()->setMaxFallingSpeed ();
        }
        else if ( m_PlayerControls.keyboard.rotateClockwise == key )
        {
            getGrid ()->rotateClockwise ();
        }
        else if ( m_PlayerControls.keyboard.rotateCounterClockwise == key )
        {
            getGrid ()->rotateCounterClockwise ();
        }
    }
}

void
HumanPlayer::keyUp (uint32_t key)
{
    if ( Options::KeyboardControls == m_PlayerControls.controlsType &&
         m_PlayerControls.keyboard.pushDown == key )
    {
        getGrid ()->setNormalFallingSpeed ();
    }
}
#endif // !IS_GP2X_HOST

void
HumanPlayer::loadOptions (void)
{
    m_PlayerControls = Options::getInstance ().getPlayerControls (getSide ());
    m_Joystick.setControls (m_PlayerControls);
}
