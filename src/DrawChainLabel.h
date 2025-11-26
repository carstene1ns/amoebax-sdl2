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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
#if !defined (AMOEBAX_DRAW_CHAIN_LABEL_H)
#define AMOEBAX_DRAW_CHAIN_LABEL_H

#include <functional>
#include <algorithm>
#include "ChainLabel.h"
#include "Font.h"
#include "Surface.h"

namespace Amoebax
{
    ///
    /// \struct DrawChainLabel
    /// \brief Draws a single chain label.
    ///
    /// This struct is intended to be used as a parameter to std::for_each()
    /// while trying to draw the list of chain labels from a Grid.
    ///
    struct DrawChainLabel: public std::unary_function<ChainLabel *, void>
    {
        /// The surface to get the chain label's images.
        Surface *chainLabelImage;
        /// The SDL surface to draw the chain label to.
        SDL_Surface *destination;
        /// The font to use to draw the step chain.
        Font *font;

        ///
        /// \brief Default constructor.
        ///
        /// \param chainLabelImage The image of the chain label.
        /// \param font The font to use the step chain.
        /// \param destination The destination to draw the chain label to.
        ///
        DrawChainLabel (Surface *chainLabelImage, Font *font,
                        SDL_Surface *destination):
            chainLabelImage (chainLabelImage),
            destination (destination),
            font (font)
        {
        }

        ///
        /// \brief Draws a single chain label.
        ///
        /// \param chainLabel The chain label to draw.
        ///
        void
        operator() (const ChainLabel *chainLabel)
        {
            if ( chainLabel->isActive () )
            {
                std::stringstream chainLabelText;
                chainLabelText << int(chainLabel->getStepChain ()) << " ";
                uint16_t chainLabelTextWidth =
                    font->getTextWidth (chainLabelText.str ());
                int16_t x = chainLabel->getX () -
                            (chainLabelTextWidth +
                             chainLabelImage->getWidth ()) / 2;
                int16_t y = chainLabel->getY ();
                font->write (chainLabelText.str (),
                             x, y - font->getHeight () / 2, destination);
                chainLabelImage->blit (x + chainLabelTextWidth,
                                       y - chainLabelImage->getHeight () / 2,
                                       destination);
            }
        }
    };
}

#endif // !AMOEBAX_DRAW_CHAIN_LABEL_H
