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
#include <limits>
#include "AnticipatoryAIPlayer.h"
#include "Grid.h"
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
/// \param anticipate Which pairs to anticipate.
///
AnticipatoryAIPlayer::AnticipatoryAIPlayer (IPlayer::PlayerSide side,
                                            Anticipate anticipate):
    AIPlayer (side, k_TimeToWaitForNextMove, k_TimeDeviation),
    m_PairsToAnticipate (anticipate)
{
}

int32_t
AnticipatoryAIPlayer::computeScore (const GridStatus::PositionResult &result) const
{
    int32_t score = 0;

    score -= 1 * (result.heightMean);
    score -= 5 * (result.heightMax);
    score += 20 * result.stepChains.at (0);
    score += 10 * result.groupsAmoebasNumber;
    if ( result.heightMax > (Grid::k_VisibleHeight - 1) )
    {
        score -= 10000;
    }

    return score;
}

///
/// \brief Gets the number of next pairs to anticipate.
///
/// \return If anticipate the next one alone or the next two pairs.
///
AnticipatoryAIPlayer::Anticipate
AnticipatoryAIPlayer::getPairsToAnticipate (void) const
{
    return m_PairsToAnticipate;
}

bool
AnticipatoryAIPlayer::shouldCheckFollowingFallingPair (void) const
{
    return AnticipateNextTwo == getPairsToAnticipate ();
}

bool
AnticipatoryAIPlayer::shouldCheckNextFallingPair (void) const
{
    return true;
}

bool
AnticipatoryAIPlayer::shouldThePairFall (void) const
{
    return true;
}

bool
AnticipatoryAIPlayer::shouldSatelliteBeVisible (void) const
{
    return false;
}
