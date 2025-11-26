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
#if !defined (AMOEBAX_AMOEBA_H)
#define AMOEBAX_AMOEBA_H

#include <stdint.h>

namespace Amoebax
{
    ///
    /// \class Amoeba
    /// \brief Holds a single amoeba's type and status.
    ///
    class Amoeba
    {
        public:
            ///
            /// \brief The possible amoebas' colours.
            ///
            enum Colour
            {
                /// Red colour.
                ColourRed = 0,
                /// Blue colour.
                ColourBlue = 1,
                /// Green colour.
                ColourGreen = 2,
                /// Orange colour.
                ColourOrange = 3,
                /// Purple colour.
                ColourPurple = 4,
                /// This isn't a colour. This is the for ghost amoebas.
                ColourGhost = 5,
                /// This isn't a colour. This is for empty cells.
                ColourNone = 6
            };

            ///
            /// \brief The possible state of the amoabas' neighbourhood.
            enum State
            {
                /// No neighbours.
                StateNone = 0,
                /// One neighbour at top.
                StateTop = 1,
                /// One neighbour at right.
                StateRight = 2,
                /// Two neighbours: at top and at right.
                StateTopRight = 3,
                /// One neighbour at bottom.
                StateBottom = 4,
                /// Two neighbours: at top and at bottom.
                StateTopBottom = 5,
                /// Two neighbours: at right and at bottom.
                StateRightBottom = 6,
                /// Three neighbours: at top, at right, and at bottom.
                StateTopRightBottom = 7,
                /// One neighbour at left.
                StateLeft = 8,
                /// Two neighbours: at top and at left.
                StateTopLeft = 9,
                /// Two neighbours: at right and at left.
                StateRightLeft = 10,
                /// Three neighbours: at top, at right, and at left.
                StateTopRightLeft = 11,
                /// Two neighbours: at bottom and at left.
                StateBottomLeft = 12,
                /// Three neighbours: at top, at bottom, and at left.
                StateTopBottomLeft = 13,
                /// Three neighbours: at right, at bottom, and at left.
                StateRightBottomLeft = 14,
                /// Four neighbours: at top, at right, at bottom, and at left.
                StateTopRightBottomLeft = 15
            };

            explicit Amoeba (Colour colour);
            ~Amoeba (void);

            Colour getColour (void) const;
            State getState (void) const;
            uint16_t getX (void) const;
            uint16_t getY (void) const;
            bool isDying (void) const;
            bool isVisible (void) const;
            void setDying (bool dying);
            void setState (State state);
            void setVisible (bool visible);
            void setX (uint16_t x);
            void setY (uint16_t y);

        private:
            /// Amoeba's colour.
            Colour m_Colour;
            /// Tells if the amoeba is dying.
            bool m_Dying;
            /// Amoeba's state or neighbourhood state.
            State m_State;
            /// Tells if the amoeba should be visible.
            bool m_Visible;
            /// Amoeba's X position.
            uint16_t m_X;
            /// Amoeba's Y position.
            uint16_t m_Y;
    };

    ///
    /// \brief Gets amoeba's colour.
    ///
    /// \return The amoeba's colour.
    ///
    inline Amoeba::Colour
    Amoeba::getColour (void) const
    {
        return m_Colour;
    }

    ///
    /// \brief Gets amoeba's current state.
    ///
    /// \return The current amoeba's state.
    ///
    inline Amoeba::State
    Amoeba::getState (void) const
    {
        return m_State;
    }

    ///
    /// \brief Gets the current amoeba's X position.
    ///
    /// \return The current amoeba's X position.
    ///
    inline uint16_t
    Amoeba::getX (void) const
    {
        return m_X;
    }

    ///
    /// \brief Gets the current amoeba's Y position.
    ///
    /// \return The current amoeba's Y position.
    ///
    inline uint16_t
    Amoeba::getY (void) const
    {
        return m_Y;
    }

    ///
    /// \brief Tells if the amoeba is dying.
    ///
    /// \return \a true if the amoeba is dying, \a false otherwise.
    ///
    inline bool
    Amoeba::isDying (void) const
    {
        return m_Dying;
    }

    ///
    /// \brief Tells if the amoeba is visible.
    ///
    /// \return \a true if the amoeba is visible, \a false otherwise.
    ///
    inline bool
    Amoeba::isVisible (void) const
    {
        return m_Visible;
    }

    ///
    /// \brief Sets the amoeba to a dying state.
    ///
    /// \param dying The new dying state to set the amoeba to. This is the
    ///              value returned by isDying().
    ///
    inline void
    Amoeba::setDying (bool dying)
    {
        m_Dying = dying;
    }

    ///
    /// \brief Sets the amoeba's neighbourhood state.
    ///
    /// \param state The new amoeba's neighbourhood state.
    ///
    inline void
    Amoeba::setState (State state)
    {
        m_State = state;
    }

    ///
    /// \brief Sets the amoeba's visibility.
    ///
    /// \param visible \a true if the amoeba should be visible, \a false
    ///                otherwise.
    ///
    inline void
    Amoeba::setVisible (bool visible)
    {
        m_Visible = visible;
    }

    ///
    /// \brief Sets the amoeba's X screen position.
    ///
    /// \param x The new amoeba's X screen position.
    ///
    inline void
    Amoeba::setX (uint16_t x)
    {
        m_X = x;
    }

    ///
    /// \brief Sets the amoeba's Y screen position.
    ///
    /// \param y The new amoeba's Y screen position.
    ///
    inline void
    Amoeba::setY (uint16_t y)
    {
        m_Y = y;
    }
}

#endif // !AMOEBAX_AMOEBA_H
