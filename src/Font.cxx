//
// Cross-platform free Puyo-Puyo clone.
// Copyright (C) 2006, 2007 Emma's Software
//
// Based on SFont, which is Copyright (c) 2003 Karl Bartel
// http://www.linux-games.com/sfont/
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
#include <numeric>
#include <SDL.h>
#include <algorithm>
#include <functional>
#include "Font.h"

using namespace Amoebax;

///
/// \brief Font's default constructor.
///
Font::Font (void):
    m_FontSurface (0)
{
}

///
/// \brief Font's copy constructor.
///
/// \param font The font to copy from.
///
Font::Font (const Font &font):
    m_CharacterInformation (font.m_CharacterInformation),
    m_FontSurface (new Surface (*font.m_FontSurface))
{
}

///
/// \brief Font's destructor.
///
/// There's nothing to do, since the auto_ptr will release the
/// surface for us.
///
Font::~Font (void)
{
}

///
/// \brief Gets the width that would take a text rendered.
///
/// \param text The text to check its width.
/// \return The text's width once rendered.
///
uint16_t
Font::getTextWidth (const std::string &text) const
{
    return std::accumulate (text.begin (), text.end (),
                            static_cast<uint16_t>(0), TextWidth (this));
}

///
/// \brief Creates a new from from a file.
///
/// \param fileName The file name to use to load a new font.
/// \return The new created Font from the file.
///
Font *
Font::fromFile (const std::string &fileName)
{
    std::auto_ptr<Surface> newFontSurface (Surface::fromFile (fileName));
    Font *newFont = new Font ();
    newFont->m_FontSurface = newFontSurface;
    newFont->init ();

    return newFont;
}

///
/// \brief Initializes the font.
///
void
Font::init (void)
{
    // Gets the surface's pink color.
    const uint32_t pinkColor = SDL_MapRGB (m_FontSurface->toSDLSurface ()->format,
                                           255, 0, 255);
    uint16_t x = 0;
    m_CharacterInformation.clear ();
    while ( x < m_FontSurface->toSDLSurface ()->w )
    {
        if ( pinkColor != m_FontSurface->getPixel (x, 0) )
        {
            Character character;
            character.first = x;
            while ( (x < m_FontSurface->toSDLSurface ()->w) &&
                    (pinkColor != m_FontSurface->getPixel (x, 0)) )
            {
                x++;
            }
            character.second = x;
            m_CharacterInformation.push_back (character);
        }
        x++;
    }
    uint32_t transparentPixel =
        m_FontSurface->getPixel (0, m_FontSurface->toSDLSurface ()->h - 1);
    m_FontSurface->setColorKey (transparentPixel);
}

///
/// \brief Writes a text on a given position.
///
/// \param text The text to write.
/// \param x The x coordinate to write the text to.
/// \param y The y coordinate to write the text to.
/// \param destination The surface to write the text to.
///
void
Font::write (const std::string &text, uint16_t x, uint16_t y,
             SDL_Surface *destination) const
{
    std::for_each (text.begin (), text.end (),
                   TextWrite (x, y, this, destination));
}

///
/// \brief Writes a text horizontaly centered into a surface.
///
/// \param text The text to write.
/// \param y The y coordinate to write the text to.
/// \param destination The destination surface to write the text.
///
void
Font::write (const std::string &text, uint16_t y, SDL_Surface *destination) const
{
    write (text, destination->w / 2 - getTextWidth (text) / 2, y, destination);
}
