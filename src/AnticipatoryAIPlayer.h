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
#if !defined (AMOEBAX_ANTICIPATORY_AI_PLAYER_H)
#define AMOEBAX_ANTICIPATORY_AI_PLAYER_H

#include "GridStatus.h"
#include "AIPlayer.h"

namespace Amoebax
{
    ///
    /// \class AnticipatoryAIPlayer
    /// \brief An AI player that takes into accound the next one or two pairs.
    ///
    class AnticipatoryAIPlayer: public AIPlayer
    {
        public:
            ///
            /// \enum Anticipate
            /// \brief Tells which pairs to anticipate.
            ///
            enum Anticipate
            {
                /// Anticipate only the next falling pair.
                AnticipateNextOne,
                /// Anticipate the next and following pairs.
                AnticipateNextTwo
            };

            AnticipatoryAIPlayer (IPlayer::PlayerSide side,
                                  Anticipate anticipate);

        protected:
            virtual int32_t computeScore (const GridStatus::PositionResult &result) const;
            virtual bool shouldCheckFollowingFallingPair (void) const;
            virtual bool shouldCheckNextFallingPair (void) const;
            virtual bool shouldThePairFall (void) const;
            virtual bool shouldSatelliteBeVisible (void) const;

        private:
            inline Anticipate getPairsToAnticipate (void) const;

            /// Tells which pairs to anticipate.
            Anticipate m_PairsToAnticipate;
    };
}

#endif // !AMOEBAX_ANTICIPATORY_AI_PLAYER_H
