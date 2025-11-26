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
#if !defined (AMOEBAX_SURFACE_H)
#define AMOEBAX_SURFACE_H

#include <string>

// Forward declarations.
struct SDL_Surface;

namespace Amoebax
{
    ///
    /// \class Surface.
    /// \brief Holds an image surface.
    ///
    class Surface
    {
        public:
            /// The maximum value the alpha component can take.
            const static uint8_t k_MaxAlpha = 255;

            Surface (void);
            Surface (const Surface &surface);
            ~Surface (void);

            void blit (SDL_Surface *destination);
            void blit (uint16_t destinationX, uint16_t destinationY,
                       SDL_Surface *destination);
            void blit (uint16_t sourceX, uint16_t sourceY,
                       uint16_t sourceWidth, uint16_t sourceHeight,
                       uint16_t destinationX, uint16_t destinationY,
                       SDL_Surface *destination);
            uint32_t getPixel (uint16_t x, uint16_t y);
            static Surface *fromFile (const std::string fileName);
            static Surface *fromScreen (void);
            uint16_t getHeight (void) const;
            uint16_t getWidth (void) const;
            Surface &operator= (const Surface &surface);
            void resize (float scaleFactor);
            void setAlpha (uint8_t alpha);
            void setColorKey (uint32_t colorKey);
            static void swap (Surface &lhs, Surface &rhs);
            SDL_Surface *toSDLSurface (void) const;

        private:
            Surface (SDL_Surface *SDLSurface);

            /// The real image's SDL surface.
            SDL_Surface *m_SDLSurface;
    };

    ///
    /// \brief Returns the Surface's SDL_Surface pointer.
    ///
    /// \return The pointer to SDL_Surface that represents this surface.
    /// \note This surface must not be manually deleted.
    ///
    inline SDL_Surface *
    Surface::toSDLSurface (void) const
    {
        return m_SDLSurface;
    }
}

#endif // !AMOEBAX_SURFACE_H
