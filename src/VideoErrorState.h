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
#if !defined (AMOEBAX_VIDEO_ERROR_STATE_H)
#define AMOEBAX_VIDEO_ERROR_STATE_H

#include "IState.h"
#include "Surface.h"

namespace Amoebax
{
    ///
    /// \class VideoErrorState
    /// \brief Shows an error message when the video mode couldn't be set.
    ///
    class VideoErrorState: public IState
    {
        public:
            VideoErrorState (IState *activeState);

            virtual void activate (void);
            virtual void joyMotion (uint8_t joystick, uint8_t axis, int16_t value);
            virtual void joyDown (uint8_t joystick, SDL_GameControllerButton button);
            virtual void joyUp (uint8_t joystick, SDL_GameControllerButton button);
            virtual void keyDown (uint32_t key);
            virtual void keyUp (uint32_t key);
            virtual void redrawBackground (SDL_Rect *region, SDL_Surface *screen);
            virtual void render (SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void videoModeChanged (void);

        private:
            void removeState (void);

            /// The state that is the currently active state, before the error.
            IState *m_ActiveState;
            /// The background image to show.
            std::unique_ptr<Surface> m_Background;
            /// Tells if the state was already removed.
            bool m_StateRemoved;
    };
}

#endif // !AMOEBAX_VIDEO_ERROR_STATE_H
