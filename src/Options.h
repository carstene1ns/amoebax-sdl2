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
#if !defined (AMOEBAX_OPTIONS_H)
#define AMOEBAX_OPTIONS_H

#include <list>
#include <string>
#include "IPlayer.h"

namespace Amoebax
{
    /// A single high score.
    typedef std::pair<uint32_t, std::string> HighScore;

    ///
    /// \class Options
    /// \brief Loads and saves the game options.
    ///
    class Options
    {
        public:
            ///
            /// \enum ControlsType
            /// \brief The type of controls a player is using.
            ///
            enum ControlsType
            {
                /// The player is using the keyboard.
                KeyboardControls,
                /// The player is using the joystick.
                JoystickControls
            };

            ///
            /// \struct PlayerControls
            /// \brief The controls options for a single player.
            ///
            struct PlayerControls
            {
                /// The type of controls a player is using.
                ControlsType controlsType;
                /// The joystick's settings.
                struct
                {
                    /// The joystick's index.
                    uint8_t index;
                    /// The key to move the falling pair left.
                    int16_t moveLeft;
                    /// The key to move the falling pair right.
                    int16_t moveRight;
                    /// The key to push the falling pair down.
                    int16_t pushDown;
                    /// The key to rotate the falling pair clockwise.
                    int16_t rotateClockwise;
                    /// The key to rotate the falling pair counter-clockwise.
                    int16_t rotateCounterClockwise;
                } joystick;

                /// The keyboard's settings.
                struct
                {
                    /// The key to move the falling pair left.
                    uint32_t moveLeft;
                    /// The key to move the falling pair right.
                    uint32_t moveRight;
                    /// The key to push the falling pair down.
                    uint32_t pushDown;
                    /// The key to rotate the falling pair clockwise.
                    uint32_t rotateClockwise;
                    /// The key to rotate the falling pair counter-clockwise.
                    uint32_t rotateCounterClockwise;
                } keyboard;
            };

            Options (void);
            virtual ~Options (void);

            void decrementVolume (void);
            std::list<HighScore> &getHighScoreList(void);
            static Options &getInstance (void);
            static unsigned int getMaxVolumeLevel (void);
            PlayerControls getPlayerControls (IPlayer::PlayerSide player);
            unsigned int getScreenDepth (void);
            unsigned int getScreenHeight (void);
            unsigned int getScreenWidth (void);
            unsigned int getVolumeLevel (void);
            void incrementVolume (void);
            bool isFullScreen (void);
            bool isHighScore (uint32_t score);
            bool isSoundEnabled (void);
            bool isVideoOptionsAtDefault (void);
            void setDefaultVideoOptions (void);
            void setDefaultPlayersControls (void);
            void setFullScreen (bool fullScreen);
            void setHighScore (uint32_t score, const std::string &name);
            void setPlayerControls (IPlayer::PlayerSide player,
                                    const PlayerControls &controls);
            void setScreenDepth (unsigned int depth);
            void setScreenHeight (unsigned int height);
            void setScreenWidth (unsigned int width);
            void setSoundEnabled (bool enabled);

        protected:
            ///
            /// \brief Gets a boolean option value.
            ///
            /// \param section The section where the value is located at.
            /// \param name The name of the value to get.
            /// \param defaultValue The value to return in case the optiona
            ///                     \a name doesn't exist in \a section.
            /// \return The option value \a name in \a section, or
            ///         \a defaultValue if \a name doesn't exist.
            ///
            virtual bool getBooleanValue (const std::string &section,
                                          const std::string &name,
                                          bool defaultValue) = 0;
            ///
            /// \brief Gets an integer option value.
            ///
            /// \param section The section where the value is locate at.
            /// \param name The name of the value to get.
            /// \param defaultValue The value to return in case the
            ///                     name \a name doesn't exist in
            ///                     section \a section.
            /// return The value named \a named in section \a section, or
            ///        \a defaultValue if the name doesn't exist.
            ///
            virtual int getIntegerValue (const std::string &section,
                                         const std::string &name,
                                         int defaultValue) = 0;
            ///
            /// \brief Gets an string option value.
            ///
            /// \param section The section where the value is locate at.
            /// \param name The name of the value to get.
            /// \param defaultValue The value to return in case the
            ///                     name \a name doesn't exist in
            ///                     section \a section.
            /// return The value named \a named in section \a section, or
            ///        \a defaultValue if the name doesn't exist.
            ///
            virtual std::string getStringValue (const std::string &section,
                                                const std::string &name,
                                                const std::string &defaultValue) = 0;

            ///
            /// \brief Sets a boolean option value.
            ///
            /// \param section The section where to write the value to.
            /// \param name The name to use to save the value with.
            /// \param value The boolean value to save as \a name in \a section.
            ///
            virtual void setBooleanValue (const std::string &section,
                                          const std::string &name,
                                          bool value) = 0;
            ///
            /// \brief Sets an integer option value.
            ///
            /// \param section The section where to write the value to.
            /// \param name The name to use to save the value with.
            /// \param value The integer value to save as \a name in section
            ///              \a section.
            ///
            virtual void setIntegerValue (const std::string &section,
                                          const std::string &name,
                                          int value) = 0;
            ///
            /// \brief Sets an string option value.
            ///
            /// \param section The section where to write the value to.
            /// \param name The name to use to save the value with.
            /// \param value The integer value to save as \a name in section
            ///              \a section.
            ///
            virtual void setStringValue (const std::string &section,
                                         const std::string &name,
                                         const std::string &value) = 0;

        private:
            // Prevent copies of Options classes.
            ///
            /// \brief Copy constructor.
            ///
            /// \note This constructor is left unimplemented because we don't
            /// want copies of Options objects. Don't use it.
            ///
            Options (const Options &);

            ///
            /// \brief Assigment operator.
            ///
            /// \note This operator is left unimplemented because we don't
            /// want copies of Options objects. Don't use it.
            ///
            Options &operator= (const Options &);

            std::list<HighScore>::iterator findScorePosition (uint32_t score);
            void loadHighScoreList (void);
            void saveHighScoreList (void);

            /// The list of high scores.
            std::list<HighScore> m_HighScore;
    };
}

#endif // !AMOEBAX_OPTIONS_H
