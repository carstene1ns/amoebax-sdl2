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
#include <algorithm>
#include <cassert>
#include <SDL.h>
#include <sstream>
#include "Amoeba.h"
#include "DrawAmoeba.h"
#include "DrawChainLabel.h"
#include "File.h"
#include "HumanPlayer.h"
#include "NewHighScoreState.h"
#include "Options.h"
#include "System.h"
#include "TrainingState.h"

using namespace Amoebax;

/// The speed in which the "Level Up!" label goes up (px. / 100ms.)
static const float k_LevelUpSpeed = 3.0f / 100.0f;
/// The size of the silhouettes' border in pixels.
static const uint8_t k_SilhouetteBorder = 8;

namespace Amoebax
{
    ///
    /// \struct Level
    /// \brief Definition of training levels.
    ///
    const static struct Level
    {
        /// The minimum score points to have to be in this level.
        uint32_t minScore;
        /// The probability to add ghost amoebas each time (in %).
        uint8_t probabilityToAdd;
        /// The max number of ghost amobeas that can be added each time (0-30).
        uint8_t maxAmoebas;
    }
    /// The possible levels and the score to have to "level up!"
    k_Levels[] =
    {
        {    0,  0, 0},
        {  440, 20, 1},
        { 1240, 25, 1},
        { 2200, 30, 1},
        { 3712, 30, 2},
        { 5896, 40, 1},
        { 8080, 40, 2},
        {10000, 45, 1},
        {10500, 30, 3},
        {13000, 50, 2},
        {16000, 35, 4},
        {18500, 40, 4},
        {21000, 45, 4},
        {23500, 40, 5},
        {26000, 60, 6}
    };
}
const static uint8_t k_NumLevels = sizeof (k_Levels) / sizeof (k_Levels[0]);

///
/// \brief Default constructor.
///
TrainingState::TrainingState (void):
    IState (),
    m_Amoebas (nullptr),
    m_AmoebasSize (k_MaxAmoebasSize),
    m_Background (nullptr),
    m_BackgroundMusic (Music::fromFile (File::getMusicFilePath ("training.ogg"))),
    m_ChainLabel (nullptr),
    m_CurrentLevel (1),
    m_GameOver (nullptr),
    m_Generator (new PairGenerator ()),
    m_Go (nullptr),
    m_GoTime (k_LevelUpTime / 4),
    m_LevelUp (nullptr),
    m_LevelUpPosition (0.0f),
    m_LevelUpSound (Sound::fromFile (File::getSoundFilePath ("levelup.wav"))),
    m_LevelUpTime (0),
    m_Player (new HumanPlayer (IPlayer::RightSide)),
    m_Ready (nullptr),
    m_ReadyTime (k_LevelUpTime / 2),
    m_ScoreFont (nullptr),
    m_SilhouetteBorder (k_SilhouetteBorder),
    m_Silhouettes (nullptr),
    m_SoundLose (Sound::fromFile (File::getSoundFilePath ("youlose.wav")))
{
    loadGraphicResources ();

    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    getPlayer ()->setGrid (
            new Grid (static_cast<uint16_t>(k_PositionXGrid * screenScale),
                      static_cast<uint16_t>(k_PositionYGrid * screenScale),
                      static_cast<uint16_t>(k_PositionXQueue * screenScale),
                      static_cast<uint16_t>(k_PositionYQueue * screenScale),
                      static_cast<uint16_t>(k_PositionXWaiting * screenScale),
                      static_cast<uint16_t>(k_PositionYWaiting * screenScale),
                      getAmoebasSize (), Grid::QueueSideRight));

    // Create the pair generator, add the grids and then generate 4 pair.
    m_Generator->addGrid (getPlayerGrid ());
    m_Generator->generate (4);
}

void
TrainingState::activate (void)
{
    getPlayer ()->loadOptions ();
    if ( Music::isPaused () )
    {
        Music::resume ();
    }
    else
    {
        m_BackgroundMusic->play ();
    }
}

///
/// \brief Adds ghost amoebas to the grid.
///
/// Based on the current level and its constrains, this
/// function adds at random ghost amoebas at the grid.
///
void
TrainingState::addGhostAmoebas (void)
{
    const Level &currentLevel = k_Levels[getCurrentLevel () - 1];
    if ( currentLevel.maxAmoebas != 0 &&
         (rand () % 101) < currentLevel.probabilityToAdd )
    {
        getPlayerGrid ()->incrementNumberOfWaitingGhosts ((rand () %
                                                 currentLevel.maxAmoebas) + 1);
    }
}

///
/// \brief Checks if the current level should be changed.
///
void
TrainingState::checkLevelChange (void)
{
    // If we are not at the last level, and the current grid's score
    // passes the minimum score for the next level, then incrememnt
    // the current level and prepare the label.
    bool levelChanged = false;
    if ( getCurrentLevel () < k_NumLevels )
    {
        while ( getCurrentLevel () < k_NumLevels &&
                getPlayerGrid ()->getScore () > k_Levels[getCurrentLevel ()].minScore )
        {
            incrementCurrentLevel ();
            levelChanged = true;
        }

        if ( levelChanged )
        {
            m_LevelUpSound->play ();
            m_LevelUpPosition = Options::getInstance ().getScreenHeight () / 2;
            m_LevelUpTime = k_LevelUpTime;
        }
    }
}

///
/// \brief Gets the size of an amoeba.
///
/// \return The size (the same for width and height) of an amoeba.
///
inline uint8_t
TrainingState::getAmoebasSize (void) const
{
    return m_AmoebasSize;
}

///
/// \brief Gets the current training's level.
///
/// \return The current level the player is training.
///
inline uint16_t
TrainingState::getCurrentLevel (void) const
{
    return m_CurrentLevel;
}

///
/// \brief Gets the time to display the "Go!" label.
///
/// \return The time that the "Go!" label should be visible.
///
inline int32_t
TrainingState::getGoTime (void) const
{
    return m_GoTime;
}

///
/// \brief Gets the training player.
///
/// \return The training's player.
///
inline IPlayer *
TrainingState::getPlayer (void) const
{
    return m_Player.get ();
}

///
/// \brief Gets the player's grid.
///
/// \return The player's grid.
///
inline Grid *
TrainingState::getPlayerGrid (void) const
{
    IPlayer *player = getPlayer ();
    assert ( 0 != player && "There's no player to get the grid from." );
    return player->getGrid ();
}

///
/// \brief Gets the time to display the "Ready?" label.
///
/// \return The time that the "Ready?" label should be visible.
///
inline int32_t
TrainingState::getReadyTime (void) const
{
    return m_ReadyTime;
}

///
/// \brief Gets the size of the silhouettes' border.
///
/// \return The size of the silhouettes's border.
///
inline uint8_t
TrainingState::getSilhouetteBorder (void) const
{
    return m_SilhouetteBorder;
}

///
/// \brief Increments the current level.
///
/// \param amount The amount to increment the current level.
///
inline void
TrainingState::incrementCurrentLevel (uint8_t amount)
{
    setCurrentLevel (getCurrentLevel () + amount);
}

void
TrainingState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
    getPlayer ()->joyMotion (joystick, axis, value);
}

void
TrainingState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    switch (button)
    {
        case SDL_CONTROLLER_BUTTON_START:
        case SDL_CONTROLLER_BUTTON_B:
            pauseOrEndGame ();
        break;

        default:
            getPlayer ()->joyDown (joystick, button);
        break;
    }

    // TODO
    getPlayer ()->joyDown (joystick, button);

}

void
TrainingState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
    getPlayer ()->joyUp (joystick, button);
}

void
TrainingState::keyDown (uint32_t key)
{
    if ( SDLK_ESCAPE == key )
    {
        pauseOrEndGame ();
    }
    else if ( getPlayerGrid ()->isFilled () && SDLK_RETURN == key )
    {
        pauseOrEndGame ();
    }
    else
    {
        getPlayer ()->keyDown (key);
    }
}

void
TrainingState::keyUp (uint32_t key)
{
    getPlayer ()->keyUp (key);
}

///
/// \brief Loads all graphic resources.
///
void
TrainingState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();

    m_Amoebas.reset (
            Surface::fromFile (File::getGraphicsFilePath ("amoebas.png")));
    m_Amoebas->resize (screenScale);

    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("menuBackground.png")));
    {
        std::unique_ptr<Surface> gridBackground (
                Surface::fromFile (File::getGraphicsFilePath ("training.png")));
        gridBackground->blit (m_Background->toSDLSurface ());
    }
    m_Background->resize (screenScale);

    m_ChainLabel.reset (
            Surface::fromFile (File::getGraphicsFilePath ("chain.png")));
    m_ChainLabel->resize (screenScale);

    m_GameOver.reset (
            Surface::fromFile (File::getGraphicsFilePath ("gameover.png")));
    m_GameOver->resize (screenScale);

    m_LevelUp.reset (
            Surface::fromFile (File::getGraphicsFilePath ("levelup.png")));
    m_LevelUp->resize (screenScale);

    m_Silhouettes.reset (
            Surface::fromFile (File::getGraphicsFilePath ("silhouettes.png")));
    m_Silhouettes->resize (screenScale);

    m_ScoreFont.reset (Font::fromFile (File::getFontFilePath ("score")));

    // Load the "Go!" and "Ready?" labels only if they are going to be shown.
    if ( mustShowInitialLabels () )
    {
        m_Go.reset (Surface::fromFile (File::getGraphicsFilePath ("go.png")));
        m_Ready.reset (
                Surface::fromFile (File::getGraphicsFilePath ("ready.png")));
    }

    // Set the current amoeba's size and silhouette border size.
    setAmoebasSize (static_cast<uint8_t> (k_MaxAmoebasSize * screenScale));
    setSilhouetteBorder (static_cast<uint8_t> (screenScale *
                                               k_SilhouetteBorder));
}

///
/// \brief Tells if the "Go!!" label should be displayed.
///
/// \return \a true If the "Go!!" label should be displayed,
///         \a false otherwise.
///
inline bool
TrainingState::mustShowGoLabel (void) const
{
    return getGoTime () > 0;
}

///
/// \brief Tells if either the "Go!!" or the "Ready?" labels should be shown.
///
/// \return \a true if either label must be displayed, \a false otherwise.
///
inline bool
TrainingState::mustShowInitialLabels (void) const
{
    return mustShowGoLabel () || mustShowReadyLabel ();
}

///
/// \brief Tells if the "Ready?" label should be displayed.
///
/// \return \a true If the "Ready?" label should be displayed,
///         \a false otherwise.
///
inline bool
TrainingState::mustShowReadyLabel (void) const
{
    return getReadyTime () > 0;
}

///
/// \brief Pauses or ends the game.
///
/// If the game is not over it gets paused, otherwise return to the main menu
/// except in the case that the score is a high score. In which case the system
/// prompts the player's name before going to the main menu.
///
void
TrainingState::pauseOrEndGame (void)
{
    if ( getPlayerGrid ()->isFilled () )
    {
        System::getInstance ().removeActiveState ();
        if ( Options::getInstance ().isHighScore (getPlayerGrid ()->getScore ()) )
        {
            System::getInstance ()
                .setActiveState (new NewHighScoreState (getPlayerGrid ()->getScore ()));
        }
    }
    else
    {
        System::getInstance ().pause ();
    }
}

///
/// \brief Sets the size of the amoebas.
///
/// \param size The size of each amoeba. The size is the same for the
///             width and for the height.
///
inline void
TrainingState::setAmoebasSize (uint8_t size)
{
    m_AmoebasSize = size;
}

///
/// \brief Sets the current state.
///
/// \param level The new current state.
///
inline void
TrainingState::setCurrentLevel (uint8_t level)
{
    m_CurrentLevel = level;
}

///
/// \brief Sets the time to display the "Go!!" label.
///
/// \param time The time to set as time to show the "Go!!" label. If it's
///             negative, the "Go!!" label gets deleted.
///
inline void
TrainingState::setGoTime (int32_t time)
{
    m_GoTime = time;
    if ( 0 > time )
    {
        m_Go.reset (0);
    }
}

///
/// \brief Sets the time to show the "Ready?" label.
///
/// \param time The time to set to show the "Ready?" label. If it's negative
///             the "Ready?" label gets deleted.
///
inline void
TrainingState::setReadyTime (int32_t time)
{
    m_ReadyTime = time;
    if ( 0 > time )
    {
        m_Ready.reset (0);
    }
}

///
/// \brief Sets the size of the silhouettes' border.
///
/// \param border The size of the silhouettes' border.
///
inline void
TrainingState::setSilhouetteBorder (uint8_t border)
{
    m_SilhouetteBorder = border;
}

void
TrainingState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
TrainingState::render (SDL_Surface *screen)
{
    // Get the clip rectangle of the grid, the queued amoebas, and
    // the waiting ghost amoebas.
    SDL_Rect gridRectangle;
    gridRectangle.x = getPlayerGrid ()->getGridPositionX ();
    gridRectangle.y = getPlayerGrid ()->getGridPositionY ();
    gridRectangle.w = getPlayerGrid ()->getQueuePositionX () +
                      getAmoebasSize () - gridRectangle.x;
    gridRectangle.h = getAmoebasSize () * Grid::k_VisibleHeight;

    SDL_Rect queueRectangle;
    queueRectangle.x = getPlayerGrid ()->getQueuePositionX ();
    queueRectangle.y = getPlayerGrid ()->getQueuePositionY ();
    queueRectangle.w = getAmoebasSize ();
    queueRectangle.h = 4 * getAmoebasSize ();

    SDL_Rect waitingGhosts;
    waitingGhosts.x = getPlayerGrid ()->getWaitingGhostPositionX ();
    waitingGhosts.y = getPlayerGrid ()->getWaitingGhostPositionY ();
    waitingGhosts.w = getAmoebasSize () * Grid::k_GridWidth;
    waitingGhosts.h = getAmoebasSize ();

    // Set the clip rectangle of the grid.
    SDL_SetClipRect (screen, &gridRectangle);
    // Draw the main falling amoeba's silhouette.
    const Grid::FallingAmoeba mainAmoeba =
        getPlayerGrid ()->getFallingMainAmoeba ();
    if ( 0 != mainAmoeba.amoeba )
    {
        int8_t silhouetteFrame = getPlayerGrid ()->getSilhouetteFrame ();
        if ( 0 < silhouetteFrame )
        {
            Amoeba *amoeba = mainAmoeba.amoeba;
            uint8_t silhouetteSize = getAmoebasSize () +
                                     2 * getSilhouetteBorder ();
            m_Silhouettes->blit (silhouetteSize * silhouetteFrame,
                    silhouetteSize * amoeba->getColour (),
                    silhouetteSize, silhouetteSize,
                    amoeba->getX () - getSilhouetteBorder (),
                    amoeba->getY () - getSilhouetteBorder (),
                    screen);
        }
    }
    // Draw grid's amoebas.
    const std::list<Amoeba *> &activeAmoebas = getPlayerGrid ()->getActiveAmoebas ();
    for_each (activeAmoebas.begin (), activeAmoebas.end (),
            DrawAmoeba (getAmoebasSize (), m_Amoebas.get (), screen));

    // Set the clip rectangle for the qeued amoebas.
    SDL_SetClipRect (screen, &queueRectangle);
    const std::list<Amoeba *> &queuedAmoebas = getPlayerGrid ()->getQueuedAmoebas ();
    for_each (queuedAmoebas.begin (), queuedAmoebas.end (),
            DrawAmoeba (getAmoebasSize (), m_Amoebas.get (), screen));


    // Set the clip rectangle for the ghost amoebas.
    SDL_SetClipRect (screen, &waitingGhosts);
    // Draw ghosts amoebas.
    const std::vector<Amoeba *> &ghostAmoebas =
        getPlayerGrid ()->getWaitingGhostAmoebas ();
    for_each (ghostAmoebas.begin (), ghostAmoebas.end (),
              DrawAmoeba (getAmoebasSize (), m_Amoebas.get (), screen));

    // Remove the clipping area.
    SDL_SetClipRect (screen, 0);

    // Draw chain labels.
    const std::list<ChainLabel *> &chainLabels = getPlayerGrid ()->getChainLabels ();
    for_each (chainLabels.begin (), chainLabels.end (),
              DrawChainLabel (m_ChainLabel.get (), m_ScoreFont.get (), screen));

    // Draw the current score and the current level, both right aligned to
    // its position.
    const float scaleFactor = System::getInstance ().getScreenScaleFactor ();

    std::stringstream scoreString;
    scoreString << getPlayerGrid ()->getScore ();
    uint16_t scoreWidth = m_ScoreFont->getTextWidth (scoreString.str ());
    uint16_t scoreX = static_cast<uint16_t>(k_PositionXScore * scaleFactor) -
                      scoreWidth;
    uint16_t scoreY = static_cast<uint16_t>(k_PositionYScore * scaleFactor);

    std::stringstream levelString;
    levelString << getCurrentLevel ();
    uint16_t levelWidth = m_ScoreFont->getTextWidth (levelString.str ());
    uint16_t levelX = static_cast<uint16_t>(k_PositionXLevel * scaleFactor) -
                      levelWidth;
    uint16_t levelY = static_cast<uint16_t>(k_PositionYLevel * scaleFactor);

    m_ScoreFont->write (scoreString.str (), scoreX, scoreY, screen);
    m_ScoreFont->write (levelString.str (), levelX, levelY, screen);

    // Draw the 'Level Up!' label if it must be show.
    if ( m_LevelUpTime > 0 )
    {
        m_LevelUp->blit (getPlayerGrid ()->getGridPositionX () +
                         getAmoebasSize () * Grid::k_GridWidth / 2 -
                         m_LevelUp->getWidth () / 2,
                         static_cast<int16_t>(m_LevelUpPosition), screen);
    }
    // Draw the 'Ready?' or 'Go!!' labels if they must be shown.
    if ( mustShowInitialLabels () )
    {
        if ( mustShowReadyLabel () )
        {
            m_Ready->blit (getPlayerGrid ()->getGridPositionX () +
                           getAmoebasSize () * Grid::k_GridWidth / 2 -
                           m_Ready->getWidth () / 2,
                           getPlayerGrid ()->getGridPositionY () +
                           getAmoebasSize () * (Grid::k_VisibleHeight - 1) / 2 -
                           m_Ready->getHeight () / 2, screen);
        }
        else
        {
            m_Go->blit (getPlayerGrid ()->getGridPositionX () +
                        getAmoebasSize () * Grid::k_GridWidth / 2 -
                        m_Go->getWidth () / 2,
                        getPlayerGrid ()->getGridPositionY () +
                        getAmoebasSize () * (Grid::k_VisibleHeight - 1) / 2 -
                        m_Go->getHeight () / 2, screen);
        }
    }

    // Draw the 'Game Over' text if the board is filled.
    if ( getPlayerGrid ()->isFilled () )
    {
        m_GameOver->blit (gridRectangle.x + getAmoebasSize () *
                          Grid::k_GridWidth / 2  -
                          m_GameOver->getWidth () / 2,
                          gridRectangle.y + gridRectangle.h / 2 -
                          m_GameOver->getHeight () / 2,
                          screen);
    }
}

void
TrainingState::update (uint32_t elapsedTime)
{
    if ( mustShowInitialLabels () )
    {
        if ( mustShowReadyLabel () )
        {
            setReadyTime (getReadyTime () - elapsedTime);
        }
        else
        {
            setGoTime (getGoTime () - elapsedTime);
        }
    }
    else if ( !getPlayerGrid ()->isFilled () )
    {
        getPlayer ()->update (elapsedTime);
        // If the game is over, stop the music and play the lose sound.
        if ( getPlayerGrid ()->isFilled () )
        {
            m_BackgroundMusic->stop ();
            m_SoundLose->play ();
        }
        // If the grid has a new falling pair (i.e., has no floating amoebas
        // and the previous pair alredy felt.), then add ghost amoebas
        // based on the current level and check if we must change the current
        // level.
        else if ( getPlayerGrid ()->hasNewFallingPair () )
        {
            addGhostAmoebas ();
            checkLevelChange ();
        }
        // Otherwise, just update the "Level Up!" labe, in case it must be
        // shown.
        else if ( m_LevelUpTime > 0 )
        {
            m_LevelUpPosition -= k_LevelUpSpeed * elapsedTime;
            m_LevelUpTime -= elapsedTime;
        }
    }
}

void
TrainingState::videoModeChanged (void)
{
    loadGraphicResources ();
}
