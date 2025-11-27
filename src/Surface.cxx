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
#include <assert.h>
#include <math.h>
#include <SDL.h>
#include <stdexcept>
#include "cute_png.h"
#include "Surface.h"
#include "System.h"

using namespace Amoebax;

///
/// \brief Surface's default constructor.
///
/// Makes an empty (NULL) surface.
///
Surface::Surface (void):
    m_SDLSurface (0)
{
}

///
/// \brief Creates a new surface with an already loaded SDL surface.
///
/// \param SDLSurface The SDL surface that the class will have
///                   ownership of.
///
Surface::Surface (SDL_Surface *SDLSurface):
    m_SDLSurface (SDLSurface)
{
}

///
/// \brief Surface's copy constructor.
///
/// \param surface The surface to copy from.
///
Surface::Surface (const Surface &surface):
    m_SDLSurface (0)
{
    if ( 0 != surface.m_SDLSurface )
    {
        SDL_Surface *tempSDLSurface =
            SDL_CreateRGBSurface (0,
                                  surface.m_SDLSurface->w,
                                  surface.m_SDLSurface->h,
                                  surface.m_SDLSurface->format->BitsPerPixel,
                                  surface.m_SDLSurface->format->Rmask,
                                  surface.m_SDLSurface->format->Gmask,
                                  surface.m_SDLSurface->format->Bmask,
                                  surface.m_SDLSurface->format->Amask);
        if ( NULL == tempSDLSurface )
        {
            throw std::runtime_error (SDL_GetError ());
        }
        if ( SDL_BlitSurface (surface.m_SDLSurface, NULL, tempSDLSurface, NULL) < 0 )
        {
            throw std::runtime_error (SDL_GetError ());
        }
        std::swap (m_SDLSurface, tempSDLSurface);
        SDL_FreeSurface (tempSDLSurface);
    }
}

///
/// \brief Blits the whole surface to the left-top corner of destination.
///
/// \param destination The distantion surface to blit this surface to.
///
void
Surface::blit (SDL_Surface *destination)
{
    blit (0, 0, getWidth (), getHeight (), 0, 0, destination);
}

///
/// \brief Blits the whole surface to a position in destination.
///
/// \param destinationX The X position where to blit the source into the
///                     destination.
/// \param destinationY The Y position where to blit the source into the
///                     destination.
/// \param destination The destination surface.
///
void
Surface::blit (uint16_t destinationX, uint16_t destinationY,
              SDL_Surface *destination)
{
    blit (0, 0, getWidth (), getHeight (), destinationX, destinationY,
          destination);
}

///
/// \brief Blits a part of the surface into another surface.
///
/// \param sourceX The X origin of the source surface to blit.
/// \param sourceY The X origin of the source surface to blit.
/// \param sourceWidth The width of the source surface part to blit.
/// \param sourceHeight The height of the source surface part to blit.
/// \param destinationX The X position where to blit the source into the
///                     destination.
/// \param destinationY The Y position where to blit the source into the
///                     destination.
/// \param destination The destination surface.
///
void
Surface::blit (uint16_t sourceX, uint16_t sourceY,
               uint16_t sourceWidth, uint16_t sourceHeight,
               uint16_t destinationX, uint16_t destinationY,
               SDL_Surface *destination)
{
    SDL_Rect sourceRect;
    sourceRect.x = sourceX;
    sourceRect.y = sourceY;
    sourceRect.w = sourceWidth;
    sourceRect.h = sourceHeight;

    SDL_Rect destinationRect;
    destinationRect.x = destinationX;
    destinationRect.y = destinationY;
    destinationRect.w = sourceWidth;
    destinationRect.h = sourceHeight;

    SDL_BlitSurface (m_SDLSurface, &sourceRect, destination, &destinationRect);
    System::getInstance ().invalidateScreenRegion (&destinationRect);
}

///
/// \brief Surface's destructor.
///
/// Frees the memory used by the image's surface.
///
Surface::~Surface (void)
{
    SDL_FreeSurface (m_SDLSurface);
}

///
/// \brief Gets the height of the surface.
///
/// \return Surface's height.
///
uint16_t
Surface::getHeight (void) const
{
    return toSDLSurface ()->h;
}


///
/// \brief Gets the pixel color from a surface's position.
///
/// \param x The x position to get the color from.
/// \param y The y position to get the color from.
/// \return The color at surface's \a x, \a y position.
///
uint32_t
Surface::getPixel (uint16_t x, uint16_t y)
{
    assert (x < m_SDLSurface->w && "Tried to get a color outside the surface.");
    assert (y < m_SDLSurface->h && "Tried to get a color outside the surface.");

    uint32_t pixelColor = 0;
    SDL_LockSurface (m_SDLSurface);
    switch (m_SDLSurface->format->BytesPerPixel)
    {
        case 1:
            pixelColor = static_cast<uint32_t>(*(static_cast<uint8_t *>(m_SDLSurface->pixels) + y * m_SDLSurface->pitch + x));
            break;
        case 2:
            pixelColor = static_cast<uint32_t>(*(static_cast<uint16_t *>(m_SDLSurface->pixels) + y * m_SDLSurface->pitch / 2 + x));
            break;
        case 3:
            {
            uint8_t *pixels = static_cast<uint8_t *>(m_SDLSurface->pixels) +
                              y * m_SDLSurface->pitch +
                              x * m_SDLSurface->format->BytesPerPixel;
            uint8_t red = *(pixels + m_SDLSurface->format->Rshift / 8);
            uint8_t green = *(pixels + m_SDLSurface->format->Gshift / 8);
            uint8_t blue = *(pixels + m_SDLSurface->format->Bshift / 8);
            pixelColor = SDL_MapRGB (m_SDLSurface->format, red, green, blue);
            }
            break;
        case 4:
            pixelColor = *(static_cast<uint32_t *>(m_SDLSurface->pixels) +
                           y * m_SDLSurface->pitch / 4 + x);
            break;
    }
    SDL_UnlockSurface (m_SDLSurface);

    return pixelColor;
}


///
/// \brief Gets the width of the surface.
///
/// \return Surface's width.
///
uint16_t
Surface::getWidth (void) const
{
    return toSDLSurface ()->w;
}


///
/// \brief Creates a new surface from a file.
///
/// \param fileName The file name to use to load the surface from.
/// \return The Surface class with the file loaded.
///
Surface *
Surface::fromFile (std::string fileName)
{
    cp_image_t img = cp_load_png (fileName.c_str ());
    if ( 0 == img.pix )
    {
        throw std::runtime_error (cp_error_reason);
    }

    SDL_Surface *loadedSurface =
        SDL_CreateRGBSurfaceWithFormatFrom (img.pix, img.w, img.h, 32,
                                            img.w * 4, SDL_PIXELFORMAT_RGBA32);
    if ( NULL == loadedSurface )
    {
        throw std::runtime_error (SDL_GetError ());
    }

    // HACK: allow SDL freeing the pixel data
    loadedSurface->flags &= ~SDL_PREALLOC;

    return new Surface (loadedSurface);
/*
    // FIXME: add back when different format needed
    SDL_Surface *convertedSurface =
        SDL_ConvertSurfaceFormat ( loadedSurface, SDL_PIXELFORMAT_RGBA32, 0 );
    SDL_FreeSurface ( loadedSurface );
    if ( NULL == convertedSurface )
    {
        throw std::runtime_error (SDL_GetError ());
    }
    return new Surface (convertedSurface);
*/
}

///
/// \brief Creates a new surface as a copy of the screen.
///
/// \return The Surface object with a copy of the screen.
///
Surface *
Surface::fromScreen (void)
{
    SDL_Surface *screen = System::getInstance ().getScreenSDLSurface ();
    // FIXME: hardcoding RGBA order
    SDL_Surface *copy =
        SDL_CreateRGBSurfaceWithFormat (0,
                                        screen->w, screen->h, 32,
                                        SDL_PIXELFORMAT_RGBA32);
    if ( 0 == copy )
    {
        throw std::runtime_error (SDL_GetError ());
    }
    SDL_BlitSurface (screen, NULL, copy, NULL);

    return new Surface (copy);
}

///
/// \brief Surface's assign operator.
///
/// \param surface The surface to assign to the current surface.
///
Surface &
Surface::operator= (const Surface &surface)
{
    Surface tempSurface (surface);
    swap (*this, tempSurface);

    return *this;
}

///
/// \brief Scales the image using an scale factor.
///
/// \param scaleFactor The scale factor to use to resize the image.
///
void
Surface::resize (float scaleFactor)
{
    // Can only resize 32-bit surfaces.
    assert ( 0 != m_SDLSurface && "Tried to resize an unloaded surface" );
    assert ( 32 == m_SDLSurface->format->BitsPerPixel &&
             "Tried to resize a non 32-bit surface." );

    // If the scale factor is approximaltey 1.0f, then we don't need
    // to do anything!.
    if ( fabs (scaleFactor - 1.0f) < 1e-3 )
    {
        return;
    }

    // Create the new scaled surface.
    SDL_Surface *scaledSDLSurface =
        SDL_CreateRGBSurface (m_SDLSurface->flags,
                              static_cast<int>(m_SDLSurface->w * scaleFactor),
                              static_cast<int>(m_SDLSurface->h * scaleFactor),
                              m_SDLSurface->format->BitsPerPixel,
                              m_SDLSurface->format->Rmask,
                              m_SDLSurface->format->Gmask,
                              m_SDLSurface->format->Bmask,
                              m_SDLSurface->format->Amask);
    if ( 0 == scaledSDLSurface )
    {
        throw std::runtime_error (SDL_GetError ());
    }

    // Now, for each pixel in the scaled surface, get the corresponding
    // coordinate on the original surface and using a bilinear filter,
    // smooth the color for the scaled pixel.
    SDL_LockSurface (m_SDLSurface);
    SDL_LockSurface (scaledSDLSurface);

    uint32_t *scaledPixels = static_cast<uint32_t *>(scaledSDLSurface->pixels);
    uint32_t *originalPixels = static_cast<uint32_t *>(m_SDLSurface->pixels);
    int scaledPitch = scaledSDLSurface->pitch / sizeof (uint32_t);
    int originalPitch = m_SDLSurface->pitch / sizeof (uint32_t);
    for ( int scaledY = 0 ; scaledY < scaledSDLSurface->h ; ++scaledY )
    {
        float originalY = scaledY / scaleFactor;
        int destinationY = static_cast<int>(floorf (originalY));
        float yRatio = originalY - destinationY;
        float yOpposite = 1 - yRatio;

        uint32_t *currentScaledPixel = scaledPixels + scaledY * scaledPitch;

        for ( int scaledX = 0 ; scaledX < scaledSDLSurface->w ; ++scaledX )
        {
            float scaledAlpha = 0.0f;
            float scaledBlue = 0.0f;
            float scaledGreen = 0.0f;
            float scaledRed = 0.0f;

            float originalX = scaledX / scaleFactor;
            int destinationX = static_cast<int>(floorf (originalX));
            float xRatio = originalX - destinationX;
            float xOpposite = 1 - xRatio;

            uint8_t originalAlpha = 0;
            uint8_t originalBlue = 0;
            uint8_t originalGreen = 0;
            uint8_t originalRed = 0;

            // Left-top pixel.
            SDL_GetRGBA (originalPixels[destinationY * originalPitch + destinationX],
                         m_SDLSurface->format,
                         &originalRed, &originalGreen, &originalBlue,
                         &originalAlpha);
            scaledAlpha += originalAlpha * xOpposite * yOpposite;
            scaledBlue += originalBlue * xOpposite * yOpposite;
            scaledGreen += originalGreen * xOpposite * yOpposite;
            scaledRed += originalRed * xOpposite * yOpposite;

            // Right-top pixel.
            SDL_GetRGBA (originalPixels[destinationY * originalPitch + destinationX + 1],
                         m_SDLSurface->format,
                         &originalRed, &originalGreen, &originalBlue,
                         &originalAlpha);
            scaledAlpha += originalAlpha * xRatio * yOpposite;
            scaledBlue += originalBlue * xRatio * yOpposite;
            scaledGreen += originalGreen * xRatio * yOpposite;
            scaledRed += originalRed * xRatio * yOpposite;

            // Left-bottom pixel.
            SDL_GetRGBA (originalPixels[(destinationY + 1) * originalPitch + destinationX],
                         m_SDLSurface->format,
                         &originalRed, &originalGreen, &originalBlue,
                         &originalAlpha);
            scaledAlpha += originalAlpha * xOpposite * yRatio;
            scaledBlue += originalBlue * xOpposite * yRatio;
            scaledGreen += originalGreen * xOpposite * yRatio;
            scaledRed += originalRed * xOpposite * yRatio;

            // Right-bottom pixel.
            SDL_GetRGBA (originalPixels[(destinationY + 1) * originalPitch + destinationX + 1],
                         m_SDLSurface->format,
                         &originalRed, &originalGreen, &originalBlue,
                         &originalAlpha);
            scaledAlpha += originalAlpha * xRatio * yRatio;
            scaledBlue += originalBlue * xRatio * yRatio;
            scaledGreen += originalGreen * xRatio * yRatio;
            scaledRed += originalRed * xRatio * yRatio;

            *currentScaledPixel =
                SDL_MapRGBA (scaledSDLSurface->format,
                             static_cast<uint8_t>(scaledRed),
                             static_cast<uint8_t>(scaledGreen),
                             static_cast<uint8_t>(scaledBlue),
                             static_cast<uint8_t>(scaledAlpha));
            currentScaledPixel++;
        }
    }

    SDL_UnlockSurface (scaledSDLSurface);
    SDL_UnlockSurface (m_SDLSurface);

    // Done. Swap the surfaces and free the original surface.
    std::swap (m_SDLSurface, scaledSDLSurface);
    SDL_FreeSurface (scaledSDLSurface);
}

///
/// \brief Sets the alpha value.
///
/// \param alpha The alpha value to set to the surface.
///
void
Surface::setAlpha (uint8_t alpha)
{
    SDL_SetSurfaceAlphaMod (toSDLSurface (), alpha);
}

///
/// \brief Sets the transparent color.
///
/// \param colorKey The color that will be transparent while blitting.
///
void
Surface::setColorKey (uint32_t colorKey)
{
    SDL_SetColorKey (m_SDLSurface, SDL_TRUE, colorKey);
}

///
/// \brief Swaps two surfaces.
///
/// \param lhs The left hand side surface of the swap operation.
/// \param rhs The right hand side surface of the swap operation.
///
void
Surface::swap (Surface &lhs, Surface &rhs)
{
    std::swap (lhs.m_SDLSurface, rhs.m_SDLSurface);
}
