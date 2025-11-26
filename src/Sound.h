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
#if !defined (AMOEBAX_SOUND_H)
#define AMOEBAX_SOUND_H

#include <SDL_mixer.h>
#include <string>

namespace Amoebax
{
    ///
    /// \class Sound
    /// \brief Sound effect.
    ///
    class Sound
    {
        public:
            ~Sound (void);
            static Sound *fromFile (const std::string &fileName);
            void play (int times = 0);
            void stop (void);

        private:
            // A sound can onluy created through fromFile().
            Sound (void);
            // Sounds can't be copied.
            ///
            /// \brief Copy constructor.
            ///
            /// \note This constructor is left unimplemented because we
            /// don't want copied of Sound objects. Don't use it.
            ///
            Sound (const Sound &);

            ///
            /// \brief Assigment Operator.
            ///
            /// \note This operator is left unimplemented because we
            /// don't want copies of Sound objects. Don't use it.
            ///
            Sound &operator= (const Sound &);

            /// The channel the sound is playing at.
            int m_Channel;
            /// The actual SDL sound.
            Mix_Chunk *m_SDLSound;
    };
}

#endif // !AMOEBAX_SOUND_H
