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
#if !defined (AMOEBAX_CREDITS_STATE_H)
#define AMOEBAX_CREDITS_STATE_H

#include "IState.h"
#include "System.h"

namespace Amoebax
{
    ///
    /// \class CreditsState.
    /// \brief The credits state.
    ///
    class CreditsState: public IState
    {
        public:
            CreditsState (void);

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
            void removeCreditsState (void);
            void loadGraphicResources (void);

            /// The menu's background.
            std::unique_ptr<Surface> m_Background;
            /// The font to use to draw the people's name.
            std::unique_ptr<Font> m_NamesFont;
            /// The font to use to draw the different development areas.
            std::unique_ptr<Font> m_SectionsFont;
            /// Tells if the state was already removed or not.
            bool m_StateRemoved;
    };
}

#endif // !AMOEABX_CREDITS_STATE_H
