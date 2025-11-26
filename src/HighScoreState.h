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

#if !defined (AMOEBAX_HIGH_SCORE_STATE_H)
#define AMOEBAX_HIGH_SCORE_STATE_H

#include "IState.h"

namespace Amoebax
{
    ///
    /// \class HighScoreState.
    /// \brief Shows the high scores list.
    ///
    class HighScoreState: public IState
    {
        public:
            explicit HighScoreState (int32_t scoreToHighLigh = -1);

            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value);
            virtual void joyDown (uint8_t joystick, uint8_t button);
            virtual void joyUp (uint8_t joystick, uint8_t button);

#if !defined (IS_GP2X_HOST)
            virtual void keyDown (uint32_t key);
            virtual void keyUp (uint32_t key);
#endif // !IS_GP2X_HOST

            virtual void redrawBackground (SDL_Rect *region,
                                           SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void render (SDL_Surface *screen);
            virtual void videoModeChanged (void);

        private:
            /// The vertical position to start writting the scores.
            static const uint16_t k_InitialYPosition = 300;
            /// The horizontal position to write left-aligned the scores' names.
            static const uint16_t k_NamePosition = 200;
            /// The horizontal position to write right-aligned the actual scores.
            static const uint16_t k_ScorePosition = 1080;

            int32_t getScoreToHighLight (void) const;
            void loadGraphicResources (void);
            void removeHighScoreState (void);

            /// The background.
            std::auto_ptr<Surface> m_Background;
            /// The font to use for regular scores.
            std::auto_ptr<Font> m_Font;
            /// The font to use to highlight an score.
            std::auto_ptr<Font> m_FontHighLight;
            /// The score to high light.
            int32_t m_ScoreToHighLight;
            /// Tells if the state was already removed from the system.
            bool m_StateRemoved;
    };
}

#endif
