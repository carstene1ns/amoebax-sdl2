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
#include <cassert>
#include <limits>
#include "Amoeba.h"
#include "Grid.h"
#include "GridStatus.h"

using namespace Amoebax;

///
/// \brief Default constructor.
///
/// Creates an empty state.
///
GridStatus::GridStatus (void):
    m_State (Grid::k_GridWidth * Grid::k_GridHeight, Amoeba::ColourNone)
{
}

///
/// \brief Constructor.
///
/// \param state The current state of the grid.
///
GridStatus::GridStatus (const std::vector<Amoeba::Colour> &state):
    m_State (state)
{
    assert ( m_State.size () == (Grid::k_GridWidth * Grid::k_GridHeight) &&
             "The state size is not the same as the grid size." );
}

///
/// \brief Checks what would happen in a given position.
///
/// Checks what would happen if the falling pair falls in the given
/// positions. This is mainly used for AI players.
///
/// \note This code assumes that the main amoeba will fall first and then
///       will fall the satellite amoeba. This means that the satellite amoeba
///       can't be below the main amoeba for this function to work properly.
///
/// \param main The position of the main falling amoeba.
/// \param satellite The position of the satellite falling amoeba.
/// \param result The result of the position of the pair.
///
void
GridStatus::checkPositions (const Grid::FallingAmoeba &main,
                           const Grid::FallingAmoeba &satellite,
                           PositionResult &result)
{
    // Store the main colour.
    Amoeba::Colour mainColour = main.amoeba->getColour ();
    // Find the Y position of the main amoeba.
    int mainY = Grid::k_GridHeight - 1;
    while ( Amoeba::ColourNone != getAmoebaColourAt (main.x, mainY) )
    {
        mainY--;
    }
    // Place the main amoeba as if it already fell.
    setAmoebaColourAt (main.x, mainY, mainColour);

    // Store the satellite colour.
    Amoeba::Colour satelliteColour = satellite.amoeba->getColour ();
    // Find the Y position of the satellite amoeba.
    int satelliteY = Grid::k_GridHeight - 1;
    while ( Amoeba::ColourNone != getAmoebaColourAt (satellite.x, satelliteY) )
    {
        satelliteY--;
    }
    // Place the main amoeba as if it already fell.
    setAmoebaColourAt (satellite.x, satelliteY, satelliteColour);

    result.ghostAmoebasRemoved = 0;
    // Now check the groups that the main and satellite amoebas can make.
    {
        uint16_t stepChainGroup = 0;
        std::set<Position> visitedPositions;
        Group group;
        Group ghosts;
        // Main amoeba.
        makeGroup (group, ghosts, main.x, mainY, visitedPositions);
        if ( 4 <= group.size () )
        {
            stepChainGroup += group.size ();
            removeAmoebaGroup (group);
            result.ghostAmoebasRemoved += ghosts.size ();
            removeAmoebaGroup (ghosts);
        }
        // Satellite amoeba.
        makeGroup (group, ghosts, satellite.x, satelliteY, visitedPositions);
        if ( 4 <= group.size () )
        {
            stepChainGroup += group.size ();
            removeAmoebaGroup (group);
            result.ghostAmoebasRemoved += ghosts.size ();
            removeAmoebaGroup (ghosts);
        }
        result.stepChains.clear ();
        result.stepChains.push_back (stepChainGroup);
    }

    // Find the amoebas that are left floating and make them fall, check
    // for groups and remove the groups, until there are no more floating
    // amoebas.
    Group floatingAmoebas (findFloatingAmoebas ());
    while ( !floatingAmoebas.empty () )
    {
        std::vector<Group> groups (makeFloatingAmoebasFall (floatingAmoebas));
        std::vector<Group>::const_iterator currentGroup = groups.begin ();
        uint16_t stepChainGroup = 0;
        for ( bool isGhostGroup = false ;
              currentGroup != groups.end () ;
              ++currentGroup, isGhostGroup = !isGhostGroup )
        {
            // For each floating there's two consecutive groups: one for
            // the "regular" amoebas and the other for the ghosts groups.
            if ( isGhostGroup )
            {
                result.ghostAmoebasRemoved += currentGroup->size ();
            }
            else
            {
                stepChainGroup += currentGroup->size ();
            }
            removeAmoebaGroup (*currentGroup);
        }
        result.stepChains.push_back (stepChainGroup);
        floatingAmoebas = findFloatingAmoebas ();
    }
    // Now get the height of each column, accumulate the height to then
    // compute the mean, store the maximum and also store separatelly the
    // third column's height.
    result.heightMax = std::numeric_limits<uint16_t>::min ();
    result.heightMean = 0;
    result.heightThirdColumn = 0;
    for ( uint8_t currentColumn = 0 ; currentColumn < Grid::k_GridWidth ;
          ++currentColumn )
    {
        // Get the column's height.
        uint16_t y = Grid::k_GridHeight - 1;
        uint16_t height = 0;
        while ( Amoeba::ColourNone != getAmoebaColourAt (currentColumn, y) )
        {
            --y;
            ++height;
        }
        // Accumulate the height to compute the mean and store it if it's max.
        result.heightMax = std::max (result.heightMax, height);
        result.heightMean += height;
        // If we are on the third column (i.e, column 2) store it.
        if ( 2 == currentColumn )
        {
            result.heightThirdColumn = height;
        }
    }
    // Now compute the actual mean.
    result.heightMean /= Grid::k_GridWidth;

    // Get the number of groups of less than 4 amoebas (i.e., at this state
    // just the groups since there's no group of 4 or more amoebas.)
    {
        std::set<Position> visitedPositions;
        result.groups.clear ();
        result.groupsAmoebasNumber = 0;
        for ( int16_t currentY = 0 ; currentY < Grid::k_GridHeight ; ++currentY )
        {
            for ( int16_t currentX = 0 ; currentX < Grid::k_GridWidth ; ++currentX )
            {
                Group group;
                Group ghosts;
                makeGroup (group, ghosts, currentX, currentY, visitedPositions);
                if ( 1 < group.size () )
                {
                    result.groupsAmoebasNumber += group.size ();
                    result.groups.push_back (group.size ());
                }
            }
        }
    }
}

///
/// \brief Finds all floating amoebas.
///
/// A floating amoeba is just an amoeba that has lost its supporting
/// amoeba and so it's "floating" on the grid.
///
/// \return The list of all floating amoebas starting from the lower
///         grid's line.
///
std::vector<GridStatus::Position>
GridStatus::findFloatingAmoebas (void) const
{
    // Check all but the last line.
    Group floatingAmoebas;
    for ( int16_t row = Grid::k_GridHeight - 2 ; row >= 0 ; --row )
    {
        for ( int16_t column = 0 ; column < Grid::k_GridWidth ; ++column )
        {
            if ( Amoeba::ColourNone != getAmoebaColourAt (column, row) &&
                 Amoeba::ColourNone == getAmoebaColourAt (column, row + 1) )
            {
                floatingAmoebas.push_back (std::make_pair (column, row));
            }
        }
    }
    return floatingAmoebas;
}

///
/// \brief Gets the amoeba's colour at a given grid's position.
///
/// \param x The X position of the grid to get the amoeba's colour from.
/// \param y The Y position of the grid to get the amoeba's colour from.
/// \return The amoeba's colour at position \p x and \p y or Amoeba::ColourNone
///         if there is no amoeba at this position or the position is
///         incorrect (i.e., out of grid's limits.)
///
inline Amoeba::Colour
GridStatus::getAmoebaColourAt (int16_t x, int16_t y) const
{
    if ( 0 <= x && x < Grid::k_GridWidth &&
         0 <= y && y < Grid::k_GridHeight )
    {
        return m_State[y * Grid::k_GridWidth + x];
    }
    return Amoeba::ColourNone;
}

///
/// \brief Makes the floating amoebas fall into they right position.
///
/// Once a floating amoeba is identified, this function just takes it and
/// makes it fall until another amoeba is found or the bottom-most line
/// is reached. Then it checks if it can make more groups and stores it.
///
/// \param floatingAmoebas The floating amoebas to make fall.
/// \return A list with all groups made when the amoebas felt.
///
std::vector<GridStatus::Group>
GridStatus::makeFloatingAmoebasFall (const Group &floatingAmoebas)
{
    Group positionsToCheck;
    for ( Group::const_iterator currentAmoeba = floatingAmoebas.begin () ;
          currentAmoeba != floatingAmoebas.end () ; ++currentAmoeba )
    {
        int x = currentAmoeba->first;
        int y = currentAmoeba->second;
        Amoeba::Colour colour = getAmoebaColourAt (x, y);
        setAmoebaColourAt (x, y, Amoeba::ColourNone);
        y = Grid::k_GridHeight - 1;
        while ( Amoeba::ColourNone != getAmoebaColourAt (x, y) )
        {
            --y;
        }
        setAmoebaColourAt (x, y, colour);
        positionsToCheck.push_back (std::make_pair (x, y));
    }
    // Now check if we can make groups from the floating amoebas once fallen.
    std::vector<Group> floatingGroups;
    std::set<Position> visitedPositions;
    for ( Group::const_iterator currentPosition = positionsToCheck.begin () ;
          currentPosition != positionsToCheck.end () ; ++currentPosition )
    {
        Group group;
        Group ghosts;
        makeGroup (group, ghosts, currentPosition->first,
                   currentPosition->second, visitedPositions);
        if ( 4 <= group.size () )
        {
            floatingGroups.push_back (group);
            floatingGroups.push_back (ghosts);
        }
    }
    return floatingGroups;
}

///
/// \brief Makes a group of equal coloured amoebas recursively.
///
/// \param group The group vector to save the group member found to.
/// \param ghosts The ghosts amoebas that are in contact with the group.
/// \param x The X position to look for a possible group candidate.
/// \param y The Y position to look for a possible group candidate.
/// \param visitedPositions The set of the already visited position by the
///                         algorithm. Initially, this should be empty.
/// \param groupColour The colour of the amoeba to be part of the group.
/// \param initialAmoeba Tells if the amoeba at \p x and \p y should be the
///                      first amoeba of the group and use its colour to check
///                      for more amoebas.
///
void
GridStatus::makeGroup (Group &group, Group &ghosts, int16_t x, int16_t y,
                      std::set<Position> &visitedPositions,
                      Amoeba::Colour groupColour, bool initialAmoeba)
{
    Position currentPosition (std::make_pair (x, y));
    // Only check this position if we didn't visited it yet.
    if ( visitedPositions.end () == visitedPositions.find (currentPosition) )
    {
        // If there's any amoeba at the given position, check if is of the
        // same colour as the others in the group.
        Amoeba::Colour colour = getAmoebaColourAt (x, y);
        if ( Amoeba::ColourNone != colour )
        {
            // Add the current position to the already visited positions.
            visitedPositions.insert (currentPosition);

            // If this amoeba is the first one, then we need to use its
            // colour as the group's colour.
            if ( initialAmoeba )
            {
                groupColour = colour;
            }
            // Can't make groups of ghost amoebas. If this amoeba is
            // a ghosts, then set it to the ghosts groups and don't look
            // any further.
            if ( Amoeba::ColourGhost == colour )
            {
                ghosts.push_back (currentPosition);
            }
            // Otherwise, if the amoeba's colour is the same as the
            // group's colour, add it to the group vector and check for its
            // neighbours.
            else if ( colour == groupColour )
            {
                group.push_back (currentPosition);
                makeGroup (group, ghosts, x, y - 1, visitedPositions, groupColour, false);
                makeGroup (group, ghosts, x + 1, y, visitedPositions, groupColour, false);
                makeGroup (group, ghosts, x, y + 1, visitedPositions, groupColour, false);
                makeGroup (group, ghosts, x - 1, y, visitedPositions, groupColour, false);
            }
        }
    }
}

///
/// \brief Removes a group of amoebas from the grid.
///
/// \param group The group of amoebas to remove from the grid.
///
void
GridStatus::removeAmoebaGroup (const Group &group)
{
    for ( Group::const_iterator currentPosition = group.begin () ;
          currentPosition != group.end () ; ++currentPosition )
    {
        setAmoebaColourAt (currentPosition->first, currentPosition->second,
                           Amoeba::ColourNone);
    }
}

///
/// \brief Sets the amoeba's colour at a given grid's position.
///
/// \param x The X position to set the amoeba's colour to.
/// \param y The Y position to set the amoeba's colour to.
/// \param colour The amoeba's colour to set at grid's position \p x and \p y.
///
inline void
GridStatus::setAmoebaColourAt (int16_t x, int16_t y, Amoeba::Colour colour)
{
    if ( 0 <= x && x < Grid::k_GridWidth &&
         0 <= y && y < Grid::k_GridHeight )
    {
        m_State[y * Grid::k_GridWidth + x] = colour;
    }
}
