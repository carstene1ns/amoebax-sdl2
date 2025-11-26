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
#if !defined (AMOEBAX_NEW_HIGH_SCORE_STATE_H)
#define AMOEBAX_NEW_HIGH_SCORE_STATE_H

#include "IState.h"

namespace Amoebax
{
    ///
    /// \class NewHighScoreState.
    /// \brief Prompts the player for her name.
    ///
    class NewHighScoreState: public IState
    {
        public:
            explicit NewHighScoreState (uint32_t score);
            virtual ~NewHighScoreState (void);

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
            /// The time required change cursor's visibility (in ms.)
            static const uint32_t k_CursorVisibleTime = 300;
            /// The Y screen position to write the new high score.
            static const uint16_t k_ScorePosition = 300;
            /// The maximum length in characters that the name can have.
            static const std::string::size_type k_NameMaxLength = 15;
            /// The Y screen position to write the name.
            static const uint16_t k_NamePosition = 615;

            void acceptName (void);
            void addNewCharacter (void);
            bool isCursorVisible (void) const;
            const std::string getCursorValue (void) const;
            int32_t getCursorVisibleTime (void) const;
            uint32_t getScore (void) const;
            void loadGraphicResources (void);
            void removePreviousCharacter ();
            void resetCursorVisibility (void);
            void selectNextCursorValue (void);
            void selectPreviousCursorValue (void);
            void setCursorVisible (bool visible);
            void setCursorVisibleTime (int32_t time);

            /// The background image.
            std::unique_ptr<Surface> m_Background;
            /// Background music.
            std::unique_ptr<Music> m_BackgroundMusic;
            /// The current index of m_CursorValues.
            uint8_t m_CursorValueIndex;
            /// The possible values that the cursor can take.
            const std::string m_CursorValues;
            /// Tells if the cursor is visible.
            bool m_CursorVisible;
            /// The time the cursor should remain visible or invisible.
            int32_t m_CursorVisibleTime;
            /// The font to use to highlight.
            std::unique_ptr<Font> m_HighLightFont;
            /// The name to set as the high score.
            std::string m_Name;
            /// The font to use to draw the currently entered name.
            std::unique_ptr<Font> m_NameFont;
            /// The score to save.
            uint32_t m_Score;
    };
}

#endif // !AMOEBAX_NEW_HIGH_SCORE_STATE_H
