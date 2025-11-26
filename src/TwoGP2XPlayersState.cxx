//
// Cross-platform free Puyo-Puyo clone.
// Copyright (C) 2006, 2077 Emma's software
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
#include <algorithm>
#include "Amoeba.h"
#include "DrawAmoeba.h"
#include "File.h"
#include "IMatchObserver.h"
#include "System.h"
#include "TwoGP2XPlayersState.h"

using namespace Amoebax;

/// The size of the silhouettes' border in pixels.
static const uint8_t k_SilhouetteBorder = 2;

///
/// \brief Default constructor.
///
TwoGP2XPlayersState::TwoGP2XPlayersState (const std::string &backgroundFileName,
                                          uint32_t leftPlayerScore,
                                          uint32_t rightPlayerScore,
                                          IMatchObserver *observer):
    IState (),
    m_AmoebasLeft (0),
    m_AmoebasRight (0),
    m_AmoebasSize (k_MaxAmoebasSize),
    m_Background (0),
    m_BackgroundFileName (backgroundFileName),
    m_BackgroundMusic (0),
    m_GameIsOver (false),
    m_Generator (new PairGenerator ()),
    m_Go (0),
    m_GoTime (k_DefaultGoTime),
    m_LeftGrid (new Grid (static_cast<uint16_t>(k_PositionXLeftGrid),
                          static_cast<uint16_t>(k_PositionYLeftGrid),
                          static_cast<uint16_t>(k_PositionXLeftQueue),
                          static_cast<uint16_t>(k_PositionYLeftQueue),
                          static_cast<uint16_t>(k_PositionXLeftWaiting),
                          static_cast<uint16_t>(k_PositionYLeftWaiting),
                          getAmoebaSize (), Grid::QueueSideLeft,
                          leftPlayerScore, Grid::LayoutHorizontal)),
    m_Observer (observer),
    m_Ready (0),
    m_ReadyTime (k_DefaultGoTime * 2),
    m_RightGrid (new Grid (static_cast<uint16_t>(k_PositionXRightGrid),
                           static_cast<uint16_t>(k_PositionYRightGrid),
                           static_cast<uint16_t>(k_PositionXRightQueue),
                           static_cast<uint16_t>(k_PositionYRightQueue),
                           static_cast<uint16_t>(k_PositionXRightWaiting),
                           static_cast<uint16_t>(k_PositionYRightWaiting),
                           getAmoebaSize (), Grid::QueueSideRight,
                           rightPlayerScore, Grid::LayoutHorizontal)),
    m_ScoreFont (0),
    m_SilhouetteBorder (k_SilhouetteBorder),
    m_SilhouettesLeft (0),
    m_SilhouettesRight (0),
    m_StateAlreadyRemoved (false),
    m_Winner (IPlayer::RightSide),
    m_WinnerLoser (0)
{
    assert ( 0 < getAmoebasSize () && "The amoeba's size is invalid." );

    loadGraphicsResources ();

    // Add the grids to the pair generator and then generate 4 pairs.
    m_Generator->addGrid (getLeftGrid ());
    m_Generator->addGrid (getRightGrid ());
    m_Generator->generate (4);

    // Load music.
    std::string musicFileName (backgroundFileName);
    musicFileName.replace (musicFileName.rfind (".png"), 4, ".ogg");
    m_BackgroundMusic.reset (
            Music::fromFile (File::getMusicFilePath (musicFileName)));
}

///
/// \brief Destructor.
///
TwoGP2XPlayersState::~TwoGP2XPlayersState (void)
{
}

void
TwoGP2XPlayersState::activate (void)
{
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
/// \brief Tells if the game is over.
///
/// The game is over when one or both grids is filled up.
///
/// \return \a true when at least one grid is filled up, \a false otherwise.
///
inline bool
TwoGP2XPlayersState::gameIsOver (void) const
{
    return m_GameIsOver;
}

///
/// \brief Gets the size of the amoebas.
///
/// \return The size of a single amoeba. All amoebas are squared, so
///         their width and height are equal.
///
inline uint8_t
TwoGP2XPlayersState::getAmoebaSize (void) const
{
    return m_AmoebasSize;
}

///
/// \brief Gets the time to display the "Go!" label.
///
/// \return The time that the "Go!" label should be visible.
///
inline int32_t
TwoGP2XPlayersState::getGoTime (void) const
{
    return m_GoTime;
}

///
/// \brief Gets the left grid.
///
/// \return The pointer to the left grid.
///
inline Grid *
TwoGP2XPlayersState::getLeftGrid (void)
{
    return m_LeftGrid.get ();
}

///
/// \brief Gets the left player's score.
///
/// \return The score of the left player.
///
inline uint32_t
TwoGP2XPlayersState::getLeftPlayerScore (void)
{
    return getLeftGrid ()->getScore ();
}

///
/// \brief Gets the time to display the "Ready?" label.
///
/// \return The time that the "Ready?" label should be visible.
///
inline int32_t
TwoGP2XPlayersState::getReadyTime (void) const
{
    return m_ReadyTime;
}

///
/// \brief Gets the right grid.
///
/// \return The pointer to the right grid.
///
inline Grid *
TwoGP2XPlayersState::getRightGrid (void)
{
    return m_RightGrid.get ();
}

///
/// \brief Gets the right player's score.
///
/// \return The score of the right player.
///
inline uint32_t
TwoGP2XPlayersState::getRightPlayerScore (void)
{
    return getRightGrid ()->getScore ();
}

///
/// \brief Gets the size of the silhouettes' border.
///
/// \return The size of the silhouettes's border.
///
inline uint8_t
TwoGP2XPlayersState::getSilhouetteBorder (void) const
{
    return m_SilhouetteBorder;
}


///
/// \brief Gets the winner's side.
///
/// \return The side whose player is the winner.
///
inline IPlayer::PlayerSide
TwoGP2XPlayersState::getWinnerSide (void) const
{
    return m_Winner;
}

void
TwoGP2XPlayersState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
TwoGP2XPlayersState::joyDown (uint8_t joystick, uint8_t button)
{
    if ( gameIsOver () )
    {
        if ( GP2X_BUTTON_START == button && !m_StateAlreadyRemoved )
        {
            System::getInstance ().removeActiveState ();
            m_StateAlreadyRemoved = true;
        }
    }
    else
    {
        switch (button)
        {
            case GP2X_BUTTON_A:
                getRightGrid ()->rotateClockwise ();
            break;

            case GP2X_BUTTON_B:
                getRightGrid ()->setMaxFallingSpeed ();
            break;

            case GP2X_BUTTON_X:
                getRightGrid ()->moveLeft ();
            break;

            case GP2X_BUTTON_Y:
                getRightGrid ()->moveRight ();
            break;

            case GP2X_BUTTON_START:
                System::getInstance ().pause ();
            break;

            case GP2X_BUTTON_RIGHT:
            case GP2X_BUTTON_CLICK:
                getLeftGrid ()->rotateClockwise ();
            break;

            case GP2X_BUTTON_LEFT:
                getLeftGrid ()->setMaxFallingSpeed ();
            break;

            case GP2X_BUTTON_UP:
            case GP2X_BUTTON_UPRIGHT:
                getLeftGrid ()->moveLeft ();
            break;

            case GP2X_BUTTON_DOWN:
            case GP2X_BUTTON_DOWNRIGHT:
                getLeftGrid ()->moveRight ();
            break;
        }
    }
}

void
TwoGP2XPlayersState::joyUp (uint8_t joystick, uint8_t button)
{
    switch (button)
    {
        case GP2X_BUTTON_B:
            getRightGrid ()->setNormalFallingSpeed ();
        break;

        case GP2X_BUTTON_LEFT:
            getLeftGrid ()->setNormalFallingSpeed ();
        break;
    }
}

///
/// \brief Loads all graphical resources.
///
void
TwoGP2XPlayersState::loadGraphicsResources (void)
{
    m_AmoebasLeft.reset (
            Surface::fromFile (File::getGraphicsFilePath ("leftgrid-gp2x.png")));
    m_AmoebasRight.reset (
            Surface::fromFile (File::getGraphicsFilePath ("rightgrid-gp2x.png")));
    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath (m_BackgroundFileName)));
    {
        std::auto_ptr<Surface> grid (
                Surface::fromFile (File::getGraphicsFilePath ("twoplayers-gp2x.png")));
        grid->blit (m_Background->toSDLSurface ());
    }

    m_SilhouettesLeft.reset (Surface::fromFile (
                File::getGraphicsFilePath ("leftsilhouettes-gp2x.png")));
    m_SilhouettesRight.reset (Surface::fromFile (
                File::getGraphicsFilePath ("rightsilhouettes-gp2x.png")));
    setSilhouetteBorder (k_SilhouetteBorder);

    m_WinnerLoser.reset (
            Surface::fromFile (File::getGraphicsFilePath ("winnerloser-gp2x.png")));

    m_ScoreFont.reset (Font::fromFile (File::getFontFilePath ("score")));

    // Load the "Go!" and "Ready?" labels only if they are going to be shown.
    if ( mustShowInitialLabels () )
    {
        m_Go.reset (
                Surface::fromFile (File::getGraphicsFilePath ("go-gp2x.png")));
        m_Ready.reset (
                Surface::fromFile (File::getGraphicsFilePath ("ready-gp2x.png")));
    }
}

///
/// \brief Tells if the "Go!!" label should be displayed.
///
/// \return \a true If the "Go!!" label should be displayed,
///         \a false otherwise.
///
inline bool
TwoGP2XPlayersState::mustShowGoLabel (void) const
{
    return getGoTime () > 0;
}

///
/// \brief Tells if either the "Go!!" or the "Ready?" labels should be shown.
///
/// \return \a true if either label must be displayed, \a false otherwise.
///
inline bool
TwoGP2XPlayersState::mustShowInitialLabels (void) const
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
TwoGP2XPlayersState::mustShowReadyLabel (void) const
{
    return getReadyTime () > 0;
}

void
TwoGP2XPlayersState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
TwoGP2XPlayersState::render (SDL_Surface *screen)
{
    // Draw left grid's amoebas.
    {
        SDL_Rect gridRectangle;
        gridRectangle.x = getLeftGrid ()->getGridPositionX () -
                          getAmoebaSize () * 12;
        gridRectangle.y = getLeftGrid ()->getGridPositionY ();
        gridRectangle.w = getAmoebaSize () * 12;
        gridRectangle.h = getLeftGrid ()->getQueuePositionY () +
                          getAmoebaSize () - gridRectangle.y;
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
                uint8_t silhouetteSize = getAmoebaSize () +
                    2 * getSilhouetteBorder ();
                m_SilhouettesLeft->blit (silhouetteSize * silhouetteFrame,
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
                       DrawAmoeba (getAmoebaSize (), m_AmoebasLeft.get (),
                                   screen));
    }
    // Draw left queued amoebas.
    {
        SDL_Rect queueRectangle;
        queueRectangle.x = getLeftGrid ()->getQueuePositionX () -
                           4 * getAmoebaSize ();
        queueRectangle.y = getLeftGrid ()->getQueuePositionY ();
        queueRectangle.w = 4 * getAmoebaSize ();
        queueRectangle.h = getAmoebaSize ();

        SDL_SetClipRect (screen, &queueRectangle);
        const std::list<Amoeba *> &queuedAmoebas = getLeftGrid ()->getQueuedAmoebas ();
        for_each (queuedAmoebas.begin (), queuedAmoebas.end (),
                DrawAmoeba (getAmoebaSize (), m_AmoebasLeft.get (), screen));
    }
    // Draw left waiting amoebas.
    {
        SDL_Rect waitingGhosts;
        waitingGhosts.x = getLeftGrid ()->getWaitingGhostPositionX () -
                          getAmoebaSize ();
        waitingGhosts.y = getLeftGrid ()->getWaitingGhostPositionY ();
        waitingGhosts.w = getAmoebaSize ();
        waitingGhosts.h = getAmoebaSize () * 6;
        SDL_SetClipRect (screen, &waitingGhosts);

        const std::vector<Amoeba *> &leftGhostAmoebas =
            getLeftGrid ()->getWaitingGhostAmoebas ();
        std::for_each (leftGhostAmoebas.begin (), leftGhostAmoebas.end (),
                       DrawAmoeba (getAmoebaSize (), m_AmoebasLeft.get (),
                                   screen));
    }

    // Draw right grid's amoebas.
    {
        SDL_Rect gridRectangle;
        gridRectangle.x = getRightGrid ()->getQueuePositionX ();
        gridRectangle.y = getRightGrid ()->getQueuePositionY () -
                          getAmoebaSize ();
        gridRectangle.w = getAmoebaSize () * 12;
        gridRectangle.h = getRightGrid ()->getGridPositionY () -
                          gridRectangle.y;
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
                uint8_t silhouetteSize = getAmoebaSize () +
                    2 * getSilhouetteBorder ();
                m_SilhouettesRight->blit (silhouetteSize * silhouetteFrame,
                        silhouetteSize * amoeba->getColour (),
                        silhouetteSize, silhouetteSize,
                        amoeba->getX () - getSilhouetteBorder (),
                        amoeba->getY () - getSilhouetteBorder (),
                        screen);
            }
        }

        const std::list<Amoeba *> &rightActiveAmoebas =
            getRightGrid ()->getActiveAmoebas ();
        std::for_each (rightActiveAmoebas.begin (),
                       rightActiveAmoebas.end (),
                       DrawAmoeba (getAmoebaSize (), m_AmoebasRight.get (),
                                   screen));
    }
    // Draw right queued amoebas.
    {
        SDL_Rect queueRectangle;
        queueRectangle.x = getRightGrid ()->getQueuePositionX ();
        queueRectangle.y = getRightGrid ()->getQueuePositionY () -
                           getAmoebaSize ();
        queueRectangle.w = 4 * getAmoebaSize ();
        queueRectangle.h = getAmoebaSize ();

        SDL_SetClipRect (screen, &queueRectangle);
        const std::list<Amoeba *> &queuedAmoebas = getRightGrid ()->getQueuedAmoebas ();
        for_each (queuedAmoebas.begin (), queuedAmoebas.end (),
                DrawAmoeba (getAmoebaSize (), m_AmoebasRight.get (), screen));
    }
    // Draw right waiting amoebas.
    {
        SDL_Rect waitingGhosts;
        waitingGhosts.x = getRightGrid ()->getWaitingGhostPositionX ();
        waitingGhosts.y = getRightGrid ()->getWaitingGhostPositionY () -
                          getAmoebaSize () * 6;
        waitingGhosts.w = getAmoebaSize ();
        waitingGhosts.h = getAmoebaSize () * 6;
        SDL_SetClipRect (screen, &waitingGhosts);

        const std::vector<Amoeba *> &rightGhostAmoebas =
            getRightGrid ()->getWaitingGhostAmoebas ();
        std::for_each (rightGhostAmoebas.begin (), rightGhostAmoebas.end (),
                       DrawAmoeba (getAmoebaSize (), m_AmoebasRight.get (),
                                   screen));
    }

    // Remove any clipping area.
    SDL_SetClipRect (screen, 0);

    // Draw the current player's score.  The right player has its
    // score right aligned and the left player left aligned.
    std::ostringstream leftScoreString;
    leftScoreString << getLeftPlayerScore ();
    uint16_t leftScoreX = k_PositionXLeftScore;
    uint16_t leftScoreY = k_PositionYLeftScore;

    std::ostringstream rightScoreString;
    rightScoreString << getRightPlayerScore ();
    uint16_t rightScoreWidth =
        m_ScoreFont->getTextWidth (rightScoreString.str ());
    uint16_t rightScoreX = k_PositionXRightScore - rightScoreWidth;
    uint16_t rightScoreY = k_PositionYRightScore;

    m_ScoreFont->write (leftScoreString.str (), leftScoreX, leftScoreY, screen);
    m_ScoreFont->write (rightScoreString.str (), rightScoreX, rightScoreY,
                        screen);

    if ( gameIsOver () )
    {
        // Draw left grid's result graphic.
        {
            uint16_t sourceWidth = m_WinnerLoser->getWidth () / 2;
            uint16_t sourceHeight = m_WinnerLoser->getHeight () / 2;
            uint16_t sourceY =
                (IPlayer::RightSide == getWinnerSide ()) ? sourceHeight : 0;
            uint16_t sourceX = 0;
            uint16_t destinationX = getLeftGrid ()->getGridPositionX () -
                                    getAmoebaSize () * 6 - sourceWidth / 2;
            uint16_t destinationY = getLeftGrid ()->getGridPositionY () +
                                    getAmoebaSize () * 3 - sourceHeight / 2;

            m_WinnerLoser->blit (sourceX, sourceY, sourceWidth, sourceHeight,
                                 destinationX, destinationY, screen);
        }

        // Draw right grid's result graphic.
        {
            uint16_t sourceWidth = m_WinnerLoser->getWidth () / 2;
            uint16_t sourceHeight = m_WinnerLoser->getHeight () / 2;
            uint16_t sourceY =
                (IPlayer::LeftSide == getWinnerSide ()) ? sourceHeight : 0;
            uint16_t sourceX = sourceWidth;
            uint16_t destinationX = getRightGrid ()->getGridPositionX () +
                                    getAmoebaSize () * 6 - sourceWidth / 2;
            uint16_t destinationY = getRightGrid ()->getGridPositionY () -
                                    getAmoebaSize () * 3 - sourceHeight / 2;

            m_WinnerLoser->blit (sourceX, sourceY, sourceWidth, sourceHeight,
                                 destinationX, destinationY, screen);
        }
    }
    else if ( mustShowInitialLabels () )
    {
        if ( mustShowReadyLabel () )
        {
            uint16_t sourceWidth = m_Ready->getWidth () / 2;
            uint16_t sourceHeight = m_Ready->getHeight ();
            uint16_t leftDestinationX = getLeftGrid ()->getGridPositionX () -
                getAmoebaSize () * 6 - sourceWidth / 2;
            uint16_t leftDestinationY = getLeftGrid ()->getGridPositionY () +
                getAmoebaSize () * 3 - sourceHeight / 2;
            uint16_t rightDestinationX = getRightGrid ()->getGridPositionX () +
                getAmoebaSize () * 6 - sourceWidth / 2;
            uint16_t rightDestinationY = getRightGrid ()->getGridPositionY () -
                getAmoebaSize () * 3 - sourceHeight / 2;

            m_Ready->blit (0, 0, sourceWidth, sourceHeight,
                           leftDestinationX, leftDestinationY, screen);
            m_Ready->blit (sourceWidth, 0, sourceWidth, sourceHeight,
                           rightDestinationX, rightDestinationY, screen);
        }
        else
        {
            uint16_t sourceWidth = m_Go->getWidth () / 2;
            uint16_t sourceHeight = m_Go->getHeight ();
            uint16_t leftDestinationX = getLeftGrid ()->getGridPositionX () -
                getAmoebaSize () * 6 - sourceWidth / 2;
            uint16_t leftDestinationY = getLeftGrid ()->getGridPositionY () +
                getAmoebaSize () * 3 - sourceHeight / 2;
            uint16_t rightDestinationX = getRightGrid ()->getGridPositionX () +
                getAmoebaSize () * 6 - sourceWidth / 2;
            uint16_t rightDestinationY = getRightGrid ()->getGridPositionY () -
                getAmoebaSize () * 3 - sourceHeight / 2;

            m_Go->blit (0, 0, sourceWidth, sourceHeight,
                        leftDestinationX, leftDestinationY, screen);
            m_Go->blit (sourceWidth, 0, sourceWidth, sourceHeight,
                        rightDestinationX, rightDestinationY, screen);
        }
    }

}

///
/// \brief The game is over.
///
/// Check which is the winner player's side and marks the
/// game as over.  If there's an attached observer, also
/// notifies it.
///
void
TwoGP2XPlayersState::setGameOver (void)
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
TwoGP2XPlayersState::setGoTime (int32_t time)
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
TwoGP2XPlayersState::setReadyTime (int32_t time)
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
TwoGP2XPlayersState::setSilhouetteBorder (uint8_t border)
{
    m_SilhouetteBorder = border;
}

void
TwoGP2XPlayersState::update (uint32_t elapsedTime)
{
    // Only update if no grid is filled.
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
            // Update both grids.
            getLeftGrid ()->update (elapsedTime);
            getRightGrid ()->update (elapsedTime);
            // Update the waiting ghost amoebas.
            getLeftGrid ()->incrementNumberOfWaitingGhosts (
                    getRightGrid ()->getOpponentGhostAmoebas ());
            getRightGrid ()->incrementNumberOfWaitingGhosts (
                    getLeftGrid ()->getOpponentGhostAmoebas ());
            if ( getLeftGrid ()->isFilled () || getRightGrid ()->isFilled () )
            {
                setGameOver ();
                Music::stop ();
            }
        }
    }
}

void
TwoGP2XPlayersState::videoModeChanged (void)
{
    loadGraphicsResources ();
}
