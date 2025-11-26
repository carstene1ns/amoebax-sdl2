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
#if !defined (AMOEBAX_PAIR_GENERATOR_H)
#define AMOEBAX_PAIR_GENERATOR_H

#include <vector>
#include "Amoeba.h"

namespace Amoebax
{
    // Forward declarations.
    class Grid;

    ///
    /// \class PairGenerator.
    /// \brief Generates pairs of amoebas.
    ///
    class PairGenerator
    {
        public:
            void addGrid (Grid *griddToAdd);
            void generate (uint8_t pairsToGenerate = 1) const;

        private:
            ///
            /// \struct GeneratePair
            /// \brief Tells to the grid which pair to generate.
            ///
            struct GeneratePair: public std::unary_function<Grid *, void>
            {
                /// The colour of the first amoeba.
                Amoeba::Colour firstAmoebaColour;
                /// The colour of the second amoeba.
                Amoeba::Colour secondAmoebaColour;

                ///
                /// \brief Default constructor.
                ///
                /// \param firstAmoeba The colour to set the first amoeba to.
                /// \param secondAmoeba The colour to set the second amoeba to.
                ///
                GeneratePair (Amoeba::Colour firstAmoeba,
                              Amoeba::Colour secondAmoeba):
                    firstAmoebaColour (firstAmoeba),
                    secondAmoebaColour (secondAmoeba)
                {
                }

                ///
                /// \brief Adds a new amoeba pair to a grid.
                ///
                /// The new pair will be created using the colours given at the
                /// constructor and will be passed to the \a grid. The grid
                /// then has the ownership and those amoebas and must
                /// destroy it when no longer needed.
                ///
                /// \param grid The grid to add the new pair to.
                ///
                void
                operator() (Grid *grid)
                {
                    Amoeba *firstAmoeba = 0;
                    Amoeba *secondAmoeba = 0;
#if !defined (IS_GP2X_HOST)
                    try
#endif // !IS_GP2X_HOST
                    {
                        firstAmoeba = new Amoeba (firstAmoebaColour);
                        secondAmoeba = new Amoeba (secondAmoebaColour);
                    }
#if !defined (IS_GP2X_HOST)
                    // XXX: GP2X can't even start with this try catch block.
                    catch (std::exception e)
                    {
                        delete firstAmoeba;
                        delete secondAmoeba;
                        throw e;
                    }
#endif // !IS_GP2X_HOST
                    grid->addNewPair (firstAmoeba, secondAmoeba);
                }
            };

            /// The list of grid to add the generated pairs to.
            std::vector<Grid *> m_Grids;
    };
}

#endif // !AMOEBAX_PAIR_GENERATOR_H
