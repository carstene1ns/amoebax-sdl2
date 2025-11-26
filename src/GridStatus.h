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
#if !defined (AMOEBAX_GRID_STATUS_H)
#define AMOEBAX_GRID_STATUS_H

#include <vector>
#include "Amoeba.h"
#include "Grid.h"

namespace Amoebax
{
    ///
    /// \class GridStatus
    /// \brief Stores the state of a grid.
    ///
    /// This class is mainly used for AI players to check different
    /// possible combinations of amoebas in order to compute the
    /// best result.  It stores the state of a grid (i.e., where the
    /// amoebas are in a given time) and lets perform "simulations"
    /// on this grid state.
    ///
    class GridStatus
    {
        public:
            /// The position (x, y) of a amoeba in the grid.
            typedef std::pair<int16_t, int16_t> Position;
            /// A Group of amoebas identified by they position.
            typedef std::vector<Position> Group;

            ///
            /// \struct PositionResult
            /// \brief The result of a position check.
            ///
            /// This struct is used to check the results of
            /// making fall a pair to a position. This structure
            /// is mainly used for AI players.
            ///
            struct PositionResult
            {
                /// Ghost amoebas removed.
                uint16_t ghostAmoebasRemoved;
                /// The groups of same coloured amoebas that are less of 4.
                std::vector<uint16_t> groups;
                /// The total of amoebas that are forming a group.
                uint16_t groupsAmoebasNumber;
                /// The maximum height from all columns.
                uint16_t heightMax;
                /// The mean height form all column.
                uint16_t heightMean;
                /// The height of the 3rd column.
                uint16_t heightThirdColumn;
                /// The steps chains and their size. If no step chain was formed, the first element is 0.
                std::vector<uint16_t> stepChains;
            };

            explicit GridStatus ();
            explicit GridStatus (const std::vector<Amoeba::Colour> &state);

            void checkPositions (const Grid::FallingAmoeba &main,
                                 const Grid::FallingAmoeba &satellite,
                                 PositionResult &result);

        private:
            Group findFloatingAmoebas (void) const;
            Amoeba::Colour getAmoebaColourAt (int16_t x, int16_t y) const;
            std::vector<Group> makeFloatingAmoebasFall (const Group &floatingAmoebas);
            void makeGroup (Group &group,
                            Group &ghosts,
                            int16_t x, int16_t y,
                            std::set<Position> &visitedPositions,
                            Amoeba::Colour groupColour = Amoeba::ColourNone,
                            bool initialAmoeba = true);
            void removeAmoebaGroup (const Group &group);
            void setAmoebaColourAt (int16_t x, int16_t y, Amoeba::Colour colour);

            /// The current grid's state.
            std::vector<Amoeba::Colour> m_State;
    };
}

#endif // !AMOEBAX_GRID_STATUS_H
