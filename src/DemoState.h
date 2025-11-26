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
#if !defined (AMOEBAX_DEMO_STATE_H)
#define AMOEBAX_DEMO_STATE_H

#include "IMatchObserver.h"
#include "IState.h"
#include "TwoPlayersState.h"

namespace Amoebax
{
    ///
    /// \class DemoState
    /// \brief Shows a match between two computer players for some time.
    ///
    class DemoState: public IState, public IMatchObserver
    {
        public:
            DemoState (void);

            virtual void activate (void);
            virtual void endOfMatch (IPlayer::PlayerSide winner,
                                     uint32_t leftPlayerScore,
                                     uint32_t rightPlayerScore);
            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value);
            virtual void joyDown (uint8_t joystick, uint8_t button);
            virtual void joyUp (uint8_t joystick, uint8_t button);
#if !defined (IS_GP2X_HOST)
            virtual void keyDown (uint32_t key);
            virtual void keyUp (uint32_t key);
#endif // IS_GP2X_HOST
            virtual void redrawBackground (SDL_Rect *region, SDL_Surface *screen);
            virtual void render (SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void videoModeChanged (void);

        private:
            /// The time the demo lasts in ms.
            static const int32_t k_DemoTime = 30000;

            int32_t getDemoTime (void) const;
            void removeState (void);
            void setDemoTime (int32_t time);

            /// The time the demo has to run.
            int32_t m_DemoTime;
            /// The two players mode that will show the match.
            std::auto_ptr<TwoPlayersState> m_Match;
            /// Tells if the state was already removed.
            bool m_StateRemoved;
    };
}

#endif // !AMOEBAX_DEMO_STATE_H
