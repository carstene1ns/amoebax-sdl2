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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
#if !defined (AMOEBAX_TWO_GP2X_PLAYERS_STATE_H)
#define AMOEBAX_TWO_GP2X_PLAYERS_STATE_H

#include <string>
#include "Grid.h"
#include "IPlayer.h"
#include "IState.h"
#include "PairGenerator.h"
#include "Font.h"

namespace Amoebax
{
    // Forward declarations.
    class IMatchObserver;

    ///
    /// \class TwoGP2XPlayersState.
    /// \brief Two human players playing at the same time on a single GP2X.
    ///
    class TwoGP2XPlayersState: public IState
    {
        public:
            explicit TwoGP2XPlayersState (
                                const std::string &backgroundFileName = "menuBackground.png",
                                 uint32_t leftPlayerScore = 0,
                                 uint32_t rightPlayerScore = 0,
                                 IMatchObserver *observer = 0);
            virtual ~TwoGP2XPlayersState (void);

            virtual void activate (void);
            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value);
            virtual void joyDown (uint8_t joystick, uint8_t button);
            virtual void joyUp (uint8_t joystick, uint8_t button);

            inline virtual bool shouldBePaused (void) { return true; }
            virtual void redrawBackground (SDL_Rect *region,
                                           SDL_Surface *screen);
            virtual void render (SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void videoModeChanged (void);

        private:
            /// The time to show the "Go!!" label in milliseconds.
            static const int32_t k_DefaultGoTime = 250;
            /// In this mode the amoebas' size is always 10x10 pixels.
            static const uint8_t k_MaxAmoebasSize = 10;
            /// The X position of the left grid's top-left corner.
            static const uint16_t k_PositionXLeftGrid = 128;
            /// The Y position of the left grid's top-left corner.
            static const uint16_t k_PositionYLeftGrid = 90;
            /// The X position of the left queue's top-left corner.
            static const uint16_t k_PositionXLeftQueue = 128;
            /// The Y position of the left queue's top-left corner.
            static const uint16_t k_PositionYLeftQueue = 154;
            /// The X position of the left score's top-left corner.
            static const uint16_t k_PositionXLeftScore = 7;
            /// The Y position of the left score's top-left corner.
            static const uint16_t k_PositionYLeftScore = 165;
            /// The X position of the left waiting ghost amoebas' top-left corner.
            static const uint16_t k_PositionXLeftWaiting = 146;
            /// The Y position of the left waiting ghost amoebas' top-left corner.
            static const uint16_t k_PositionYLeftWaiting = 90;
            /// The X position of the right grid's top-left corner.
            static const uint16_t k_PositionXRightGrid = 192;
            /// The Y position of the right grid's top-left corner.
            static const uint16_t k_PositionYRightGrid = 149;
            /// The X position of the right queue's top-left corner.
            static const uint16_t k_PositionXRightQueue = 192;
            /// The Y position of the right queue's top-left corner.
            static const uint16_t k_PositionYRightQueue = 88;
            /// The X position of the right screo's top-right corner.
            static const uint16_t k_PositionXRightScore = 310;
            /// The Y position of the right screo's top-right corner.
            static const uint16_t k_PositionYRightScore = 73;
            /// The X position of the right waiting ghost amoebas' top-left corner.
            static const uint16_t k_PositionXRightWaiting = 174;
            /// The Y position of the right waiting ghost amoebas' top-left corner.
            static const uint16_t k_PositionYRightWaiting = 149;

            bool gameIsOver (void) const;
            uint8_t getAmoebaSize (void) const;
            int32_t getGoTime (void) const;
            Grid *getLeftGrid (void);
            uint32_t getLeftPlayerScore (void);
            int32_t getReadyTime (void) const;
            Grid *getRightGrid (void);
            uint32_t getRightPlayerScore (void);
            uint8_t getSilhouetteBorder (void) const;
            IPlayer::PlayerSide getWinnerSide (void) const;
            void loadGraphicsResources (void);
            bool mustShowGoLabel (void) const;
            bool mustShowInitialLabels (void) const;
            bool mustShowReadyLabel (void) const;
            void setGameOver (void);
            void setGoTime (int32_t time);
            void setReadyTime (int32_t time);
            void setSilhouetteBorder (uint8_t border);

            /// Amoebas left's sprites.
            std::auto_ptr<Surface> m_AmoebasLeft;
            /// Amoebas right's sprites.
            std::auto_ptr<Surface> m_AmoebasRight;
            /// The amoebas' size in pixels. Amoebas are squared.
            uint8_t m_AmoebasSize;
            /// Two players mode's background.
            std::auto_ptr<Surface> m_Background;
            /// The file name of the background image to use.
            std::string m_BackgroundFileName;
            /// Background music.
            std::auto_ptr<Music> m_BackgroundMusic;
            /// Tells if the game is over.
            bool m_GameIsOver;
            /// The generator of amoebas pairs.
            std::auto_ptr<PairGenerator> m_Generator;
            /// The "Go!!" label.
            std::auto_ptr<Surface> m_Go;
            /// The time the "Go!!" label is displayed.
            int32_t m_GoTime;
            /// Left player's grid.
            std::auto_ptr<Grid> m_LeftGrid;
            /// The match observer to notify when the game is over.
            IMatchObserver *m_Observer;
            /// The "Ready?" label.
            std::auto_ptr<Surface> m_Ready;
            /// The time the "Ready?" label is displayed.
            int32_t m_ReadyTime;
            /// Right player's grid.
            std::auto_ptr<Grid> m_RightGrid;
            /// The font to be used for the player's score.
            std::auto_ptr<Font> m_ScoreFont;
            /// The border size of the silhouettes.
            uint8_t m_SilhouetteBorder;
            /// The silhouettes image for the left board.
            std::auto_ptr<Surface> m_SilhouettesLeft;
            /// The silhouettes image for the right board.
            std::auto_ptr<Surface> m_SilhouettesRight;
            /// Tells if the state was already removed.
            bool m_StateAlreadyRemoved;
            /// The winner's side.
            IPlayer::PlayerSide m_Winner;
            /// The Winner / Loser graphics.
            std::auto_ptr<Surface> m_WinnerLoser;
    };
}

#endif // !AMOEBAX_TWO_GP2X_PLAYERS_STATE_H
