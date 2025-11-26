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
#if !defined (AMOEBAX_TOURNAMENT_STATE_H)
#define AMOEBAX_TOURNAMENT_STATE_H

#include "IMatchObserver.h"
#include "IState.h"
#include "IPlayer.h"

namespace Amoebax
{

    ///
    /// \class TournamentState.
    /// \brief Tournament mode.
    ///
    class TournamentState: public IState, public IMatchObserver
    {
        public:
            ///
            /// \struct Player
            /// \brief A player of the tournament.
            ///
            struct Player
            {
                /// The name of the player's character.
                std::string name;
                /// The index of the player's characer.
                uint8_t characterIndex;
                /// Tells the computer level, if isComputerPlayer is \a true.
                uint8_t computerPlayerLevel;
                /// Tells if the players is computer controlled or not.
                bool isComputerPlayer;
                /// The player's score.
                uint32_t score;
            };

            TournamentState (const std::vector <Player> &players);
            ~TournamentState (void);

            virtual void activate (void);
            virtual void endOfMatch (IPlayer::PlayerSide winner,
                                     uint32_t leftPlayerScore,
                                     uint32_t rightPlayerScore);
            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value);
            virtual void joyDown (uint8_t joystick, uint8_t button);
            virtual void joyUp (uint8_t joystick, uint8_t button);
#if !defined (IS_GP2X_HOST)
            virtual void keyDown (uint32_t key);
            virtual void keyUp (uint32_t key);
#endif // !IS_GP2X_HOST
            virtual void redrawBackground (SDL_Rect *region, SDL_Surface *screen);
            virtual void render (SDL_Surface *screen);
            virtual bool shouldBePaused (void) { return true; }
            virtual void update (uint32_t elapsedTime);
            virtual void videoModeChanged (void);

        private:
            ///
            /// \struct Match
            /// \brief A match between two players.
            ///
            struct Match
            {
                /// The parent match of this match.  Used to level up the winner.
                Match *parent;
                /// The match's children.  To traverse the tree.
                std::pair<Match *, Match *> children;
                /// The players that "fight" in this match.
                std::pair<Player, Player> opponents;
            };

            void createMatchTree (const std::vector<Player> &players);
            void deleteMatch (Match *match);
            IState *getBestTwoPlayersState (const Player &leftCharacter,
                                            const Player &rightCharacter);
            Match *getCurrentMatch (void);
            int32_t getCurrentMatchBlinkTime (void) const;
            int32_t getCurrentMatchStartTime (void) const;
            IPlayer *getMatchPlayer (const Player &player,
                                     IPlayer::PlayerSide side) const;
            void loadGraphicResources (void);
            void renderMatch (Match *match, SDL_Surface *screen,
                              uint16_t x, uint8_t level = 0);
            bool showCurrentOpponents (void) const;
            void setCurrentMatch (Match *match);
            void setCurrentMatchBlinkTime (int32_t blinkTime);
            void setCurrentMatchStartTime (int32_t startTime);
            void setShowCurrentOpponents (bool showCurrentOpponents);
            void startCurrentMatch (void);

            /// The current match.
            Match *m_CurrentMatch;
            /// The background.
            std::auto_ptr<Surface> m_Background;
            /// The background music.
            std::auto_ptr<Music> m_BackgroundMusic;
            /// The time for toggling the show current match.
            int32_t m_CurrentMatchBlinkTime;
            /// The time for starting the current match.
            int32_t m_CurrentMatchStartTime;
            /// The player's faces.
            std::auto_ptr<Surface> m_Faces;
            /// Horizontal offset of players.
            std::vector<uint16_t> m_HorizontalOffset;
            /// The matches.
            Match m_Matches;
            /// The number of players.
            size_t m_NumPlayers;
            /// Tells if show the current match's opponents.
            bool m_ShowCurrentOpponents;
            /// The sound of when you lose.
            std::auto_ptr<Sound> m_SoundLose;
            /// The sound of when you win.
            std::auto_ptr<Sound> m_SoundWin;
            /// Vertical position of players.
            std::vector<uint16_t> m_VerticalPosition;
    };
}

#endif // !AMOEBAX_TOURNAMENT_STATE_H
