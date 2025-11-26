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
#if !defined (AMOEBAX_IPLAYER_H)
#define AMOEBAX_IPLAYER_H

#include <memory>
#include <SDL.h>
#include "Grid.h"

namespace Amoebax
{
    ///
    /// \class IPlayer
    /// \brief Interface class for all players.
    ///
    class IPlayer
    {
        public:
            ///
            /// \enum PlayerSide
            /// \brief The player's grid side.
            ///
            enum PlayerSide
            {
                /// The player controls the left grid.
                LeftSide = 0,
                /// The player controls the right grid or the training's grid.
                RightSide = 1
            };

            ///
            /// \brief Default constructor.
            ///
            /// \param side The side the player will control.
            ///
            explicit IPlayer (IPlayer::PlayerSide side):
                m_Grid (nullptr),
                m_Side (side)
            { }

            ///
            /// \brief Destructor.
            ///
            inline virtual ~IPlayer (void) { }

            ///
            /// \brief Gets the player's grid.
            ///
            /// \return The grid that the player is controlling.
            ///
            Grid *getGrid (void) const
            {
                return m_Grid.get ();
            }

            ///
            /// \brief A joystick axis motion event was received.
            ///
            /// \param joystick The joystick index that caused the event.
            /// \param axis The axis index that caused the event.
            /// \param value The current axis value. The range is between
            ///              -32768 and 32767.
            ///
            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value) = 0;

            ///
            /// \brief A joystick button down event was received.
            ///
            /// \param joystick The joystick index that caused the event.
            /// \param button The button that was pressed.
            ///
            virtual void joyDown (uint8_t joystick, SDL_GameControllerButton button) = 0;

            ///
            /// \brief A joystick button was released.
            ///
            /// \param joystick The joysticj index that caused the event.
            /// \param button The button that was released.
            ///
            virtual void joyUp (uint8_t joystick, SDL_GameControllerButton button) = 0;

            ///
            /// \brief A key down event was received.
            ///
            /// \param key The symbol of the key that was pressed.
            ///            See SDLKey for possible values.
            ///
            virtual void keyDown (uint32_t key) = 0;

            ///
            /// \brief A key release event was received.
            ///
            /// \param key The symbol of the key that was released.
            ///            See SDLKey for possible values.
            ///
            virtual void keyUp (uint32_t key) = 0;

            ///
            /// \brief Loads the player's options.
            ///
            virtual void loadOptions (void) = 0;

            ///
            /// \brief Sets the player's grid.
            ///
            /// \param grid The grid that the player controlls. This
            ///             grid will be owned by the player and deleted
            ///             alsongside with the player.
            ///
            void setGrid (Grid *grid)
            {
                m_Grid.reset (grid);
            }

            ///
            /// \brief The player should update its logic.
            ///
            /// This function is called once per frame to give
            /// to the player the opportunity to update its own logic.
            /// At leats it updated its own grid logic.
            ///
            /// \param elapsedTime The time thay elapsed from the last
            ///                    call to this function.
            ///
            virtual void update (uint32_t elapsedTime)
            {
                Grid *grid = getGrid ();
                if ( 0 != grid )
                {
                    grid->update (elapsedTime);
                }
            }

        protected:
            ///
            /// \brief Gets the side the player is controlling.
            ///
            /// \return The player's side.
            ///
            IPlayer::PlayerSide getSide (void) const
            {
                return m_Side;
            }

        private:
            /// The player's grid.
            std::unique_ptr<Grid> m_Grid;
            /// The player's side.
            IPlayer::PlayerSide m_Side;
    };
}

#endif // !AMOEBAX_IPLAYER_H
