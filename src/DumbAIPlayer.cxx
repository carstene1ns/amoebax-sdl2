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
#if defined (HAVE_CONFIG_H)
#include <config.h>
#endif // HAVE_CONFIG_H
#include "DumbAIPlayer.h"

using namespace Amoebax;

/// The average time the player waits between moves, in ms.
static const uint32_t k_TimeToWaitForNextMove = 700;
/// The max. number of milliseconds to add/subtract to the average.
static const uint32_t k_TimeDeviation = 125;

///
/// \brief Default constructor.
///
/// \param side The grid's side the player is controlling.
///
DumbAIPlayer::DumbAIPlayer (IPlayer::PlayerSide side):
    AIPlayer (side, k_TimeToWaitForNextMove, k_TimeDeviation)
{
}

int32_t
DumbAIPlayer::computeScore (const GridStatus::PositionResult &result) const
{
    return rand ();
}

bool
DumbAIPlayer::shouldCheckFollowingFallingPair (void) const
{
    return false;
}

bool
DumbAIPlayer::shouldCheckNextFallingPair (void) const
{
    return false;
}

bool
DumbAIPlayer::shouldThePairFall (void) const
{
    return false;
}

bool
DumbAIPlayer::shouldSatelliteBeVisible (void) const
{
    return true;
}
