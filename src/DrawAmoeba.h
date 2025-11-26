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
#if !defined (AMOEBAX_DRAW_AMOEBA_H)
#define AMOEBAX_DRAW_AMOEBA_H

#include <functional>
#include <algorithm>
#include "Surface.h"

namespace Amoebax
{
    ///
    /// \struct DrawAmoeba
    /// \brief Draws a single amoeba.
    ///
    /// This struct is intended to be used as a parameter to std::for_each()
    /// while trying to draw the amoebas from a Grid.
    ///
    struct DrawAmoeba
    {
        /// The size (both width and height are the same) of a single amoeba.
        uint8_t amoebasSize;
        /// The SDL surface to draw the amoeba to.
        SDL_Surface *destination;
        /// The surface to get the amoeba's images to use.
        Surface *source;

        ///
        /// \brief Default constructor.
        ///
        /// \param amoebasSize The size of a single amoeba. The width and the
        ///                    height of an amoeba must be the same.
        /// \param source The surface to use to draw the amoebas from.
        /// \param destination The SDL surface to use to draw the amoebas to.
        DrawAmoeba (uint8_t amoebasSize, Surface *source,
                    SDL_Surface *destination):
            amoebasSize (amoebasSize),
            destination (destination),
            source (source)
        {
        }

        ///
        /// \brief Draws a single amoeba.
        ///
        /// \param amoeba The amoeba to draw.
        ///
        void
        operator() (const Amoeba *amoeba)
        {
            if ( amoeba->isVisible () )
            {
                source->blit (amoebasSize * amoeba->getState (),
                              amoebasSize * amoeba->getColour (),
                              amoebasSize, amoebasSize,
                              amoeba->getX (), amoeba->getY (), destination);
            }
        }
    };
}

#endif // !AMOEBAX_DRAW_AMOEBA_H
