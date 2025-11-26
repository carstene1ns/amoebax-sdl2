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
#if !defined (AMOEBAX_NORMAL_STATE_H)
#define AMOEBAX_NORMAL_STATE_H

#include <string>
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "IMatchObserver.h"
#include "IPlayer.h"
#include "IState.h"


namespace Amoebax
{
    ///
    /// \class NormalState.
    /// \brief The "normal mode" state.
    ///
    class NormalState: public IState, public IMatchObserver
    {
        public:
            NormalState (const std::string &characterName);

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
#endif // !IS_GP2X_HOST
            virtual void redrawBackground (SDL_Rect *region, SDL_Surface *screen);
            virtual void render (SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void videoModeChanged (void);

        private:
            void askTryAgain (void);
            const std::string &getCharacterName (void) const;
            uint8_t getCurrentOpponentLevel (void) const;
            uint32_t getCurrentScore (void) const;
            void incrementOpponentLevel (void);
            void loadGraphicsResources (void);
            bool mustAskTryAgain (void) const;

            /// The name of the player's character.
            std::string m_CharacterName;
            /// The current opponent's level.
            uint8_t m_CurrentOpponentLevel;
            /// The current human's score.
            uint32_t m_CurrentScore;
            /// The sound of when you lose.
            std::auto_ptr<Sound> m_SoundLose;
            /// The sound of when you win.
            std::auto_ptr<Sound> m_SoundWin;
            /// Tells if we need to ask the user to try again.
            bool m_TryAgain;
            /// The winner of a match.
            IPlayer::PlayerSide m_Winner;
    };
}

#endif // !AMOEBAX_NORMAL_STATE_H
