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
#if defined (HAVE_CONFIG_H)
#include <config.h>
#endif // HAVE_CONFIG_H
#include <cassert>
#include "AdvancedAIPlayer.h"
#include "AIPlayerFactory.h"
#include "AnticipatoryAIPlayer.h"
#include "DumbAIPlayer.h"
#include "SimpleAIPlayer.h"

using namespace Amoebax;

///
/// \brief Creates a pointer to an AI player given a level.
///
/// Based on the player's level, this function creates the most
/// suitable AIPlayer class and returns it.
///
/// \param level The level to use to create the AI player.
/// \param side The side the player will play at.
/// \return The pointer to the newly created AI player.
///
AIPlayer *
AIPlayerFactory::create (uint8_t level, IPlayer::PlayerSide side)
{
    AIPlayer *player = 0;
    switch (level)
    {
            case 0:
                player = new DumbAIPlayer (side);
                break;

            case 1:
                player = new SimpleAIPlayer (side, SimpleAIPlayer::AggressivityNone);
                break;

            case 2:
                player = new SimpleAIPlayer (side, SimpleAIPlayer::AggressivityFull);
                break;

            case 3:
                player = new AnticipatoryAIPlayer (side, AnticipatoryAIPlayer::AnticipateNextTwo);
                break;

            case 4:
                player = new AnticipatoryAIPlayer (side, AnticipatoryAIPlayer::AnticipateNextOne);
                break;

            case 5:
                player = new AdvancedAIPlayer (side);
                break;

            default:
                assert (false && "AI player level not valid!");
                break;
    }
    return player;
}

///
/// \brief Gets the player's name based on the opponent's level.
///
/// \param level The opponent's level.
/// \return The player's name.
///
std::string
AIPlayerFactory::getPlayerName (uint8_t level)
{
    std::string name ("");
    switch (level)
    {
        case 0:
            name = "K.Quita";
            break;

        case 1:
            name = "Angus";
            break;

        case 2:
            name = "Kerberos";
            break;

        case 3:
            name = "Spike";
            break;

        case 4:
            name = "Mr.Bones";
            break;

        case 5:
            name = "Pen";
            break;

        default:
            assert (false && "Tried to use an invalid oppoent's level.");
            break;
    }

    return name;
}

///
/// \brief Gets the background file name to use depending on the opponent's level.
///
/// \param level The opponent's level.
/// \return The file name to use to open the background as an imatge.
///
std::string
AIPlayerFactory::getBackgroundFileName (uint8_t level)
{
    return AIPlayerFactory::getPlayerName (level) + "Background.png";
}

///
/// \brief Gets the background file name to use at random.
///
/// \return The file name to use to open the background as an imatge.
///
std::string
AIPlayerFactory::getRandomBackgroundFileName (void)
{
    return getBackgroundFileName (rand () % k_MaxPlayerLevel);
}


///
/// \brief Gets the winner of an hypotetical match between to AI players.
///
/// \param leftLevel The level of the left AI player.
/// \param rightLevel The level of the right AI player.
/// \return The winner side in case these two players fought in a match.
///
IPlayer::PlayerSide
AIPlayerFactory::getWinner (uint8_t leftLevel, uint8_t rightLevel)
{
    uint8_t probabilityLeftPlayerWins = 0;

    // If both players have the same level, then both have the same
    // probability to win.
    if ( leftLevel == rightLevel )
    {
        probabilityLeftPlayerWins = 50;
    }
    else
    {
        switch (leftLevel)
        {
            case 0:
                switch (rightLevel)
                {
                    case 1:
                        probabilityLeftPlayerWins = 15;
                        break;

                    case 2:
                        probabilityLeftPlayerWins = 12;
                        break;

                    case 3:
                        probabilityLeftPlayerWins = 9;
                        break;

                    case 4:
                        probabilityLeftPlayerWins = 6;
                        break;

                    case 5:
                        probabilityLeftPlayerWins = 3;
                        break;

                    default:
                        probabilityLeftPlayerWins = 50;
                        break;
                }
                break;

            case 1:
                switch (rightLevel)
                {
                    case 0:
                        probabilityLeftPlayerWins = 85;
                        break;

                    case 2:
                        probabilityLeftPlayerWins = 32;
                        break;

                    case 3:
                        probabilityLeftPlayerWins = 29;
                        break;

                    case 4:
                        probabilityLeftPlayerWins = 25;
                        break;

                    case 5:
                        probabilityLeftPlayerWins = 20;
                        break;

                    default:
                        probabilityLeftPlayerWins = 14;
                        break;
                }
                break;

            case 2:
                switch (rightLevel)
                {
                    case 0:
                        probabilityLeftPlayerWins = 88;
                        break;

                    case 1:
                        probabilityLeftPlayerWins = 68;
                        break;

                    case 3:
                        probabilityLeftPlayerWins = 34;
                        break;

                    case 4:
                        probabilityLeftPlayerWins = 29;
                        break;

                    case 5:
                        probabilityLeftPlayerWins = 23;
                        break;

                    default:
                        probabilityLeftPlayerWins = 50;
                        break;
                }
                break;

            case 3:
                switch (rightLevel)
                {
                    case 0:
                        probabilityLeftPlayerWins = 91;
                        break;

                    case 1:
                        probabilityLeftPlayerWins = 71;
                        break;

                    case 2:
                        probabilityLeftPlayerWins = 66;
                        break;

                    case 4:
                        probabilityLeftPlayerWins = 31;
                        break;

                    case 5:
                        probabilityLeftPlayerWins = 26;
                        break;

                    default:
                        probabilityLeftPlayerWins = 50;
                        break;
                }
                break;

            case 4:
                switch (rightLevel)
                {
                    case 0:
                        probabilityLeftPlayerWins = 94;
                        break;

                    case 1:
                        probabilityLeftPlayerWins = 75;
                        break;

                    case 2:
                        probabilityLeftPlayerWins = 71;
                        break;

                    case 3:
                        probabilityLeftPlayerWins = 69;
                        break;

                    case 5:
                        probabilityLeftPlayerWins = 35;
                        break;

                    default:
                        probabilityLeftPlayerWins = 50;
                        break;
                }
                break;

            case 5:
                switch (rightLevel)
                {
                    case 0:
                        probabilityLeftPlayerWins = 97;
                        break;

                    case 1:
                        probabilityLeftPlayerWins = 80;
                        break;

                    case 2:
                        probabilityLeftPlayerWins = 77;
                        break;

                    case 3:
                        probabilityLeftPlayerWins = 74;
                        break;

                    case 4:
                        probabilityLeftPlayerWins = 65;
                        break;

                    default:
                        probabilityLeftPlayerWins = 50;
                        break;
                }
                break;

            default:
                probabilityLeftPlayerWins = 50;
                break;
        }
    }


    return ((rand () % 100) < probabilityLeftPlayerWins) ? IPlayer::LeftSide :
                                                           IPlayer::RightSide;
}
