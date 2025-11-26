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
#if !defined (AMOEBAX_NORMAL_SETUP_STATE_H)
#define AMOEBAX_NORMAL_SETUP_STATE_H

#include <string>
#include "IState.h"
#include "Surface.h"
#include "Font.h"

namespace Amoebax
{
    ///
    /// \class NormalSetupState.
    /// \brief Normalt's set-up.
    ///
    class NormalSetupState: public IState
    {
        public:
            NormalSetupState ();

            virtual void activate (void);
            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value);
            virtual void joyDown (uint8_t joystick, SDL_GameControllerButton button);
            virtual void joyUp (uint8_t joystick, SDL_GameControllerButton button);
            virtual void keyDown (uint32_t key);
            virtual void keyUp (uint32_t key);
            virtual void redrawBackground (SDL_Rect *region, SDL_Surface *screen);
            virtual void render (SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void videoModeChanged (void);

        private:
            std::string getCharacterName (void) const;
            uint8_t getSelectedCharacter (void) const;
            void loadGraphicResources (void);
            void removeState (bool fadeOff = true);
            void selectCharacter (void);
            void selectNextCharacter (void);
            void selectPreviousCharacter (void);
            void setSelectedCharacter (uint8_t character);

            /// The background.
            std::unique_ptr<Surface> m_Background;
            /// The font when the option is not selected.
            std::unique_ptr<Font> m_FontNormal;
            /// The font when the option is selected.
            std::unique_ptr<Font> m_FontSelected;
            /// The currently selected character.
            uint8_t m_SelectedCharacter;
            /// The state was already removed.
            bool m_StateRemoved;
    };
}

#endif // !AMOEBAX_NORMAL_SETUP_STATE_H
