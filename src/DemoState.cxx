//
// Cross-platform free Puyo-Puyo clone.
// Copyright (C) 2006, 2007 Emma's software
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
#include "AIPlayerFactory.h"
#include "DemoState.h"
#include "System.h"

using namespace Amoebax;

///
/// \brief Default constructor.
///
DemoState::DemoState (void):
    IState (),
    IMatchObserver (),
    m_DemoTime (k_DemoTime),
    m_Match (nullptr),
    m_StateRemoved (false)
{
    uint8_t leftPlayerLevel = rand () % AIPlayerFactory::k_MaxPlayerLevel;
    uint8_t rightPlayerLevel = rand () % AIPlayerFactory::k_MaxPlayerLevel;
    while ( leftPlayerLevel == rightPlayerLevel )
    {
        rightPlayerLevel = rand () % AIPlayerFactory::k_MaxPlayerLevel;
    }

    m_Match.reset (
            new TwoPlayersState (AIPlayerFactory::create (leftPlayerLevel,
                                                          IPlayer::LeftSide),
                                 AIPlayerFactory::create (rightPlayerLevel,
                                                          IPlayer::RightSide),
                                 AIPlayerFactory::getRandomBackgroundFileName (),
                                 0, 0, this));
}

void
DemoState::activate (void)
{
    setDemoTime (k_DemoTime);
    m_Match->activate ();
}

void
DemoState::endOfMatch (IPlayer::PlayerSide winner, uint32_t leftPlayerScore,
                       uint32_t rightPlayerScore)
{
    removeState ();
}

///
/// \brief Gets the time the demo has until the end.
///
/// \return The number of milliseconds until the end of the demo.
///
inline int32_t
DemoState::getDemoTime (void) const
{
    return m_DemoTime;
}

void
DemoState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
    removeState ();
}

void
DemoState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    removeState ();
}

void
DemoState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
DemoState::keyDown (uint32_t key)
{
    removeState ();
}

void
DemoState::keyUp (uint32_t key)
{
}

void
DemoState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Match->redrawBackground (region, screen);
}

///
/// \brief Removes this state once.
///
/// Checks if the state was already removed and if was not, then
/// removes it from the System list of active states.
///
void
DemoState::removeState (void)
{
    if ( !m_StateRemoved )
    {
        m_StateRemoved = true;
        System::getInstance ().removeActiveState ();
    }
}

void
DemoState::render (SDL_Surface *screen)
{
    m_Match->render (screen);
}

///
/// \brief Sets the time until the end of the demo.
///
/// \param time The time until the demo finishes.
///
inline void
DemoState::setDemoTime (int32_t time)
{
    m_DemoTime = time;
}

void
DemoState::update (uint32_t elapsedTime)
{
    m_Match->update (elapsedTime);
    setDemoTime (getDemoTime () - elapsedTime);
    if ( getDemoTime () <= 0 )
    {
        removeState ();
    }
}

void
DemoState::videoModeChanged (void)
{
    m_Match->videoModeChanged ();
}
