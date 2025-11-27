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
#if !defined (AMOEBAX_AI_PLAYER_FACTORY_H)
#define AMOEBAX_AI_PLAYER_FACTORY_H

#include <string>
#include "IPlayer.h"
#include "AIPlayer.h"

namespace Amoebax
{
    class AIPlayer;
    ///
    /// \class AIPlayerFactory
    /// \brief Gets the opponent's name and object from a level.
    ///
    class AIPlayerFactory
    {
        public:
            /// The maximum number of AI players.
            static const uint8_t k_MaxPlayerLevel = 6;

            static AIPlayer *create (uint8_t level, IPlayer::PlayerSide);
            static std::string getBackgroundFileName (uint8_t level);
            static std::string getPlayerName (uint8_t level);
            static std::string getRandomBackgroundFileName ();
            static IPlayer::PlayerSide getWinner (uint8_t leftLevel,
                                                  uint8_t rightLevel);

        private:
            ///
            /// \brief Constructor.
            ///
            /// \note Declared private because we don't want objects
            ///       of this class.
            ///
            AIPlayerFactory (void);
    };
}
#endif // AMOEBAX_AI_PLAYER_FACTORY_H
