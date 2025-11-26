//
// Cross-platform free Puyo-Puyo clone.
// Copyright (C) 2006, 2007 Emma's Software
//
// Based on SFont, which is Copyright (C) 2003 Karl Bartel
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
#if !defined (AMOEBAX_FONT_H)
#define AMOEBAX_FONT_H

#include <memory>
#include <string>
#include <vector>
#include <SDL.h>
#include "Surface.h"

// Forward declarations.
struct SDL_Surface;

namespace Amoebax
{
    ///
    /// \class Font.
    /// \brief Holds a bitmap font and its characters' information.
    ///
    class Font
    {
        public:
            Font (const Font &font);
            ~Font (void);

            uint16_t getHeight (void) const;
            uint16_t getTextWidth (const std::string &text) const;
            static Font *fromFile (const std::string &fileName);
            void write (const std::string &text, uint16_t x, uint16_t y,
                        SDL_Surface *destination) const;
            void write (const std::string &text, uint16_t y,
                        SDL_Surface *destination) const;

        private:
            ///
            /// \brief The position of a character in the font's bitmap.
            ///
            /// The initial and final horizontal position of a character in
            /// the the font's bitmap. Subtracting the first value from the
            /// second gives the width of the character.
            ///
            typedef std::pair<int16_t, int16_t> Character;

            ///
            /// \brief Gets the width of a text using a given font.
            ///
            struct TextWidth:
                public std::binary_function<uint16_t, char, uint16_t>
            {
                /// The font to use to calculate the text with.
                const Font *currentFont;

                ///
                /// \brief Initializes the struct with the font to use.
                ///
                /// \param fontToUse The font to use to calculate the width
                ///                  of the text.
                ///
                TextWidth (const Font *fontToUse):
                    currentFont (fontToUse)
                {
                }

                ///
                /// \brief Calculates the width of the current character.
                ///
                /// Calculates the width of the text up to the current
                /// character.
                ///
                /// \param widthSoFar The width of the text up to the previous
                ///                   character.
                /// \param currentCharacter The current character to calculate
                ///                         the with of and accumulate to the
                ///                         \a widthSoFar.
                /// \return The text's width up to the current character.
                ///
                uint16_t
                operator() (uint16_t &widthSoFar, const char &currentCharacter)
                {
                    int16_t offset = currentCharacter - 33;
                    if ( offset < 0 ||
                         static_cast<uint16_t>(offset) > currentFont->m_CharacterInformation.size () ||
                         offset == (' ' - 33) )
                    {
                        offset = 0;
                    }
                    const Character &character =
                        currentFont->m_CharacterInformation[offset];
                    return (widthSoFar + character.second - character.first);
                }
            };

            ///
            /// \brief Writes a text, character by character, using a font.
            ///
            struct TextWrite: public std::unary_function<char, void>
            {
                /// The font to use to write the text.
                const Font *currentFont;
                /// The surface where to write the text.
                SDL_Surface *destination;
                /// The current X position.
                uint16_t x;
                /// The current Y position.
                uint16_t y;

                ///
                /// \brief Initializes the structure.
                ///
                /// \param initialX The initial X position.
                /// \param initialY The initial Y position.
                /// \param fontToUse The font to use.
                /// \param surfaceDestination the surface to write the text to.
                ///
                TextWrite (uint16_t initialX, uint16_t initialY,
                           const Font *fontToUse,
                           SDL_Surface *surfaceDestination):
                    currentFont (fontToUse),
                    destination (surfaceDestination),
                    x (initialX),
                    y (initialY)
                {
                }

                ///
                /// \brief Writes a single character of the text.
                ///
                /// \param currentCharacter The current character to write.
                ///
                void
                operator () (const char &currentCharacter)
                {
                    bool writeCharacter = true;
                    int16_t offset = currentCharacter - 33;
                    if ( offset < 0 ||
                         static_cast<uint16_t>(offset) > currentFont->m_CharacterInformation.size () ||
                         offset == (' ' - 33) )
                    {
                        offset = 0;
                        writeCharacter = false;
                    }
                    const Character &character =
                        currentFont->m_CharacterInformation[offset];
                    if ( writeCharacter )
                    {
                        currentFont->m_FontSurface->blit (character.first, 0,
                                character.second - character.first,
                                currentFont->getHeight (),
                                x, y, destination);
                    }
                    x += character.second - character.first;
                }
            };

            // Only copy constructor and using fromFile() functions
            // ara available to create a Font object.
            Font (void);
            ///
            /// \brief Assigment operator.
            ///
            /// \note This operator is left unimplemented because we don't
            /// want to copy Font objects. Don't use it.
            ///
            /// \param font The object to make the copy from.
            ///
            Font &operator= (const Font &font);

            void init (void);

            /// Characters' space information.
            std::vector<Character> m_CharacterInformation;
            /// The actual font surface.
            std::auto_ptr<Surface> m_FontSurface;
    };

    ///
    /// \brief Gets the font's height.
    ///
    /// \return The font's height in pixels.
    ///
    inline uint16_t
    Font::getHeight (void) const
    {
        return m_FontSurface->toSDLSurface ()->h;
    }
}

#endif // !AMOEBAX_FONT_H
