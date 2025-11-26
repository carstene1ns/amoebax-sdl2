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
#if !defined (AMOEBAX_HUMAN_PLAYER_H)
#define AMOEBAX_HUMAN_PLAYER_H

#include "IPlayer.h"
#include "Joystick.h"
#include "Options.h"

namespace Amoebax
{
    ///
    /// \class HumanPlayer
    /// \brief A player controlled by keyboard or joystick.
    ///
    class HumanPlayer: public IPlayer
    {
        public:
            explicit HumanPlayer (IPlayer::PlayerSide side);

            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value);
            virtual void joyDown (uint8_t joystick, uint8_t button);
            virtual void joyUp (uint8_t joystick, uint8_t button);
#if !defined (IS_GP2X_HOST)
            virtual void keyDown (uint32_t key);
            virtual void keyUp (uint32_t key);
#endif // !IS_GP2X_HOST

        private:
            void loadOptions (void);

            /// The joystick handler.
            Joystick m_Joystick;
            /// The player's controls.
            Options::PlayerControls m_PlayerControls;
    };
}

#endif // !AMOEBAX_HUMAN_PLAYER_H
