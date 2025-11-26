//
// Cross-platform free Puyo-Puyo clone.
// Copyright (C) 2007 Emma's software
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
#include <cmath>
#include <limits>
#include "AdvancedAIPlayer.h"
#include "GridStatus.h"

using namespace Amoebax;

/// The average time the player waits between moves, in ms.
static const uint32_t k_TimeToWaitForNextMove = 175;
/// The max. number of milliseconds to add/subtract to the average.
static const uint32_t k_TimeDeviation = 30;

///
/// \brief Default constructor.
///
/// \param side The grid's side the player is controlling.
///
AdvancedAIPlayer::AdvancedAIPlayer (IPlayer::PlayerSide side):
    AIPlayer (side, k_TimeToWaitForNextMove, k_TimeDeviation)
{
}

int32_t
AdvancedAIPlayer::computeScore (const GridStatus::PositionResult &result) const
{
    int32_t score = 0;
    // If the third columns gets too high, it's a very bad move.
    if ( result.heightThirdColumn >= Grid::k_VisibleHeight - 2)
    {
        score = -10000;
    }
    else
    {
        score -= 10 * (result.heightMean);
        score -= 5 * (result.heightMax);
        // For each step chain that we make, we get the double of points,
        // so its a good idea :-)
        std::vector<uint16_t>::const_iterator currentStepChain = result.stepChains.begin ();
        for (uint8_t numStepChains = 0 ;
             currentStepChain != result.stepChains.end () ;
             ++currentStepChain, ++numStepChains )
        {

            score += (20 * (*currentStepChain)) /
                     1;//static_cast<int32_t>(std::pow (2.0f, numStepChains));
        }
        // It's a good idea to have as many amoebas together as possible.
        score += 10 * result.groupsAmoebasNumber;
        // And to remove as many ghost amoebas as possible.
        score += 15 * result.ghostAmoebasRemoved;

        // Get the number of ghost amoebas that are going to fall.
        uint8_t numGhostAmoebas = 0;
        const std::vector<Amoeba *> &ghostAmoebas = getGrid ()->getWaitingGhostAmoebas ();
        for ( std::vector<Amoeba *>::const_iterator currentGhost = ghostAmoebas.begin () ;
              currentGhost != ghostAmoebas.end () ; ++currentGhost )
        {
            Amoeba *ghost = *currentGhost;
            numGhostAmoebas += uint8_t (ghost->getState ());
        }

        // The further the pair, the less important it becomes.
        switch ( getWhichPairToCheck () )
        {
            case CheckingFollowingFallingPair:
                // Now, if we have "a lot of ghost" the following pair is not
                // important to us.
                if ( numGhostAmoebas >= 6 )
                {
                    score = 0;
                }
                else
                {
                    score = static_cast<int32_t> (score * 0.20);
                }
                break;

            case CheckingNextFallingPair:
                if ( numGhostAmoebas >= 6 )
                {
                    score = static_cast<int32_t> (score * 0.40);
                }
                else
                {
                    score = static_cast<int32_t> (score * 0.60);
                }
                break;

            default:
                // Nothing to do.
                break;
        }
    }

    return score;
}

bool
AdvancedAIPlayer::shouldCheckFollowingFallingPair (void) const
{
    return true;
}

bool
AdvancedAIPlayer::shouldCheckNextFallingPair (void) const
{
    return true;
}

bool
AdvancedAIPlayer::shouldThePairFall (void) const
{
    return true;
}

bool
AdvancedAIPlayer::shouldSatelliteBeVisible (void) const
{
    return false;
}
