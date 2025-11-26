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
#if !defined (AMOEBAX_CONGRATULATIONS_STATE_H)
#define AMOEBAX_CONGRATULATIONS_STATE_H

#include <string>
#include "IState.h"
#include "Surface.h"

namespace Amoebax
{
    ///
    /// \class CongratulationsState.
    /// \brief The congratulations screen.
    ///
    class CongratulationsState: public IState
    {
        public:
            CongratulationsState (const std::string &playerName,
                                  uint32_t playerScore);

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
            void endCongratulationsState (void);
            const std::string &getPlayerName (void) const;
            uint32_t getPlayerScore (void) const;
            void loadGraphicResources (void);
            bool mustEnd (void) const;

            /// The background image.
            std::auto_ptr<Surface> m_Background;
            /// Background music.
            std::auto_ptr<Music> m_BackgroundMusic;
            /// Tells if end the state or not.
            bool m_End;
            /// The player's name to congratulate.
            std::string m_PlayerName;
            /// The player's score.
            uint32_t m_PlayerScore;
    };
}


#endif  // !AMOEBAX_CONGRATULATIONS_STATE_H
