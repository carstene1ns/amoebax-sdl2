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
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <stdexcept>
#include "Grid.h"
#include "PairGenerator.h"

using namespace Amoebax;

///
/// \brief Adds a new grid that will receive the new pair generated event.
///
/// \param gridToAdd The grid to add as an observer.
///
void
PairGenerator::addGrid (Grid *gridToAdd)
{
    m_Grids.push_back (gridToAdd);
    gridToAdd->setGenerator (this);
}

///
/// \brief Generate new amoebas pairs.
///
/// \param pairsToGenerate The number of new amobas pairs to generate.
///
void
PairGenerator::generate (uint8_t pairsToGenerate) const
{
    for ( uint8_t currentPair = 0 ; currentPair < pairsToGenerate ; ++currentPair )
    {
        Amoeba::Colour firstAmoeba =
            static_cast<Amoeba::Colour>(rand () % Amoeba::ColourGhost);
        Amoeba::Colour secondAmoeba =
            static_cast<Amoeba::Colour>(rand () % Amoeba::ColourGhost);

        std::for_each (m_Grids.begin (), m_Grids.end (),
                       GeneratePair (firstAmoeba, secondAmoeba));
    }
}
