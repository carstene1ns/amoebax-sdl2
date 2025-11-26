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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
#if !defined (AMOEBAX_IMATCH_OBSERVER_H)
#define AMOEBAX_IMATCH_OBSERVER_H

#include "IPlayer.h"

namespace Amoebax
{
    ///
    /// \class IMatchObserver
    /// \brief Interface for classes that observers the match for a winner.
    ///
    class IMatchObserver
    {
        public:
            ///
            /// \brief Destructor.
            ///
            virtual ~IMatchObserver (void)
            { }

            ///
            /// \brief The current two players match is over.
            ///
            /// This is called by the TwoPlayersState when the match
            /// between the two players is over.
            ///
            /// \param winner Tells which player's side is the winner.
            /// \param leftPlayerScore The score of the left player.
            /// \param rightPlayerScore The score of the right player.
            ///
            virtual void endOfMatch (IPlayer::PlayerSide winner,
                                     uint32_t leftPlayerScore,
                                     uint32_t rightPlayerScore) = 0;
    };
}

#endif // !AMOEBAX_IMATCH_OBSERVER_H
