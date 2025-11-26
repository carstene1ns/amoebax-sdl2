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
#if !defined (AMOEBAX_TRAINING_STATE_H)
#define AMOEBAX_TRAINING_STATE

#include "IPlayer.h"
#include "IState.h"
#include "PairGenerator.h"

namespace Amoebax
{
    ///
    /// \class TrainingState.
    /// \brief The "training mode" state.
    ///
    class TrainingState: public IState
    {
        public:
            TrainingState (void);

            virtual void activate (void);
            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value);
            virtual void joyDown (uint8_t joystick, SDL_GameControllerButton button);
            virtual void joyUp (uint8_t joystick, SDL_GameControllerButton button);
            virtual void keyDown (uint32_t key);
            virtual void keyUp (uint32_t key);
            inline virtual bool shouldBePaused (void) { return true; }
            virtual void redrawBackground (SDL_Rect *region, SDL_Surface *screen);
            virtual void render (SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void videoModeChanged (void);

        private:
            /// The actual amoebas' size when the screen scale factor is 1.0f.
            static const uint8_t k_MaxAmoebasSize = 64;
            /// The time the "Level Up!" label is shown (in ms.)
            static const uint32_t k_LevelUpTime = 1000;
            /// The X position of the grid's top-left corner.
            static const uint16_t k_PositionXGrid = 433;
            /// The Y position of the grid's top-left corner.
            static const uint16_t k_PositionYGrid = 138;
            /// The X position of the current level (right aligned.)
            static const uint16_t k_PositionXLevel = 1105;
            /// The Y position of the current level (right aligned.)
            static const uint16_t k_PositionYLevel = 846;
            /// The X position of the queue's top-left corner.
            static const uint16_t k_PositionXQueue = 844;
            /// The Y position of the queue's top-left corner.
            static const uint16_t k_PositionYQueue = 138;
            /// The X position of the current score (right aligned.)
            static const uint16_t k_PositionXScore = 1105;
            /// The Y position of the current score (right aligned.)
            static const uint16_t k_PositionYScore = 730;
            /// The X position of the waiting ghost amoebas top-left corner.
            static const uint16_t k_PositionXWaiting = 433;
            /// The Y position of the waiting ghost amoebas top-left corner.
            static const uint16_t k_PositionYWaiting = 53;

            void addGhostAmoebas (void);
            void checkLevelChange (void);
            uint8_t getAmoebasSize (void) const;
            uint16_t getCurrentLevel (void) const;
            int32_t getGoTime (void) const;
            IPlayer *getPlayer (void) const;
            Grid *getPlayerGrid (void) const;
            int32_t getReadyTime (void) const;
            uint8_t getSilhouetteBorder (void) const;
            void incrementCurrentLevel (uint8_t amount = 1);
            void loadGraphicResources (void);
            bool mustShowGoLabel (void) const;
            bool mustShowInitialLabels (void) const;
            bool mustShowReadyLabel (void) const;
            void pauseOrEndGame (void);
            void setAmoebasSize (uint8_t size);
            void setCurrentLevel (uint8_t level);
            void setGoTime (int32_t time);
            void setReadyTime (int32_t time);
            void setSilhouetteBorder (uint8_t border);

            /// Amoebas sprites.
            std::unique_ptr<Surface> m_Amoebas;
            /// The amoebas' size in pixels. Amoebas are squared.
            uint8_t m_AmoebasSize;
            /// Training background
            std::unique_ptr<Surface> m_Background;
            /// Background music.
            std::unique_ptr<Music> m_BackgroundMusic;
            /// The chain label image.
            std::unique_ptr<Surface> m_ChainLabel;
            /// The current level.
            uint8_t m_CurrentLevel;
            /// The game over text.
            std::unique_ptr<Surface> m_GameOver;
            /// The generator of amoeba pairs.
            std::unique_ptr<PairGenerator> m_Generator;
            /// The "Go!!" label.
            std::unique_ptr<Surface> m_Go;
            /// The time the "Go!!" label is displayed.
            int32_t m_GoTime;
            /// The "Level Up!" label.
            std::unique_ptr<Surface> m_LevelUp;
            /// The vertical possition of the "Level Up!" label.
            float m_LevelUpPosition;
            /// The "Level Up!" sound.
            std::unique_ptr<Sound> m_LevelUpSound;
            /// The time to show the "Level Up!" label.
            int32_t m_LevelUpTime;
            /// The only player.
            std::unique_ptr<IPlayer> m_Player;
            /// The "Ready?" label.
            std::unique_ptr<Surface> m_Ready;
            /// The time the "Ready?" label is displayed.
            int32_t m_ReadyTime;
            /// The score and level's font.
            std::unique_ptr<Font> m_ScoreFont;
            /// The border size of the silhouettes.
            uint8_t m_SilhouetteBorder;
            /// The silhouettes image.
            std::unique_ptr<Surface> m_Silhouettes;
            /// The sound of when you lose.
            std::unique_ptr<Sound> m_SoundLose;
    };
}

#endif // !AMOEBAX_TRAINING_STATE
