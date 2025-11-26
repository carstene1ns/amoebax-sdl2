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
#if !defined (AMOEBAX_TWO_PLAYERS_STATE_H)
#define AMOEBAX_TWO_PLAYERS_STATE_H

#include "IPlayer.h"
#include "IState.h"
#include "PairGenerator.h"

namespace Amoebax
{
    // Forward declarations.
    class Grid;
    class IMatchObserver;

    ///
    /// \class TwoPlayersState.
    /// \brief Two players playing at the same time.
    ///
    class TwoPlayersState: public IState
    {
        public:
            TwoPlayersState (IPlayer *leftPlayer, IPlayer *rightPlayer,
                             const std::string &backgroundFileName = "menuBackground.png",
                             uint32_t leftPlayerScore = 0,
                             uint32_t rightPlayerScore = 0,
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
            inline virtual bool shouldBePaused (void) { return true; }
            virtual void redrawBackground (SDL_Rect *region, SDL_Surface *screen);
            virtual void render (SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void videoModeChanged (void);

        private:
            /// The time to show the "Go!!" label in milliseconds.
            static const int32_t k_DefaultGoTime = 250;
            /// The actual amoebas' size when the screen scale factor is 1.0f.
            static const uint8_t k_MaxAmoebasSize = 64;
            /// The X position of the left grid's top-left corner.
            static const uint16_t k_PositionXLeftGrid = 55;
            /// The Y position of the left grid's top-left corner.
            static const uint16_t k_PositionYLeftGrid = 136;
            /// The X position of the left queue's top-left corner.
            static const uint16_t k_PositionXLeftQueue = 455;
            /// The Y position of the left queue's top-left corner.
            static const uint16_t k_PositionYLeftQueue = 140;
            /// The X position of the left score' top-left corner.
            static const uint16_t k_PositionXLeftScore = 461;
            /// The Y position of the left score' top-left corner.
            static const uint16_t k_PositionYLeftScore = 848;
            /// The X position of the left waiting ghost amoebas' top-left corner.
            static const uint16_t k_PositionXLeftWaiting = 55;
            /// The Y position of the left waiting ghost amoebas' top-left corner.
            static const uint16_t k_PositionYLeftWaiting = 56;
            /// The X position of the right grid's top-left corner.
            static const uint16_t k_PositionXRightGrid = 843;
            /// The Y position of the right grid's top-left corner.
            static const uint16_t k_PositionYRightGrid = 136;
            /// The X position of the right queue's top-left corner.
            static const uint16_t k_PositionXRightQueue = 761;
            /// The Y position of the right queue's top-left corner.
            static const uint16_t k_PositionYRightQueue = 140;
            /// The X position of the right score's top-right corner.
            static const uint16_t k_PositionXRightScore = 817;
            /// The Y position of the right score's top-right corner.
            static const uint16_t k_PositionYRightScore = 730;
            /// The X position of the right waiting ghost amoebas' top-left corner.
            static const uint16_t k_PositionXRightWaiting = 843;
            /// The Y position of the right waiting ghost amoebas' top-left corner.
            static const uint16_t k_PositionYRightWaiting = 55;

            void checkForRemoveStateKey (uint32_t key);
            bool gameIsOver (void) const;
            uint8_t getAmoebasSize (void) const;
            int32_t getGoTime (void) const;
            Grid *getLeftGrid (void) const;
            IPlayer *getLeftPlayer (void) const;
            int32_t getReadyTime (void) const;
            Grid *getRightGrid (void) const;
            IPlayer *getRightPlayer (void) const;
            uint8_t getSilhouetteBorder (void) const;
            IPlayer::PlayerSide getWinnerSide (void) const;
            void loadGraphicsResources (void);
            bool mustShowGoLabel (void) const;
            bool mustShowInitialLabels (void) const;
            bool mustShowReadyLabel (void) const;
            void setAmoebasSize (uint8_t size);
            void setGameOver (void);
            void setGoTime (int32_t time);
            void setReadyTime (int32_t time);
            void setSilhouetteBorder (uint8_t border);

            /// Amoebas's sprites.
            std::auto_ptr<Surface> m_Amoebas;
            /// The amoebas' size in pixels. Amoebas are squared.
            uint8_t m_AmoebasSize;
            /// Two players mode's background.
            std::auto_ptr<Surface> m_Background;
            /// The file name to use to get the background image.
            std::string m_BackgroundFileName;
            /// Background music.
            std::auto_ptr<Music> m_BackgroundMusic;
            /// The chain label image.
            std::auto_ptr<Surface> m_ChainLabel;
            /// Tells if the game is over.
            bool m_GameIsOver;
            /// The generator of amoebas pairs.
            std::auto_ptr<PairGenerator> m_Generator;
            /// The "Go!!" label.
            std::auto_ptr<Surface> m_Go;
            /// The time the "Go!!" label is displayed.
            int32_t m_GoTime;
            /// Left player.
            std::auto_ptr<IPlayer> m_LeftPlayer;
            /// Observer for the end of match.
            IMatchObserver *m_Observer;
            /// The "Ready?" label.
            std::auto_ptr<Surface> m_Ready;
            /// The time the "Ready?" label is displayed.
            int32_t m_ReadyTime;
            /// Right player.
            std::auto_ptr<IPlayer> m_RightPlayer;
            /// The score font.
            std::auto_ptr<Font> m_ScoreFont;
            /// The border size of the silhouettes.
            uint8_t m_SilhouetteBorder;
            /// The silhouettes image.
            std::auto_ptr<Surface> m_Silhouettes;
            /// Tells if the state was already removed.
            bool m_StateAlreadyRemoved;
            /// The loser's text.
            std::auto_ptr<Surface> m_YouLose;
            /// The winner's text.
            std::auto_ptr<Surface> m_YouWin;
            /// The winner's side.
            IPlayer::PlayerSide m_Winner;
    };
}

#endif // !AMOEBAX_TWO_PLAYERS_STATE_H
