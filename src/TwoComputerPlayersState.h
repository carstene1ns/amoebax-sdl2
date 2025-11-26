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
#if !defined (AMOEBAX_TWO_COMPUTER_PLAYERS_STATE_H)
#define AMOEBAX_TWO_COMPUTER_PLAYERS_STATE_H

#include <string>
#include "IPlayer.h"
#include "IState.h"

namespace Amoebax
{
    // Forward declarations.
    class IMatchObserver;

    ///
    /// \class TwoComputerPlayersState.
    /// \brief Two players playing at the same time.
    ///
    class TwoComputerPlayersState: public IState
    {
        public:
            TwoComputerPlayersState (uint8_t leftPlayerLevel,
                                     uint8_t rightPlayerLevel,
                                     IMatchObserver *observer = 0);

            virtual void activate (void);
            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value);
            virtual void joyDown (uint8_t joystick, uint8_t button);
            virtual void joyUp (uint8_t joystick, uint8_t button);
#if !defined (IS_GP2X_HOST)
            virtual void keyDown (uint32_t key);
            virtual void keyUp (uint32_t key);
#endif // !IS_GP2X_HOST
            virtual void redrawBackground (SDL_Rect *region, SDL_Surface *screen);
            virtual void render (SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void videoModeChanged (void);

        private:
            void checkForRemoveStateKey (uint32_t key);
            IMatchObserver *getObserver (void) const;
            std::string getWinnerName (void) const;
            IPlayer::PlayerSide getWinnerSide (void) const;
            bool isObserverNotified (void) const;
            void loadGraphicsResources (void);
            void setObserverNotified (void);

            /// Two players mode's background.
            std::auto_ptr<Surface> m_Background;
            /// The match observer to notify when the match is over.
            IMatchObserver *m_Observer;
            /// Tells if the observer is already notified or not.
            bool m_ObserverNotified;
            /// Tells if the state was already removed.
            bool m_StateAlreadyRemoved;
            /// The side of the winner.
            IPlayer::PlayerSide m_WinnerSide;
            /// The name of the winner.
            std::string m_WinnerName;
    };
}

#endif // !AMOEBAX_TWO_COMPUTER_PLAYERS_STATE_H
