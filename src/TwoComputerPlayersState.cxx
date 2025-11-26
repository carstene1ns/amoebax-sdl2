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
#include "AIPlayerFactory.h"
#include "File.h"
#include "IMatchObserver.h"
#include "System.h"
#include "TwoComputerPlayersState.h"

using namespace Amoebax;
uint16_t k_SeparationBetweenPlayerAndLabel = 30;

///
/// \brief Default constructor.
///
/// \param leftPlayerLevel The level of the left player.
/// \param rightPlayerLevel The level of the right player.
/// \param observer The class that will receive an end of match notify.
///
TwoComputerPlayersState::TwoComputerPlayersState (uint8_t leftPlayerLevel,
                                                  uint8_t rightPlayerLevel,
                                                  IMatchObserver *observer):
    IState (),
    m_Background (nullptr),
    m_Observer (observer),
    m_ObserverNotified (false),
    m_StateAlreadyRemoved (false),
    m_WinnerSide (
        AIPlayerFactory::getWinner (leftPlayerLevel, rightPlayerLevel))
{
    if ( IPlayer::LeftSide == getWinnerSide () )
    {
        m_WinnerName = AIPlayerFactory::getPlayerName (leftPlayerLevel);
    }
    else
    {
        m_WinnerName = AIPlayerFactory::getPlayerName (rightPlayerLevel);
    }

    loadGraphicsResources ();
}

void
TwoComputerPlayersState::activate (void)
{
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
TwoComputerPlayersState::checkForRemoveStateKey (uint32_t key)
{
    //TODO: maybe split for joy and key
    if ( !m_StateAlreadyRemoved &&
#if defined IS_GP2X_HOST
        (key == GP2X_BUTTON_START ||
         key == GP2X_BUTTON_SELECT ||
         key == GP2X_BUTTON_B ||
         key == GP2X_BUTTON_X)
#else // !IS_GP2X_HOST
         (key == SDLK_RETURN ||
          key == SDLK_ESCAPE)
#endif // IS_GP2X_HOST
         )
    {
        System::getInstance ().removeActiveState ();
        m_StateAlreadyRemoved = true;
    }
}

///
/// \brief Gets the pointer to the match observer.
///
/// \return The pointer to the object that expects to be notified when
///         the match is over with the results.
///
inline IMatchObserver *
TwoComputerPlayersState::getObserver (void) const
{
    return m_Observer;
}

///
/// \brief Gets the name of the winner.
///
/// \return The name of the winner.
///
inline std::string
TwoComputerPlayersState::getWinnerName (void) const
{
    return m_WinnerName;
}

///
/// \brief Gets the side of the winner.
///
/// \return The side of the winner.
///
inline IPlayer::PlayerSide
TwoComputerPlayersState::getWinnerSide (void) const
{
    return m_WinnerSide;
}

///
/// \brief Tells if the observer is already notified or not.
///
/// \return \a true if the observer is already notified, \a false otherwise.
///
inline bool
TwoComputerPlayersState::isObserverNotified (void) const
{
    return m_ObserverNotified;
}

void
TwoComputerPlayersState::joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value)
{
}

void
TwoComputerPlayersState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
#if defined (IS_GP2X_HOST)
    checkForRemoveStateKey (static_cast<uint32_t>(button));
#endif // IS_GP2X_HOST
}

void
TwoComputerPlayersState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
TwoComputerPlayersState::keyDown (uint32_t key)
{
    checkForRemoveStateKey (key);
}

void
TwoComputerPlayersState::keyUp (uint32_t key)
{
}

///
/// \brief Loads all graphical resources.
///
void
TwoComputerPlayersState::loadGraphicsResources (void)
{
    float screenScale = System::getInstance ().getScreenScaleFactor ();

    uint16_t nameY = k_SeparationBetweenPlayerAndLabel;
    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("menuBackground.png")));
    {
        std::unique_ptr<Surface> winner (
                Surface::fromFile (File::getGraphicsFilePath (getWinnerName () + ".png")));
        uint16_t winnerY = m_Background->getHeight () / 2 -
                           winner->getHeight () / 2;
        winner->blit (m_Background->getWidth () / 2 - winner->getWidth () / 2,
                      winnerY, m_Background->toSDLSurface ());

        std::unique_ptr<Surface> YouWin (
                Surface::fromFile (File::getGraphicsFilePath ("winner.png")));

        YouWin->blit (m_Background->getWidth () / 2 - YouWin->getWidth () / 2,
                      m_Background->getHeight () / 2 -
                      winner->getHeight () / 2 -
                      YouWin->getHeight () -
                      static_cast<uint16_t> (k_SeparationBetweenPlayerAndLabel *
                                             screenScale),
                      m_Background->toSDLSurface ());

        nameY += winnerY + winner->getHeight ();
    }
    m_Background->resize (screenScale);
    {
        std::unique_ptr<Font> font (
                Font::fromFile (File::getFontFilePath ("fontMenu")));

        font->write (getWinnerName (),
                     m_Background->getWidth () / 2 -
                     font->getTextWidth (getWinnerName ()) / 2,
                     static_cast<uint16_t> (nameY * screenScale),
                     m_Background->toSDLSurface ());
    }
}

void
TwoComputerPlayersState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
TwoComputerPlayersState::render (SDL_Surface *screen)
{
}

///
/// \brief Marks the observer as already notified.
///
/// \see isObserverNotified()
///
void
TwoComputerPlayersState::setObserverNotified (void)
{
    m_ObserverNotified = true;
}

void
TwoComputerPlayersState::update (uint32_t elapsedTime)
{
    if ( !isObserverNotified () && getObserver () != 0 )
    {
        getObserver ()->endOfMatch (getWinnerSide (), 0, 0);
        setObserverNotified ();
        Music::stop ();
    }
}

void
TwoComputerPlayersState::videoModeChanged (void)
{
    loadGraphicsResources ();
}
