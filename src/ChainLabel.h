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
#if !defined (AMOEBAX_CHAIN_LABEL_H)
#define AMOEBAX_CHAIN_LABEL_H

#include <stdint.h>

namespace Amoebax
{
    ///
    /// \class ChainLabel
    /// \brief The label shown on the grid when a chain is made.
    ///
    class ChainLabel
    {
        public:
            ChainLabel (uint8_t stepChain, int16_t x, int16_t y,
                        int16_t finalY);
            ~ChainLabel (void);

            void activate (void);
            uint8_t getStepChain (void) const;
            int16_t getX (void) const;
            int16_t getY (void) const;
            bool isActive (void) const;
            bool isAlive (void) const;
            void update (uint32_t elapsedTime);

        private:
            /// The speed in which the label should go up (pixels / update).
            static const uint8_t k_Speed = 2;

            /// Tells if the chain label is active and must be updated.
            bool m_Active;
            /// Tells if the chain label is alive or should be deleted.
            bool m_Alive;
            /// The final Y position the label should end at.
            int16_t m_FinalY;
            /// The step chain the label is showing to the user.
            uint8_t m_StepChain;
            /// The time the label should be allowed to live when at final Y.
            int16_t m_TimeToLive;
            /// The current horizontal position of the label.
            int16_t m_X;
            /// The current vertical position of the label.
            int16_t m_Y;
    };

    ///
    /// \brief Default constructor.
    ///
    /// \param stepChain The step chain of the label.
    /// \param x The X screen position where the label should be centered at.
    /// \param y The Y screen position where the label should be centered at.
    /// \param finalY The Y position where the label should stop to move.
    ///
    inline
    ChainLabel::ChainLabel (uint8_t stepChain, int16_t x, int16_t y,
                            int16_t finalY):
        m_Active (false),
        m_Alive (true),
        m_FinalY (finalY),
        m_StepChain (stepChain),
        m_TimeToLive (500),
        m_X (x),
        m_Y (y)
    {
    }

    ///
    /// \brief Default constructor.
    ///
    inline
    ChainLabel::~ChainLabel (void)
    {
    }

    ///
    /// \brief Activates the chain label.
    ///
    inline void
    ChainLabel::activate (void)
    {
        m_Active = true;
    }

    ///
    /// \brief Gets the step chain.
    ///
    /// \return The step chain of the label.
    ///
    inline uint8_t
    ChainLabel::getStepChain (void) const
    {
        return m_StepChain;
    }

    ///
    /// \brief Gets the X screen position.
    ///
    /// \return The X position of the screen where the label should be centered
    ///         at.
    ///
    inline int16_t
    ChainLabel::getX (void) const
    {
        return m_X;
    }

    ///
    /// \brief Gets the Y screen position.
    ///
    /// \return The Y position of the screen where the label should be centered
    ///         at.
    ///
    inline int16_t
    ChainLabel::getY (void) const
    {
        return m_Y;
    }

    ///
    /// \brief Tells if the chain label is active (i.e., should be rendered.)
    ///
    /// \return \a true if the chain lable is active and should be rendered,
    ///         \a false otherwise.
    ///
    inline bool
    ChainLabel::isActive (void) const
    {
        return m_Active;
    }

    ///
    /// \brief Tells if the chain is alive.
    ///
    /// \return \a true if the chain label is alive and should not be deleted,
    ///         \a false otherwise.
    ///
    inline bool
    ChainLabel::isAlive (void) const
    {
        return m_Alive;
    }
}

#endif // !AMOEBAX_CHAIN_LABEL_H
