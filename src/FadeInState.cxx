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
#include "FadeInState.h"
#include "System.h"

using namespace Amoebax;

///
/// \brief Constructor.
///
/// \param nextState The state that will active once the fade in is done.
/// \param fadeTime The time the fade should take in milliseconds.
///
FadeInState::FadeInState (IState *nextState, uint32_t fadeTime):
    IState (),
    m_Alpha (0),
    m_Background (nullptr),
    m_FadeTime (fadeTime),
    m_NextState (nextState)
{
}

void
FadeInState::activate (void)
{
    m_Background.reset (Surface::fromScreen ());
    SDL_Rect region;
    region.x = 0;
    region.y = 0;
    region.w = m_Background->getWidth ();
    region.h = m_Background->getHeight ();
    m_NextState->redrawBackground (&region, m_Background->toSDLSurface ());
    m_NextState->render (m_Background->toSDLSurface ());
    m_Background->setAlpha (getAlpha ());
}

///
/// \brief The current alpha to apply to the screen.
///
/// \return The current alpha value to apply to the screen.
///
inline int16_t
FadeInState::getAlpha (void) const
{
    return m_Alpha;
}

///
/// \brief Gets the time the fade should take, in milliseconds.
///
/// \return The time the fade should take, in milliseconds.
///
inline uint32_t
FadeInState::getFadeTime (void) const
{
    return m_FadeTime;
}

void
FadeInState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    SDL_FillRect (screen, region, SDL_MapRGB (screen->format, 0, 0, 0));
}

void
FadeInState::render (SDL_Surface *screen)
{
    m_Background->blit (screen);
}

///
/// \brief Sets the alpha value to apply to the screen.
///
/// \param alpha The alpha value to set to the screen.
///
inline void
FadeInState::setAlpha (int16_t alpha)
{
    m_Alpha = alpha;
}

void
FadeInState::update (uint32_t elapsedTime)
{
    setAlpha (getAlpha () +
              static_cast<int16_t>(Surface::k_MaxAlpha *
                    static_cast<float> (elapsedTime) / getFadeTime ()));
    if ( getAlpha () <= Surface::k_MaxAlpha )
    {
        m_Background->setAlpha (getAlpha ());
    }
    else
    {
        System::getInstance ().removeActiveState (false);
    }
}

void
FadeInState::videoModeChanged (void)
{
}
