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
#if !defined (AMOEBAX_TOURNAMENT_MENU_STATE_H)
#define AMOEBAX_TOURNAMENT_MENU_STATE_H

#include <vector>
#include "Font.h"
#include "IState.h"
#include "Surface.h"

namespace Amoebax
{
    ///
    /// \class TournamentMenuState.
    /// \brief Prompts the user how many players will play.
    ///
    class TournamentMenuState: public IState
    {
        public:
            TournamentMenuState (void);

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
            void activateMenuOption (void);
            void loadGraphicResources (void);
            void selectNextMenuOption (void);
            void selectPreviousMenuOption (void);

            /// Background image.
            std::auto_ptr<Surface> m_Background;
            /// Menut's font.
            std::auto_ptr<Font> m_Font;
            /// Menu's font for selected items.
            std::auto_ptr<Font> m_FontSelected;
            /// The number of players.
            std::vector<uint8_t> m_Players;
            /// The current selected players option.
            std::vector<uint8_t>::iterator m_SelectedOption;
    };
}

#endif // !AMOEBAX_TOURNAMENT_MENU_STATE_H
