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
#include <SDL.h>
#include <sstream>
#include "Options.h"

#if defined (IS_OSX_HOST)
#include "OSXOptions.h"
#elif defined (IS_WIN32_HOST)
#include "Win32Options.h"
#else // IS_OSX_HOST
#include "UnixOptions.h"
#endif // !IS_OSX_PATH

using namespace Amoebax;

// Default player's controls
static const uint32_t k_PlayersControls[2][5] =
{
    // Left player
    {
        SDLK_a,                 // Move left.
        SDLK_d,                 // Move right.
        SDLK_s,                 // Push down.
        SDLK_w,                 // Rotate clockwise.
        SDLK_LCTRL              // Rotate counter-clockwise
    },

    // Right player.
    {
        SDLK_LEFT,              // Move left.
        SDLK_RIGHT,             // Move right.
        SDLK_DOWN,              // Push down.
        SDLK_UP,                // Rotate clockwise.
        SDLK_RCTRL              // Rotate counter-clockwise.
    }
};
// Player's names.
static std::string k_PlayersName[2] =
{
    std::string ("leftPlayer"),
    std::string ("rightPlayer")
};
// Default screen's bits per pixel.
static const unsigned int k_ScreenDepth = 0;
// Default screen's height.
static const unsigned int k_ScreenHeight = 600;
// Default screen's width.
static const unsigned int k_ScreenWidth = 800;
// Default volume level.
static const int k_VolumeLevel = 5;
// Maximum volume level.
static const int k_VolumeLevelMax = 10;

///
/// \brief Default Options constructor.
///
Options::Options (void)
{
}

///
/// \brief Options destructor.
///
Options::~Options (void)
{
}

///
/// \brief Decrement the current volume level.
///
/// If the current volume level is already 0, this function does nothing.
///
void
Options::decrementVolume (void)
{
    uint8_t currentVolumeLevel = getVolumeLevel ();
    if ( currentVolumeLevel > 0 )
    {
        setIntegerValue ("sound", "volume", currentVolumeLevel - 1);
    }
}


///
/// \brief Finds the position of an score in the list of high scores.
///
/// \param score The score to find its position in the list.
/// \return The position that the score should be in the list.
///
std::list<HighScore>::iterator
Options::findScorePosition (uint32_t score)
{
    std::list<HighScore>::iterator currentScore;
    for (currentScore = m_HighScore.begin();
         currentScore != m_HighScore.end();
         ++currentScore)
    {
        if ( score > currentScore->first )
        {
            break;
        }
    }

    return currentScore;
}

///
/// \brief Gets the high score list.
///
/// \return The list of high score.
///
std::list<HighScore> &
Options::getHighScoreList (void)
{
    return m_HighScore;
}

///
/// \brief Gets the only available instace of Options.
///
/// \return The only instance of options.
///
Options &
Options::getInstance (void)
{
#if defined (IS_OSX_HOST)
    static OSXOptions options;
#elif defined (IS_WIN32_HOST)
    static Win32Options options;
#else // IS_OSX_HOST
    static UnixOptions options;
#endif // !IS_OSX_HOST
    static bool firstCall = true;
    if ( firstCall )
    {
        options.loadHighScoreList ();
        firstCall = false;
    }
    return options;
}

///
/// \brief Gets the maximum volume level.
///
/// \return The value of the maximum sound volume level.
///
unsigned int
Options::getMaxVolumeLevel (void)
{
    return k_VolumeLevelMax;
}

///
/// \brief Gets the player's controls.
///
/// \param player The player to get the controls from.
/// \return The player's controls.
///
Options::PlayerControls
Options::getPlayerControls (IPlayer::PlayerSide player)
{
    const int playerIndex = static_cast<int> (player);
    Options::PlayerControls playerControls;
    std::string sectionName (k_PlayersName[playerIndex] + "-controls");
    playerControls.controlsType = static_cast<ControlsType> (
        getIntegerValue (sectionName, "type",
                         static_cast<int> (KeyboardControls)));

    playerControls.keyboard.moveLeft =
        getIntegerValue (sectionName, "KeyMoveLeft",
                         k_PlayersControls[playerIndex][0]);
    playerControls.keyboard.moveRight =
        getIntegerValue (sectionName, "KeyMoveRight",
                         k_PlayersControls[playerIndex][1]);
    playerControls.keyboard.pushDown =
        getIntegerValue (sectionName, "KeyPushDown",
                         k_PlayersControls[playerIndex][2]);
    playerControls.keyboard.rotateClockwise =
        getIntegerValue (sectionName, "KeyRotateCW",
                         k_PlayersControls[playerIndex][3]);
    playerControls.keyboard.rotateCounterClockwise =
        getIntegerValue (sectionName, "KeyRotateCCW",
                         k_PlayersControls[playerIndex][4]);

    playerControls.joystick.index =
        getIntegerValue (sectionName, "JoyIndex", 0);
    playerControls.joystick.moveLeft =
        getIntegerValue (sectionName, "JoyMoveLeft", -1);
    playerControls.joystick.moveRight =
        getIntegerValue (sectionName, "JoyMoveRight", 1);
    playerControls.joystick.pushDown =
        getIntegerValue (sectionName, "JoyPushDown", 2);
    playerControls.joystick.rotateClockwise =
        getIntegerValue (sectionName, "JoyRotateCW", 0);
    playerControls.joystick.rotateCounterClockwise =
        getIntegerValue (sectionName, "JoyRotateCCW", 1);


    return playerControls;
}

///
/// \brief Gets the screen depth to use.
///
/// \return The depth to use when setting the screen resolution.
///
unsigned int
Options::getScreenDepth (void)
{
#if defined (IS_GP2X_HOST)
    return 16;
#else   // IS_GP2X_HOST
    return getIntegerValue ("screen", "depth", k_ScreenDepth);
#endif  // !IS_GP2X_HOST
}

///
/// \brief Gets the screen height to use.
///
/// \return The height to use when setting the screen resolution.
///
unsigned int
Options::getScreenHeight (void)
{
#if defined (IS_GP2X_HOST)
    return 240;
#else // IS_GP2X_HOST
    return getIntegerValue ("screen", "height", k_ScreenHeight);
#endif // !IS_GP2X_HOST
}

///
/// \brief Gets the screen width to use.
///
/// \return The width to use when setting the screen resolution.
///
unsigned int
Options::getScreenWidth (void)
{
#if defined (IS_GP2X_HOST)
    return 320;
#else // IS_GP2X_HOST
    return getIntegerValue ("screen", "width", k_ScreenWidth);
#endif // !IS_GP2X_HOST
}

///
/// \brief Gets the current volume level.
///
/// \return The current level of the volume, from 0 to getMaxVolumeLevel().
///
unsigned int
Options::getVolumeLevel (void)
{
    return getIntegerValue ("sound", "volume", k_VolumeLevel);
}

///
/// \brief Incrememts the current volume level.
///
/// If the current level is already at max. value, this
/// function does nothing.
///
void
Options::incrementVolume (void)
{
    uint8_t currentVolumeLevel = getVolumeLevel ();
    if ( currentVolumeLevel < getMaxVolumeLevel () )
    {
        setIntegerValue ("sound", "volume", currentVolumeLevel + 1);
    }
}

///
/// \brief Tells if the screen is configured in full screen mode.
///
/// \return \a true if the video should be set in full screen mode,
///         \a false otherwise.
///
bool
Options::isFullScreen (void)
{
#if defined (IS_GP2X_HOST)
    return true;
#else // IS_GP2X_HOST
    return getBooleanValue ("screen", "fullscreen", false);
#endif // !IS_GP2X_HOST
}

///
/// \brief Tells if the score is a high score.
///
/// \param score The score to check if it is high score.
/// \return true if the score is a high score, otherwise false.
///
bool
Options::isHighScore (uint32_t score)
{
    return findScorePosition (score) != m_HighScore.end();
}

///
/// \brief Tells if the sound is enabled.
///
/// \return \a true if the sound should be enabled, \a false otherwise.
///
bool
Options::isSoundEnabled (void)
{
    return getBooleanValue ("sound", "enabled", true);
}

///
/// \brief Tells if all video options are at they default value.
///
/// \return \a true if all video options (width, height, and depth) are
///         at their default values.  \a false otherwise.
///
bool
Options::isVideoOptionsAtDefault (void)
{
    return !isFullScreen () &&
           k_ScreenDepth == getScreenDepth () &&
           k_ScreenHeight == getScreenHeight () &&
           k_ScreenWidth == getScreenWidth ();
}

///
/// \brief Loads the list of high score.
///
void
Options::loadHighScoreList (void)
{
    {
        HighScore highScore;
        highScore.first = getIntegerValue ("highscore", "0.score", 500);
        highScore.second = getStringValue ("highscore", "0.name", "JACK");
        m_HighScore.push_back (highScore);
    }
    {
        HighScore highScore;
        highScore.first = getIntegerValue ("highscore", "1.score", 400);
        highScore.second = getStringValue ("highscore", "1.name", "MICHAEL");
        m_HighScore.push_back (highScore);
    }
    {
        HighScore highScore;
        highScore.first = getIntegerValue ("highscore", "2.score", 300);
        highScore.second = getStringValue ("highscore", "2.name", "JANE");
        m_HighScore.push_back (highScore);
    }
    {
        HighScore highScore;
        highScore.first = getIntegerValue ("highscore", "3.score", 200);
        highScore.second = getStringValue ("highscore", "3.name", "JOHN");
        m_HighScore.push_back (highScore);
    }
    {
        HighScore highScore;
        highScore.first = getIntegerValue ("highscore", "4.score", 100);
        highScore.second = getStringValue ("highscore", "4.name", "MICHAELLE");
        m_HighScore.push_back (highScore);
    }
}

///
/// \brief Save the high score list.
///
void
Options::saveHighScoreList (void)
{
    std::list<HighScore>::iterator currentScore;
    uint16_t currentScoreIndex;
    for ( currentScoreIndex = 0, currentScore = m_HighScore.begin ();
          currentScore != m_HighScore.end ();
          ++currentScoreIndex, ++currentScore )
    {
        std::stringstream scoreSection;
        scoreSection << currentScoreIndex << ".score";
        setIntegerValue ("highscore", scoreSection.str (), currentScore->first);

        std::stringstream nameSection;
        nameSection << currentScoreIndex << ".name";
        setStringValue ("highscore", nameSection.str (), currentScore->second);
    }
}

///
/// \brief Sets the video options to their default value.
///
void
Options::setDefaultVideoOptions (void)
{
    setFullScreen (false);
    setScreenDepth (k_ScreenDepth);
    setScreenHeight (k_ScreenHeight);
    setScreenWidth (k_ScreenWidth);
}

///
/// \brief Sets the defaults controls for the players.
///
void
Options::setDefaultPlayersControls (void)
{
    PlayerControls controls;

    controls.controlsType = KeyboardControls;
    controls.joystick.index = 0;
    controls.joystick.moveLeft = -1;
    controls.joystick.moveRight = 1;
    controls.joystick.pushDown = 2;
    controls.joystick.rotateClockwise = 0;
    controls.joystick.rotateCounterClockwise = 1;

    controls.keyboard.moveLeft = k_PlayersControls[0][0];
    controls.keyboard.moveRight = k_PlayersControls[0][1];
    controls.keyboard.pushDown = k_PlayersControls[0][2];
    controls.keyboard.rotateClockwise = k_PlayersControls[0][3];
    controls.keyboard.rotateCounterClockwise = k_PlayersControls[0][4];
    setPlayerControls (IPlayer::LeftSide, controls);

    controls.keyboard.moveLeft = k_PlayersControls[1][0];
    controls.keyboard.moveRight = k_PlayersControls[1][1];
    controls.keyboard.pushDown = k_PlayersControls[1][2];
    controls.keyboard.rotateClockwise = k_PlayersControls[1][3];
    controls.keyboard.rotateCounterClockwise = k_PlayersControls[1][4];
    setPlayerControls (IPlayer::RightSide, controls);
}

///
/// \brief Sets the full screen mode.
///
/// \param fullScreen If \a true, the video should be set in full screen mode,
///                   otherwise it should be set to windowed mode.
///
void
Options::setFullScreen (bool fullScreen)
{
    setBooleanValue ("screen", "fullscreen", fullScreen);
}

///
/// \brief Sets a new high score.
///
/// \param score The score to set a new high score.
/// \param name The name to set a new name's high score.
///
void
Options::setHighScore (uint32_t score, const std::string &name)
{
    std::list<HighScore>::iterator currentScore = findScorePosition (score);
    if (currentScore != m_HighScore.end ())
    {
        HighScore highScore;
        highScore.first = score;
        highScore.second = name;
        m_HighScore.insert (currentScore, highScore);
        currentScore = m_HighScore.end ();
        --currentScore;
        m_HighScore.erase (currentScore);
        saveHighScoreList ();
    }
}

///
/// \brief Sets the player's controls.
///
/// \param player The player to set the controls to.
/// \param controls The player's controls to set.
///
void
Options::setPlayerControls (IPlayer::PlayerSide player,
                            const Options::PlayerControls &controls)
{
    const int playerIndex = static_cast<int> (player);
    std::string sectionName (k_PlayersName[playerIndex] + "-controls");

    setIntegerValue (sectionName, "type", static_cast<int> (controls.controlsType));
    setIntegerValue (sectionName, "KeyMoveLeft", controls.keyboard.moveLeft);
    setIntegerValue (sectionName, "KeyMoveRight", controls.keyboard.moveRight);
    setIntegerValue (sectionName, "KeyPushDown", controls.keyboard.pushDown);
    setIntegerValue (sectionName, "KeyRotateCW", controls.keyboard.rotateClockwise);
    setIntegerValue (sectionName, "KeyRotateCCW", controls.keyboard.rotateCounterClockwise);
    setIntegerValue (sectionName, "JoyIndex", controls.joystick.index);
    setIntegerValue (sectionName, "JoyMoveLeft", controls.joystick.moveLeft);
    setIntegerValue (sectionName, "JoyMoveRight", controls.joystick.moveRight);
    setIntegerValue (sectionName, "JoyPushDown", controls.joystick.pushDown);
    setIntegerValue (sectionName, "JoyRotateCW", controls.joystick.rotateClockwise);
    setIntegerValue (sectionName, "JoyRotateCCW", controls.joystick.rotateCounterClockwise);
}


///
/// \brief Sets the screen depth to use.
///
/// \param depth The depth to use when setting the screen resolution.
///
void
Options::setScreenDepth (unsigned int depth)
{
    setIntegerValue ("screen", "depth", depth);
}

///
/// \brief Sets the screen height to use.
///
/// \param height The height to use when setting the screen resolution.
///
void
Options::setScreenHeight (unsigned int height)
{
    setIntegerValue ("screen", "height", height);
}

///
/// \brief Sets the screen width to use.
///
/// \param width The width to use when setting the screen resolution.
///
void
Options::setScreenWidth (unsigned int width)
{
    setIntegerValue ("screen", "width", width);
}

///
/// \brief Enables or disables the sound subsystem.
///
/// \param enabled \a true if the sound subsystem should be enabled.
///                \a false otherwise.
///
void
Options::setSoundEnabled (bool enabled)
{
    setBooleanValue ("sound", "enabled", enabled);
}
