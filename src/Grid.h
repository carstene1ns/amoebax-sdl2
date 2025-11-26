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
#if !defined (AMOEBAX_GRID_H)
#define AMOEBAX_GRID_H

#include <list>
#include <memory>
#include <set>
#include <stdint.h>
#include <vector>
#include "Amoeba.h"
#include "Sound.h"

namespace Amoebax
{
    // Forward declarations.
    class Amoeba;
    class ChainLabel;
    class GridStatus;
    class PairGenerator;

    ///
    /// \class Grid
    /// \brief An amoebas' grid for a single player.
    ///
    class Grid
    {
        public:
            /// The first height (starting from above) that an amoeba is
            /// visible.
            static const uint16_t k_FirstVisibleHeight = 3;
            /// The grid's height in amoebas.
            static const uint16_t k_GridHeight = 12 + k_FirstVisibleHeight;
            /// The grid's width in amoebas.
            static const uint16_t k_GridWidth = 6;
            /// The maximum number of silhouette frames.
            static const uint16_t k_MaxSilhouetteFrames = 5;
            /// The height that is visible to the user.
            static const uint16_t k_VisibleHeight = k_GridHeight -
                                                    k_FirstVisibleHeight;

            ///
            /// \brief The grid's orientation.
            ///
            enum Layout
            {
                /// The grid is horizontal.
                LayoutHorizontal,
                /// The grid is vertical.
                LayoutVertical
            };

            ///
            /// \brief The grid's side of the queue.
            ///
            enum QueueSide
            {
                /// The queue is at grid's left.
                QueueSideLeft,
                /// The queue is at grid's right.
                QueueSideRight
            };

            ///
            /// \struct FallingAmoeba
            /// \brief An amoeba that is floating in the grid.
            ///
            /// A falling amoeba is just an amoeba that lost its support
            /// amoeba (i.e., the amoeba just below it died) and now must
            /// fall. Both amoebas in the FallingPair are also falling amoebas.
            ///
            struct FallingAmoeba
            {
                /// The falling amoeba.
                Amoeba *amoeba;
                /// The amoeba's X position in the grid.
                int16_t x;
                /// The amoeba's Y position in the grid.
                int16_t y;
            };

            Grid (uint16_t gridPositionX, uint16_t gridPositionY,
                  uint16_t queuePositionX, uint16_t queuePositionY,
                  uint16_t waitingGhostPositionX,
                  uint16_t waitingGhostPositionY, uint16_t amoebaSize,
                  QueueSide queueSide, uint32_t score = 0,
                  Layout layout = LayoutVertical);
            ~Grid (void);

            void addNewPair (Amoeba *main, Amoeba *satellite);
            const std::list<Amoeba *> &getActiveAmoebas (void) const;
            const std::list<ChainLabel *> &getChainLabels (void) const;
            uint16_t getGridPositionX (void) const;
            uint16_t getGridPositionY (void) const;
            FallingAmoeba getFallingMainAmoeba (void) const;
            FallingAmoeba getFallingSatelliteAmoeba (void) const;
            FallingAmoeba getFollowingFallingMainAmoeba (void) const;
            FallingAmoeba getFollowingFallingSatelliteAmoeba (void) const;
            FallingAmoeba getNextFallingMainAmoeba (void) const;
            FallingAmoeba getNextFallingSatelliteAmoeba (void) const;
            uint8_t getOpponentGhostAmoebas (void) const;
            const std::list<Amoeba *> &getQueuedAmoebas (void) const;
            uint16_t getQueuePositionX (void) const;
            uint16_t getQueuePositionY (void) const;
            uint32_t getScore (void) const;
            int8_t getSilhouetteFrame (void) const;
            GridStatus getState (void) const;
            uint16_t getWaitingGhostPositionX (void) const;
            uint16_t getWaitingGhostPositionY (void) const;
            const std::vector<Amoeba *> &getWaitingGhostAmoebas (void) const;
            bool hasNewFallingPair (void) const;
            void incrementNumberOfWaitingGhosts (uint8_t amount = 1);
            bool isFilled (void) const;
            bool isQueueMoving (void) const;
            void moveLeft (void);
            void moveRight (void);
            void rotateClockwise (void);
            void rotateCounterClockwise (void);
            void update (uint32_t elapsedTime);
            void updateQueue (uint32_t elapsedTime);
            void scheduleFallingPair (void);
            void setGenerator (PairGenerator *generator);
            void setMaxFallingSpeed (void);
            void setNormalFallingSpeed (void);

        private:
            /// Default blinking time (ms).
            static const int32_t k_DefaultBlinkTime = 25;
            /// Default dying time (ms).
            static const int32_t k_DefaultDyingTime = 250;
            /// Default falling time (ms).
            static const int32_t k_DefaultFallingTime = 500;
            /// The time required for the silhouette to be shown (ms.)
            static const int32_t k_SilhouetteTime = 100;

            ///
            /// \struct FallingPair
            /// \brief A pair of amoebas falling and controlled by a player.
            ///
            /// This pair of the amoabes are the ones the player (either
            /// a human player or an A.I. player) controls so can move
            /// and rotate them around.
            ///
            struct FallingPair
            {
                /// The main falling amoeba.
                FallingAmoeba main;
                /// The amoeba that can be rotated around the main amoeba.
                FallingAmoeba satellite;
                /// The time remaining before the pair falls an step.
                int32_t fallingTime;
                /// The offset of the amoebas in their respective grid position.
                int16_t verticalOffset;
            };

            void addChainLabel (uint8_t stepChain, int16_t x, int16_t y);
            void clearDyingAmoebas (void);
            void findFloatingAmoebas (void);
            Amoeba *getAmoebaAt (int16_t x, int16_t y) const;
            uint16_t getAmoebaSize (void) const;
            uint8_t getCurrentStepChain (void) const;
            int16_t getCurrentRotationDegree (void) const;
            Layout getLayout (void) const;
            uint8_t getNumberOfWaitingGhosts (void) const;
            void getNextPair (void);
            QueueSide getQueueSide (void) const;
            void incrementCurrentStepChain (uint8_t amount = 1);
            void incrementScore (uint16_t amount = 1);
            bool isOfSameColour (int16_t x, int16_t y, Amoeba *amoeba) const;
            bool isSatelliteRotating (void) const;
            void makeGhostsFall (void);
            void makeGroup (std::vector<FallingAmoeba> &group,
                            std::vector<FallingAmoeba> &ghosts,
                            int16_t x, int16_t y,
                            std::set<std::pair<int16_t, int16_t> > &visitedPositions,
                            Amoeba::Colour colour = Amoeba::ColourNone,
                            bool initialAmoeba = true);
            void makeChain (void);
            void markAsFilled (void);
            void setAmoebaAt (int16_t x, int16_t y, Amoeba *amoeba);
            void setAmoebaStateAt (int16_t x, int16_t y, bool neighbors = true);
            void setCurrentStepChain (uint8_t stepChain);
            void setFallingAmoebaScreenPosition (FallingAmoeba &fallingAmoeba,
                                                 int16_t verticalOffset,
                                                 bool rotating);
            void setHasNewFallingPair (bool hasNewFallingPair);
            void setNumberOfWaitingGhosts (uint8_t number);
            void setOpponentGhostAmoebas (uint8_t ghostAmoebas);
            void setScore (uint32_t score);
            void setSilhouetteFrame (int8_t frame);
            void setupFallingPair (void);
            uint8_t removeGhostAmoebas (uint8_t newGhostAmoebas);
            void rotateSatelliteAmoeba (float degreesToRotate);
            void updateWaitingGhosts (void);

            /// The list of amoebas that are in the grid, not waiting.
            std::list<Amoeba *> m_ActiveAmoebas;
            /// The size (width and height are the same) of a single amoeba.
            uint16_t m_AmoebaSize;
            /// The time need to switch the blink of dying amoebas.
            int32_t m_BlinkTime;
            /// The list of chain labels.
            std::list<ChainLabel *> m_ChainLabels;
            /// The current chain step.
            uint8_t m_CurrentStepChain;
            /// The current satellite amoeba's rotation degrees.
            float m_CurrentRotationDegrees;
            /// The remaing degrees until the rotation is done.
            float m_DegreesUntilRotationDone;
            /// Dying sound.
            std::auto_ptr<Sound> m_DieSound;
            /// Dying amoebas.
            std::vector<FallingAmoeba> m_DyingAmoebas;
            /// Dying time.
            int32_t m_DyingTime;
            /// The falling pair
            FallingPair m_FallingPair;
            /// The grid is filled (i.e., the player lost.)
            bool m_Filled;
            /// Tells that we are just making fall the first pair.
            bool m_FirstFallingPair;
            /// The amoebas that have lost they lowe neighbor and are floating.
            std::list<FallingAmoeba> m_FloatingAmoebas;
            /// The verical offset of all floating amoebas.
            int16_t m_FloatingAmoebasVerticalOffset;
            /// This grid's amoebas pair generator.
            PairGenerator *m_Generator;
            /// The actual amoeba's grid.
            std::vector<Amoeba *> m_Grid;
            /// The X position of the top-left corner of the grid.
            uint16_t m_GridPositionX;
            /// The Y position of the top-left corner of the grid.
            uint16_t m_GridPositionY;
            /// Tells the if falling pair is new in this update or not.
            bool m_HasNewFallingPair;
            /// The list of amoebas that are waiting to be in play.
            std::list<Amoeba *> m_InactiveAmoebas;
            /// The grid's layout.
            Layout m_Layout;
            /// Tells if we are falling at max speed.
            bool m_MaxFallingSpeed;
            /// The number of ghost amoebas to send to the opponent.
            uint8_t m_OpponentGhostAmoebas;
            /// The queue, amoebas waiting to fall.
            std::list<FallingPair> m_Queue;
            /// The list of pointer to the amoebas in the queue.
            std::list<Amoeba *> m_QueuedAmoebas;
            /// Tells if the queue is moving.
            bool m_QueueIsMoving;
            /// The side of the queue.
            QueueSide m_QueueSide;
            /// The X position of the top-left corner of the wait queue.
            uint16_t m_QueuePositionX;
            /// The Y position of the top-left corner of the wait queue.
            uint16_t m_QueuePositionY;
            /// The remaining horizontal offset to move the queued amoebas.
            uint16_t m_RemainingQueueHorizontalOffset;
            /// The remaining vertical offset to move the queued amoebas.
            uint16_t m_RemainingQueueVerticalOffset;
            /// The direction (clockwise or anti-clockwise) of the satellite rotation.
            float m_RotationDirection;
            /// The current player's score.
            uint32_t m_Score;
            /// The current silhouette frame.
            int8_t m_SilhouetteFrame;
            /// The silhouette frame direction, up or down.
            int8_t m_SilhouetteFrameDirection;
            /// The time required for the silhouette to change.
            int32_t m_SilhouetteTime;
            /// Tell if we are waiting for the initial 3 pairs.
            bool m_WaitingInitialAmoebas;
            /// The ghost amoebas waiting to fall to the grid.
            std::vector<Amoeba *> m_WaitingGhostAmoebas;
            /// The number of ghost amoebas waiting. Max. is 30.
            uint8_t m_WaitingGhostAmoebasNumber;
            /// The X position of where the ghost amoebas wait.
            uint16_t m_WaitingGhostPositionX;
            /// The Y position of where the ghost amoebas wait.
            uint16_t m_WaitingGhostPositionY;
    };

    ///
    /// \brief Gets the list of the active amoebas.
    ///
    /// The active amoebas are those amoebas that are currently
    /// on the game field. They are the alredy fallen amoebas and the
    /// falling pair. The two pairs waiting to fall are not active, are
    /// queued. \see getQueuedAmoebas() for more information.
    ///
    /// \return The list of amoebas that are active.
    ///
    inline const std::list<Amoeba *> &
    Grid::getActiveAmoebas (void) const
    {
        return m_ActiveAmoebas;
    }

    ///
    /// \brief Gets the amoeba's size.
    ///
    /// \return The size (both height and width, as they are equal) of the
    ///         amoebas.
    ///
    inline uint16_t
    Grid::getAmoebaSize (void) const
    {
        return m_AmoebaSize;
    }

    ///
    /// \brief Gets the list of chain labels.
    ///
    /// \return The list of chain labels to show to the user.
    ///
    inline const std::list<ChainLabel *> &
    Grid::getChainLabels (void) const
    {
        return m_ChainLabels;
    }

    ///
    /// \brief Gets the current step chain.
    ///
    /// \return The current step chain value, starting from 0.
    ///
    inline uint8_t
    Grid::getCurrentStepChain (void) const
    {
        return m_CurrentStepChain;
    }

    ///
    /// \brief Gets the grid's top-left corner X position.
    ///
    /// \return The X position of grid's top-left corner.
    ///
    inline uint16_t
    Grid::getGridPositionX (void) const
    {
        return m_GridPositionX;
    }

    ///
    /// \brief Gets the grid's top-left corner Y position.
    ///
    /// \return The Y position of grid's top-left corner.
    ///
    inline uint16_t
    Grid::getGridPositionY (void) const
    {
        return m_GridPositionY;
    }

    ///
    /// \brief Gets the layout.
    ///
    /// \return the current grid layout.
    ///
    inline Grid::Layout
    Grid::getLayout (void) const
    {
        return m_Layout;
    }

    ///
    /// \brief Gets the number of waiting ghosts.
    ///
    /// \return The number of ghosts amoebas that are waiting to fall into
    ///         the grid.
    ///
    inline uint8_t
    Grid::getNumberOfWaitingGhosts (void) const
    {
        return m_WaitingGhostAmoebasNumber;
    }

    ///
    /// \brief Gets the number of ghost amoebas to send to the opponent.
    ///
    /// \return The number of ghost amoebas for the opponent.
    ///
    inline uint8_t
    Grid::getOpponentGhostAmoebas (void) const
    {
        return m_OpponentGhostAmoebas;
    }

    ///
    /// \brief Gets the list of the queued amoebas.
    ///
    /// The active amoebas are the two pairs of amoebas waiting to fall.
    ///
    /// \return The list of amoebas that are active.
    ///
    inline const std::list<Amoeba *> &
    Grid::getQueuedAmoebas (void) const
    {
        return m_QueuedAmoebas;
    }


    ///
    /// \brief Gets the current player's score.
    ///
    /// \return The current player's score.
    ///
    inline uint32_t
    Grid::getScore (void) const
    {
        return m_Score;
    }

    ///
    /// \brief Gets the current sihouette frame.
    ///
    /// \return The current silhouette frame.
    ///
    inline int8_t
    Grid::getSilhouetteFrame (void) const
    {
        return m_SilhouetteFrame;
    }

    ///
    /// \brief Gets the list of the waiting ghost amoebas.
    ///
    /// The list of waiting amoebas is just 6 amoebas, but each amoeba's
    /// neighbour state indicates how many ghost amoebas will fall. The
    /// maximum number of ghost to fall for each one is 5, hence the
    /// maximum ghosts to fall is 30.
    ///
    /// \return The vector of 6 ghost amoebas with it's "weight" state.
    ///
    inline const std::vector<Amoeba *> &
    Grid::getWaitingGhostAmoebas (void) const
    {
        return m_WaitingGhostAmoebas;
    }

    ///
    /// \brief Tells if the falling pair is new.
    ///
    /// A falling pair is new when at the last call to update()
    /// the previous falling pair felt and there was not floating amoebas.
    /// Of course, every time a new falling pair starts to fall, this
    /// function returns \a true.
    ///
    /// \return \a true if at the previous call to update() a new pair
    ///         started to fall, \a false otherwise.
    ///
    inline bool
    Grid::hasNewFallingPair (void) const
    {
        return m_HasNewFallingPair;
    }

    ///
    /// \brief Increments the current step chain.
    ///
    /// \param amount The amount to increment the current step chain.
    ///
    inline void
    Grid::incrementCurrentStepChain (uint8_t amount)
    {
        setCurrentStepChain (getCurrentStepChain () + amount);
    }

    ///
    /// \brief Increments the current score.
    ///
    /// \param amount The amount increments the score.
    ///
    inline void
    Grid::incrementScore (uint16_t amount)
    {
        setScore (getScore () + amount);
    }

    ///
    /// \brief Tells if the grid is filled.
    ///
    /// The grid is considered filled when the falling pair can't
    /// fall because the grid's position (3, 0) has a non-floating
    /// amoeba (i.e., the falling amoebas can't fall inside the
    /// grid.)
    ///
    /// When the grid is filled, the game is over and the player
    /// lost.
    ///
    /// \return \a true if the grid is filled, \a false otherwise.
    ///
    inline bool
    Grid::isFilled (void) const
    {
        return m_Filled;
    }

    ///
    /// \brief Sets the current step chain.
    ///
    /// \param stepChain The new step chain to set.
    ///
    inline void
    Grid::setCurrentStepChain (uint8_t stepChain)
    {
        m_CurrentStepChain = stepChain;
    }

    ///
    /// \brief Sets the number of waiting ghosts.
    ///
    /// \param number The new number of ghosts amoebas that are waiting
    ///               to fall into the grid. It can't be greater than 30,
    ///               if it's then it gets clapped.
    ///
    inline void
    Grid::setNumberOfWaitingGhosts (uint8_t number)
    {
        if ( number > 30 )
        {
            m_WaitingGhostAmoebasNumber = 30;
        }
        else
        {
            m_WaitingGhostAmoebasNumber = number;
        }
    }

    ///
    /// \brief Sets the number of amoebas to send to the opponent grid.
    ///
    /// \param ghostAmoebas The number of ghost amoebas to send.
    ///
    inline void
    Grid::setOpponentGhostAmoebas (uint8_t ghostAmoebas)
    {
        m_OpponentGhostAmoebas = ghostAmoebas;
    }

    ///
    /// \brief Sets the score of the grid.
    ///
    /// \param score  The new score value.
    ///
    inline void
    Grid::setScore (uint32_t score)
    {
        m_Score = score;
    }

}

#endif // !AMOEBAX_GRID_H
