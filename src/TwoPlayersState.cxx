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
#include <cassert>
#include <SDL.h>
#include <sstream>
#include <algorithm>
#include "Amoeba.h"
#include "DrawAmoeba.h"
#include "DrawChainLabel.h"
#include "File.h"
#include "NormalState.h"
#include "System.h"
#include "TwoPlayersState.h"

using namespace Amoebax;

/// The size of the silhouettes' border in pixels.
static const uint8_t k_SilhouetteBorder = 8;

///
/// \brief Default constructor.
///
/// \param leftPlayer The player that will control the left grid.
/// \param rightPlayer The player that will control the right grid.
/// \param backgroundFileName The file to use as background.
/// \param leftPlayerScore The initial score of the left player.
/// \param rightPlayerScore The initial score of the right player.
/// \param observer The class that will receive an end of match notify.
///
TwoPlayersState::TwoPlayersState (IPlayer *leftPlayer, IPlayer *rightPlayer,
                                  const std::string &backgroundFileName,
                                  uint32_t leftPlayerScore,
                                  uint32_t rightPlayerScore,
                                  IMatchObserver *observer):
    IState (),
    m_Amoebas (nullptr),
    m_AmoebasSize (k_MaxAmoebasSize),
    m_Background (nullptr),
    m_BackgroundFileName (backgroundFileName),
    m_BackgroundMusic (nullptr),
    m_ChainLabel (nullptr),
    m_GameIsOver (false),
    m_Generator (new PairGenerator ()),
    m_Go (nullptr),
    m_GoTime (k_DefaultGoTime),
    m_LeftPlayer (leftPlayer),
    m_Observer(observer),
    m_Ready (nullptr),
    m_ReadyTime (k_DefaultGoTime * 2),
    m_RightPlayer (rightPlayer),
    m_ScoreFont (nullptr),
    m_SilhouetteBorder (k_SilhouetteBorder),
    m_Silhouettes (nullptr),
    m_StateAlreadyRemoved (false),
    m_YouLose (nullptr),
    m_YouWin (nullptr),
    m_Winner (IPlayer::RightSide)
{
    assert ( 0 < getAmoebasSize () && "The amoebas' size is invalid." );
    assert ( 0 != getLeftPlayer () && "The left player is NULL." );
    assert ( 0 != getRightPlayer () && "The right player is NULL." );

    loadGraphicsResources ();

    float screenScale = System::getInstance ().getScreenScaleFactor ();
    getLeftPlayer ()->setGrid (
            new Grid (static_cast<uint16_t>(k_PositionXLeftGrid * screenScale),
                      static_cast<uint16_t>(k_PositionYLeftGrid * screenScale),
                      static_cast<uint16_t>(k_PositionXLeftQueue * screenScale),
                      static_cast<uint16_t>(k_PositionYLeftQueue * screenScale),
                      static_cast<uint16_t>(k_PositionXLeftWaiting * screenScale),
                      static_cast<uint16_t>(k_PositionYLeftWaiting * screenScale),
                      getAmoebasSize (), Grid::QueueSideRight, leftPlayerScore));
    m_Generator->addGrid (getLeftGrid ());

    getRightPlayer ()->setGrid (
            new Grid (static_cast<uint16_t>(k_PositionXRightGrid * screenScale),
                      static_cast<uint16_t>(k_PositionYRightGrid * screenScale),
                      static_cast<uint16_t>(k_PositionXRightQueue * screenScale),
                      static_cast<uint16_t>(k_PositionYRightQueue * screenScale),
                      static_cast<uint16_t>(k_PositionXRightWaiting * screenScale),
                      static_cast<uint16_t>(k_PositionYRightWaiting * screenScale),
                      getAmoebasSize (), Grid::QueueSideLeft, rightPlayerScore));
    m_Generator->addGrid (getRightGrid ());

    // Generate the four first amoebas.
    m_Generator->generate (4);

    // Load music.
    std::string musicFileName (backgroundFileName);
    musicFileName.replace (musicFileName.rfind (".png"), 4, ".ogg");
    m_BackgroundMusic.reset (
            Music::fromFile (File::getMusicFilePath (musicFileName)));
}

void
TwoPlayersState::activate (void)
{
    getLeftPlayer ()->loadOptions ();
    getRightPlayer ()->loadOptions ();
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
/// \brief Checks if the key should remove the current state.
///
/// If the game is over and the user presses any key different
/// to the player's keys, then the state should be removed from the
/// system.
///
/// \param key The key to check to see if it's an already mapped key
///            to a player.
///
void
TwoPlayersState::checkForRemoveStateKey (uint32_t key)
{
    Options::PlayerControls leftPlayer =
        Options::getInstance ().getPlayerControls (IPlayer::LeftSide);
    Options::PlayerControls rightPlayer =
        Options::getInstance ().getPlayerControls (IPlayer::RightSide);

    //TODO: split joy/key

    if ( gameIsOver () && !m_StateAlreadyRemoved &&
#if defined IS_GP2X_HOST
        (key == GP2X_BUTTON_START ||
         key == GP2X_BUTTON_SELECT ||
         key == GP2X_BUTTON_X)
#else // !IS_GP2X_HOST
         key != leftPlayer.keyboard.moveLeft &&
         key != leftPlayer.keyboard.moveRight &&
         key != leftPlayer.keyboard.pushDown &&
         key != leftPlayer.keyboard.rotateClockwise &&
         key != leftPlayer.keyboard.rotateCounterClockwise &&
         key != rightPlayer.keyboard.moveLeft &&
         key != rightPlayer.keyboard.moveRight &&
         key != rightPlayer.keyboard.pushDown &&
         key != rightPlayer.keyboard.rotateClockwise &&
         key != rightPlayer.keyboard.rotateCounterClockwise
#endif // IS_GP2X_HOST
         )
    {
        System::getInstance ().removeActiveState ();
        m_StateAlreadyRemoved = true;
    }
}

///
/// \brief Tells if the game is over.
///
/// \return \a true if the game is over, \a false otherwise.
///
inline bool
TwoPlayersState::gameIsOver (void) const
{
    return m_GameIsOver;
}

///
/// \brief Gets the amoebas' size.
///
/// \return The size of the amoebas. Since the amoebas are squared, this
///         size is the same for width and for height.
///
inline uint8_t
TwoPlayersState::getAmoebasSize (void) const
{
    return m_AmoebasSize;
}

///
/// \brief Gets the time to display the "Go!" label.
///
/// \return The time that the "Go!" label should be visible.
///
inline int32_t
TwoPlayersState::getGoTime (void) const
{
    return m_GoTime;
}

///
/// \brief Gets the left player's grid.
///
/// \return The grid of the left player.
///
inline Grid *
TwoPlayersState::getLeftGrid (void) const
{
    IPlayer *player = getLeftPlayer ();
    assert ( 0 != player && "The left player is NULL." );
    return player->getGrid ();
}

///
/// \brief Gets the player of the left grid.
///
/// \return The pointer to the player that controls the left grid.
///
inline IPlayer *
TwoPlayersState::getLeftPlayer (void) const
{
    return m_LeftPlayer.get ();
}

///
/// \brief Gets the time to display the "Ready?" label.
///
/// \return The time that the "Ready?" label should be visible.
///
inline int32_t
TwoPlayersState::getReadyTime (void) const
{
    return m_ReadyTime;
}

///
/// \brief Gets the right player's grid.
///
/// \return The grid of the right player.
///
inline Grid *
TwoPlayersState::getRightGrid (void) const
{
    IPlayer *player = getRightPlayer ();
    assert ( 0 != player && "The right player is NULL." );
    return player->getGrid ();
}

///
/// \brief Gets the player of the right grid.
///
/// \return The pointer to the player that controls the left grid.
///
inline IPlayer *
TwoPlayersState::getRightPlayer (void) const
{
    return m_RightPlayer.get ();
}

///
/// \brief Gets the size of the silhouettes' border.
///
/// \return The size of the silhouettes's border.
///
inline uint8_t
TwoPlayersState::getSilhouetteBorder (void) const
{
    return m_SilhouetteBorder;
}

///
/// \brief Gets the winner's side.
///
/// \return The side whose player is the winner.
///
inline IPlayer::PlayerSide
TwoPlayersState::getWinnerSide (void) const
{
    return m_Winner;
}

void
TwoPlayersState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
    getLeftPlayer ()->joyMotion (joystick, axis, value);
    getRightPlayer ()->joyMotion (joystick, axis, value);
}

void
TwoPlayersState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    if ( gameIsOver () )
    {
#if defined (IS_GP2X_HOST)
        checkForRemoveStateKey (static_cast<uint32_t>(button));
#endif // IS_GP2X_HOST
    }
    else if ( button == SDL_CONTROLLER_BUTTON_START ||
              button == SDL_CONTROLLER_BUTTON_B )
    {
        System::getInstance ().pause ();
    }
    else
    {
        getLeftPlayer ()->joyDown (joystick, button);
        getRightPlayer ()->joyDown (joystick, button);
    }
}

void
TwoPlayersState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
    if ( !gameIsOver () )
    {
        getLeftPlayer ()->joyUp (joystick, button);
        getRightPlayer ()->joyUp (joystick, button);
    }
}

void
TwoPlayersState::keyDown (uint32_t key)
{
    if ( gameIsOver () )
    {
        checkForRemoveStateKey (key);
    }
    else if ( SDLK_ESCAPE == key )
    {
        System::getInstance ().pause ();
    }
    else
    {
        getLeftPlayer ()->keyDown (key);
        getRightPlayer ()->keyDown (key);
    }
}

void
TwoPlayersState::keyUp (uint32_t key)
{
    if ( !gameIsOver () )
    {
        getRightPlayer ()->keyUp (key);
        getLeftPlayer ()->keyUp (key);
    }
}

///
/// \brief Loads all graphical resources.
///
void
TwoPlayersState::loadGraphicsResources (void)
{
    float screenScale = System::getInstance ().getScreenScaleFactor ();

    m_Amoebas.reset (
            Surface::fromFile (File::getGraphicsFilePath ("amoebas.png")));
    m_Amoebas->resize (screenScale);
    setAmoebasSize (static_cast<uint8_t> (k_MaxAmoebasSize * screenScale));

    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath (m_BackgroundFileName)));
    {
        std::unique_ptr<Surface> gridBackground (
            Surface::fromFile (File::getGraphicsFilePath ("twoplayers.png")));
        gridBackground->blit (m_Background->toSDLSurface ());
    }
    m_Background->resize (screenScale);

    m_ChainLabel.reset (
            Surface::fromFile (File::getGraphicsFilePath ("chain.png")));
    m_ChainLabel->resize (screenScale);

    m_Silhouettes.reset (
            Surface::fromFile (File::getGraphicsFilePath ("silhouettes.png")));
    m_Silhouettes->resize (screenScale);
    setSilhouetteBorder (static_cast<uint8_t> (screenScale *
                                               k_SilhouetteBorder));

    m_YouLose.reset (
            Surface::fromFile (File::getGraphicsFilePath ("YouLose.png")));
    m_YouLose->resize (screenScale);

    m_YouWin.reset (
            Surface::fromFile (File::getGraphicsFilePath ("YouWin.png")));
    m_YouWin->resize (screenScale);

    m_ScoreFont.reset (Font::fromFile (File::getFontFilePath ("score")));

    // Load the "Go!" and "Ready?" labels only if they are going to be shown.
    if ( mustShowInitialLabels () )
    {
        m_Go.reset (Surface::fromFile (File::getGraphicsFilePath ("go.png")));
        m_Ready.reset (
                Surface::fromFile (File::getGraphicsFilePath ("ready.png")));
    }
}

///
/// \brief Tells if the "Go!!" label should be displayed.
///
/// \return \a true If the "Go!!" label should be displayed,
///         \a false otherwise.
///
inline bool
TwoPlayersState::mustShowGoLabel (void) const
{
    return getGoTime () > 0;
}

///
/// \brief Tells if either the "Go!!" or the "Ready?" labels should be shown.
///
/// \return \a true if either label must be displayed, \a false otherwise.
///
inline bool
TwoPlayersState::mustShowInitialLabels (void) const
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
TwoPlayersState::mustShowReadyLabel (void) const
{
    return getReadyTime () > 0;
}

void
TwoPlayersState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
TwoPlayersState::render (SDL_Surface *screen)
{
    // Draw left grid's amoebas.
    {
        SDL_Rect gridRectangle;
        gridRectangle.x = getLeftGrid ()->getGridPositionX ();
        gridRectangle.y = getLeftGrid ()->getGridPositionY ();
        gridRectangle.w = getLeftGrid ()->getQueuePositionX () +
                          getAmoebasSize () - gridRectangle.x;
        gridRectangle.h = getAmoebasSize () * Grid::k_VisibleHeight;
        SDL_SetClipRect (screen, &gridRectangle);

        // Draw the main falling amoeba's silhouette.
        const Grid::FallingAmoeba mainAmoeba =
            getLeftGrid ()->getFallingMainAmoeba ();
        if ( 0 != mainAmoeba.amoeba )
        {
            int8_t silhouetteFrame = getLeftGrid ()->getSilhouetteFrame ();
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

        const std::list<Amoeba *> &leftActiveAmoebas =
            getLeftGrid ()->getActiveAmoebas ();
        std::for_each (leftActiveAmoebas.begin (), leftActiveAmoebas.end (),
                       DrawAmoeba (getAmoebasSize (), m_Amoebas.get (), screen));
    }
    // Draw left queued amoebas.
    {
        SDL_Rect queueRectangle;
        queueRectangle.x = getLeftGrid ()->getQueuePositionX ();
        queueRectangle.y = getLeftGrid ()->getQueuePositionY ();
        queueRectangle.w = getAmoebasSize ();
        queueRectangle.h = 4 * getAmoebasSize ();

        SDL_SetClipRect (screen, &queueRectangle);
        const std::list<Amoeba *> &queuedAmoebas = getLeftGrid ()->getQueuedAmoebas ();
        for_each (queuedAmoebas.begin (), queuedAmoebas.end (),
                DrawAmoeba (getAmoebasSize (), m_Amoebas.get (), screen));
    }
    // Draw left waiting amoebas.
    {
        SDL_Rect waitingGhosts;
        waitingGhosts.x = getLeftGrid ()->getWaitingGhostPositionX ();
        waitingGhosts.y = getLeftGrid ()->getWaitingGhostPositionY ();
        waitingGhosts.w = getAmoebasSize () * Grid::k_GridWidth;
        waitingGhosts.h = getAmoebasSize ();
        SDL_SetClipRect (screen, &waitingGhosts);

        const std::vector<Amoeba *> &leftGhostAmoebas =
            getLeftGrid ()->getWaitingGhostAmoebas ();
        std::for_each (leftGhostAmoebas.begin (), leftGhostAmoebas.end (),
                       DrawAmoeba (getAmoebasSize (), m_Amoebas.get (), screen));
    }

    // Draw right grid's amoebas.
    {
        SDL_Rect gridRectangle;
        gridRectangle.x = getRightGrid ()->getQueuePositionX ();
        gridRectangle.y = getRightGrid ()->getQueuePositionY ();
        gridRectangle.w = getRightGrid ()->getGridPositionY () +
                          getAmoebasSize () * Grid::k_GridWidth - gridRectangle.x;
        gridRectangle.h = getAmoebasSize () * Grid::k_VisibleHeight;
        SDL_SetClipRect (screen, &gridRectangle);

        // Draw the main falling amoeba's silhouette.
        const Grid::FallingAmoeba mainAmoeba =
            getRightGrid ()->getFallingMainAmoeba ();
        if ( 0 != mainAmoeba.amoeba )
        {
            int8_t silhouetteFrame = getRightGrid ()->getSilhouetteFrame ();
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

        const std::list<Amoeba *> &rightActiveAmoebas =
            getRightGrid ()->getActiveAmoebas ();
        std::for_each (rightActiveAmoebas.begin (), rightActiveAmoebas.end (),
                       DrawAmoeba (getAmoebasSize (), m_Amoebas.get (), screen));
    }
    // Draw right queued amoebas.
    {
        SDL_Rect queueRectangle;
        queueRectangle.x = getRightGrid ()->getQueuePositionX ();
        queueRectangle.y = getRightGrid ()->getQueuePositionY ();
        queueRectangle.w = getAmoebasSize ();
        queueRectangle.h = 4 * getAmoebasSize ();

        SDL_SetClipRect (screen, &queueRectangle);
        const std::list<Amoeba *> &queuedAmoebas = getRightGrid ()->getQueuedAmoebas ();
        for_each (queuedAmoebas.begin (), queuedAmoebas.end (),
                DrawAmoeba (getAmoebasSize (), m_Amoebas.get (), screen));
    }

    // Draw right waiting amoebas.
    {
        SDL_Rect waitingGhosts;
        waitingGhosts.x = getRightGrid ()->getWaitingGhostPositionX ();
        waitingGhosts.y = getRightGrid ()->getWaitingGhostPositionY ();
        waitingGhosts.w = getAmoebasSize () * Grid::k_GridWidth;
        waitingGhosts.h = getAmoebasSize ();
        SDL_SetClipRect (screen, &waitingGhosts);

        const std::vector<Amoeba *> &rightGhostAmoebas =
            getRightGrid ()->getWaitingGhostAmoebas ();
        std::for_each (rightGhostAmoebas.begin (), rightGhostAmoebas.end (),
                       DrawAmoeba (getAmoebasSize (), m_Amoebas.get (), screen));
    }

    // Remove any clipping ares.
    SDL_SetClipRect (screen, 0);

    // Draw all grids' chain labels.
    {
        const std::list<ChainLabel *> rightChainLabels =
            getRightGrid()->getChainLabels ();
        std::list<ChainLabel *> chainLabels (getLeftGrid ()->getChainLabels ());

        std::copy (rightChainLabels.begin (), rightChainLabels.end (),
                   std::back_inserter (chainLabels));
        std::for_each (chainLabels.begin (), chainLabels.end (),
                       DrawChainLabel (m_ChainLabel.get (), m_ScoreFont.get (),
                                       screen));
    }

    // Draw the current players' score. The right player has its
    // score right aligned and the left player left aligned.
    const float scaleFactor = System::getInstance ().getScreenScaleFactor ();

    std::stringstream leftScoreString;
    leftScoreString << getLeftGrid ()->getScore ();
    uint16_t leftScoreX = static_cast<uint16_t>(k_PositionXLeftScore *
                                                scaleFactor);
    uint16_t leftScoreY = static_cast<uint16_t>(k_PositionYLeftScore *
                                                scaleFactor);

    std::stringstream rightScoreString;
    rightScoreString << getRightGrid ()->getScore ();
    uint16_t rightScoreWidth =
        m_ScoreFont->getTextWidth (rightScoreString.str ());
    uint16_t rightScoreX = static_cast<uint16_t>(k_PositionXRightScore *
                                                 scaleFactor) - rightScoreWidth;
    uint16_t rightScoreY = static_cast<uint16_t>(k_PositionYRightScore *
                                                 scaleFactor);

    m_ScoreFont->write (leftScoreString.str (), leftScoreX, leftScoreY, screen);
    m_ScoreFont->write (rightScoreString.str (), rightScoreX, rightScoreY,
                        screen);

    // Draw the 'You Lose', 'You Win!!' labels if the game is over.
    if ( gameIsOver () )
    {
        Surface *leftPlayer =
            IPlayer::LeftSide == getWinnerSide () ? m_YouWin.get () :
                                                    m_YouLose.get ();
        leftPlayer->blit (getLeftGrid ()->getGridPositionX () +
                          getAmoebasSize () * Grid::k_GridWidth / 2 -
                          leftPlayer->getWidth () / 2,
                          getLeftGrid ()->getGridPositionY () +
                          getAmoebasSize () * Grid::k_VisibleHeight / 2 -
                          leftPlayer->getHeight () / 2,
                          screen);

        Surface *rightPlayer =
            IPlayer::RightSide == getWinnerSide () ? m_YouWin.get () :
                                                     m_YouLose.get ();
        rightPlayer->blit (getRightGrid ()->getGridPositionX () +
                           getAmoebasSize () * Grid::k_GridWidth / 2 -
                           rightPlayer->getWidth () / 2,
                           getRightGrid ()->getGridPositionY () +
                           getAmoebasSize () * Grid::k_VisibleHeight / 2 -
                           rightPlayer->getHeight () / 2,
                           screen);
    }
    else if ( mustShowInitialLabels () )
    {
        if ( mustShowReadyLabel () )
        {
            m_Ready->blit (getLeftGrid ()->getGridPositionX () +
                           getAmoebasSize () * Grid::k_GridWidth / 2 -
                           m_Ready->getWidth () / 2,
                           getLeftGrid ()->getGridPositionY () +
                           getAmoebasSize () * (Grid::k_VisibleHeight - 1) / 2 -
                           m_Ready->getHeight () / 2, screen);
            m_Ready->blit (getRightGrid ()->getGridPositionX () +
                           getAmoebasSize () * Grid::k_GridWidth / 2 -
                           m_Ready->getWidth () / 2,
                           getRightGrid ()->getGridPositionY () +
                           getAmoebasSize () * (Grid::k_VisibleHeight - 1) / 2 -
                           m_Ready->getHeight () / 2, screen);
        }
        else
        {
            m_Go->blit (getLeftGrid ()->getGridPositionX () +
                        getAmoebasSize () * Grid::k_GridWidth / 2 -
                        m_Go->getWidth () / 2,
                        getLeftGrid ()->getGridPositionY () +
                        getAmoebasSize () * (Grid::k_VisibleHeight - 1) / 2 -
                        m_Go->getHeight () / 2, screen);
            m_Go->blit (getRightGrid ()->getGridPositionX () +
                        getAmoebasSize () * Grid::k_GridWidth / 2 -
                        m_Go->getWidth () / 2,
                        getRightGrid ()->getGridPositionY () +
                        getAmoebasSize () * (Grid::k_VisibleHeight - 1) / 2 -
                        m_Go->getHeight () / 2, screen);
        }
    }
}

///
/// \brief Sets the size of the amoebas.
///
/// \param size The new size of the amoebas. The amoebas are squared,
///             so this size if the same for the width and the height.
///
inline void
TwoPlayersState::setAmoebasSize (uint8_t size)
{
    assert ( 0 < size && "Tried to set an invalid amoebas' size." );
    m_AmoebasSize = size;
}

///
/// \brief The game is over.
///
/// Checks which is the winner player's side and marks the
/// game as over. If there's an attached observer, also
/// notifies it.
///
void
TwoPlayersState::setGameOver (void)
{
    m_GameIsOver = true;
    m_Winner = getLeftGrid ()->isFilled () ? IPlayer::RightSide :
                                             IPlayer::LeftSide;
    if ( m_Observer != 0 )
    {
        m_Observer->endOfMatch (m_Winner,
                                getLeftGrid ()->getScore (),
                                getRightGrid ()->getScore ());
    }
}

///
/// \brief Sets the time to display the "Go!!" label.
///
/// \param time The time to set as time to show the "Go!!" label. If it's
///             negative, the "Go!!" label gets deleted.
///
inline void
TwoPlayersState::setGoTime (int32_t time)
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
TwoPlayersState::setReadyTime (int32_t time)
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
TwoPlayersState::setSilhouetteBorder (uint8_t border)
{
    m_SilhouetteBorder = border;
}

void
TwoPlayersState::update (uint32_t elapsedTime)
{
    if ( !gameIsOver () )
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
        else
        {
            getLeftPlayer ()->update (elapsedTime);
            getRightPlayer ()->update (elapsedTime);
            // Update the waiting ghost amoebas from the opponent.
            getLeftGrid ()->incrementNumberOfWaitingGhosts (
                    getRightGrid ()->getOpponentGhostAmoebas ());
            getRightGrid ()->incrementNumberOfWaitingGhosts (
                    getLeftGrid ()->getOpponentGhostAmoebas ());
            if (getRightGrid ()->isFilled () || getLeftGrid ()->isFilled ())
            {
                setGameOver ();
                Music::stop ();
            }
        }
    }
}

void
TwoPlayersState::videoModeChanged (void)
{
    loadGraphicsResources ();
}
