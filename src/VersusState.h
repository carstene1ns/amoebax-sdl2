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
#if !defined (AMOEBAX_VERSUS_STATE_H)
#define AMOEBAX_VERSUS_STATE_H

#include "IState.h"
#include "Surface.h"

namespace Amoebax
{
    ///
    /// \class VersusState
    /// \brief The versus screen.
    ///
    class VersusState: public IState
    {
        public:
            VersusState (const std::string &player,
                         const std::string &opponent);

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
            void endVersusState (void);
            const std::string &getPlayerName (void) const;
            const std::string &getOpponentName (void) const;
            void loadGraphicResources (void);
            bool mustEnd (void) const;

            /// The background image.
            std::unique_ptr<Surface> m_Background;
            /// The background music.
            std::unique_ptr<Music> m_BackgroundMusic;
            /// The versus state must end.
            bool m_End;
            /// The opponent's name.
            std::string m_Opponent;
            /// The player's avatar name.
            std::string m_Player;
            /// The time elapsed since the start of the versus mode.
            uint32_t m_TimeElapsed;
    };
}


#endif  // !AMOEBAX_VERSUS_STATE_H



