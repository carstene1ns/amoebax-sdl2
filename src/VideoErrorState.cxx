//
// Cross-platform free Puyo-Puyo clone.
// Copyright (C) 2007 Emma's software
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
#if defined (HAVE_CONFIG_H)
#include <config.h>
#endif // !HAVE_CONFIG_H
#include "File.h"
#include "Font.h"
#include "System.h"
#include "VideoErrorState.h"

using namespace Amoebax;

///
/// \brief Constructor.
///
/// \param activeState The currently active state.
///
VideoErrorState::VideoErrorState (IState *activeState):
    IState (),
    m_ActiveState (activeState),
    m_Background (0),
    m_StateRemoved (false)
{
}

void
VideoErrorState::activate (void)
{
    m_Background.reset (Surface::fromScreen ());
    SDL_Rect region;
    region.x = 0;
    region.y = 0;
    region.w = m_Background->getWidth ();
    region.h = m_Background->getHeight ();
    m_ActiveState->redrawBackground (&region, m_Background->toSDLSurface ());

    SDL_Surface *blackBox =
        SDL_CreateRGBSurface (SDL_SWSURFACE | SDL_SRCALPHA,
                              m_Background->getWidth (),
                              m_Background->getHeight (),
                              m_Background->toSDLSurface ()->format->BitsPerPixel,
                              m_Background->toSDLSurface ()->format->Rmask,
                              m_Background->toSDLSurface ()->format->Gmask,
                              m_Background->toSDLSurface ()->format->Bmask,
                              m_Background->toSDLSurface ()->format->Amask);
    SDL_FillRect (blackBox, NULL, SDL_MapRGB (blackBox->format, 0, 0, 0));
    SDL_SetAlpha (blackBox, SDL_SRCALPHA, 128);
    SDL_BlitSurface (blackBox, NULL, m_Background->toSDLSurface (), NULL);

    std::auto_ptr<Font> font (Font::fromFile (File::getFontFilePath ("fontMenu")));
    font->write ("there was an error setting the video mode",
                 m_Background->getHeight () / 2 - 3 * font->getHeight (),
                 m_Background->toSDLSurface ());
    font->write ("the default settings have been set",
                 m_Background->getHeight () / 2 - 2 * font->getHeight (),
                 m_Background->toSDLSurface ());
    font->write ("press any key to continue",
                 m_Background->getHeight () / 2 + font->getHeight (),
                 m_Background->toSDLSurface ());
}

void
VideoErrorState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
VideoErrorState::joyDown (uint8_t joystick, uint8_t button)
{
    removeState ();
}

void
VideoErrorState::joyUp (uint8_t joystick, uint8_t button)
{
}

#if !defined (IS_GP2X_HOST)
void
VideoErrorState::keyDown (uint32_t key)
{
    removeState ();
}

void
VideoErrorState::keyUp (uint32_t key)
{
}
#endif // !IS_GP2X_HOST

void
VideoErrorState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

///
/// \brief Removes the current state.
///
void
VideoErrorState::removeState (void)
{
    if ( !m_StateRemoved )
    {
        m_StateRemoved = true;
        System::getInstance ().removeActiveState (false);
    }
}

void
VideoErrorState::render (SDL_Surface *screen)
{
}

void
VideoErrorState::update (uint32_t elapsedTime)
{
}

void
VideoErrorState::videoModeChanged (void)
{
}
