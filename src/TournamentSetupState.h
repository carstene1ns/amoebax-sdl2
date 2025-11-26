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
#if !defined (AMOEBAX_TOURNAMENT_SETUP_STATE_H)
#define AMOEBAX_TOURNAMENT_SETUP_STATE_H

#include <string>
#include <vector>
#include "IState.h"
#include "Surface.h"
#include "Font.h"

namespace Amoebax
{
    ///
    /// \class TournamentSetupState.
    /// \brief Tournament's set-up.
    ///
    class TournamentSetupState: public IState
    {
        public:
            TournamentSetupState (uint8_t players);

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
            ///
            /// \struct CharacterInfo
            /// \brief Information about a character.
            ///
            struct CharacterInfo
            {
                /// The character's name,
                std::string name;
                /// The character's avatar image.
                std::auto_ptr<Surface> image;
            };

            ///
            /// \struct SelectedCharacter
            /// \brief Information about a selected character.
            ///
            struct SelectedCharacter
            {
                /// The characters grid's column.
                uint8_t col;
                /// The character index.
                uint8_t index;
                /// The characters grid's row.
                uint8_t row;
            };

            bool allCharactersSelected (void) const;
            void deselectCharacter (void);
            uint8_t getCurrentPlayer (void) const;
            uint8_t getNumPlayers (void) const;
            uint8_t getSelectedCol (void) const;
            const CharacterInfo &getSelectedCharacter (void) const;
            uint8_t getSelectedCharacterIndex (void) const;
            uint8_t getSelectedRow (void) const;
            int32_t getTimeToWait (void) const;
            uint16_t getXPositionOfCol (uint8_t col) const;
            uint16_t getYPositionOfRow (uint8_t row) const;
            bool isSelectedPositionValid (void) const;
            void loadGraphicResources (void);
            void selectCharacter (void);
            void selectFirstCharacter (void);
            void selectNextCol (void);
            void selectNextRow (void);
            void selectPreviousCol (void);
            void selectPreviousRow (void);
            void setNextPlayer (void);
            void setPreviousPlayer (void);
            void setSelectedCol (uint8_t col);
            void setSelectedRow (uint8_t row);
            void setTimeToWait (int32_t timeToWait);

            /// The background.
            std::auto_ptr<Surface> m_Background;
            /// The player that is selecting a character.
            uint8_t m_CurrentPlayer;
            /// The font used to draw all texts.
            std::auto_ptr<Font> m_Font;
            /// The number of players.
            uint8_t m_NumPlayers;
            /// The faces' information.
            CharacterInfo m_Characters[14];
            /// The selected characters by players.
            std::vector<SelectedCharacter> m_SelectedCharacters;
            /// The currently selected column.
            uint8_t m_SelectedCol;
            /// The currently selected row.
            uint8_t m_SelectedRow;
            /// The current selection.
            std::auto_ptr<Surface> m_Selection;
            /// The time to wait before changing to the tournament state.
            int32_t m_TimeToWait;
    };
}

#endif // !AMOEBAX_TOURNAMENT_SETUP_STATE_H
