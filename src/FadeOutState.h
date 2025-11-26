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
#if !defined (AMOEBAX_FADE_OUT_STATE_H)
#define AMOEBAX_FADE_OUT_STATE_H

#include "IState.h"
#include "Surface.h"

namespace Amoebax
{
    ///
    /// \class FadeOutState
    /// \brief Fades the screen from its contents to black.
    ///
    class FadeOutState: public IState
    {
        public:
            explicit FadeOutState (uint32_t fadeTime = 300);

            virtual void activate (void);
            virtual void joyMotion (uint8_t joystick, uint8_t axis, int16_t value) { }
            virtual void joyDown (uint8_t joystick, uint8_t button) { }
            virtual void joyUp (uint8_t joystick, uint8_t button) { }
#if !defined (IS_GP2X_HOST)
            virtual void keyDown (uint32_t key) { }
            virtual void keyUp (uint32_t key) { }
#endif // !IS_GP2X_HOST
            virtual void redrawBackground (SDL_Rect *region, SDL_Surface *screen);
            virtual void render (SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void videoModeChanged (void);

        private:
            int16_t getAlpha (void) const;
            uint32_t getFadeTime (void) const;
            void setAlpha (int16_t alpha);

            /// The current alpha value.
            int16_t m_Alpha;
            /// The background image to fade.
            std::auto_ptr<Surface> m_Background;
            /// The time the fade should take.
            uint32_t m_FadeTime;
    };
}

#endif // AMOEBAX_FADE_OUT_STATE_H
