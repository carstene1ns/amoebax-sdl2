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
#if !defined (AMOEBAX_MUSIC_H)
#define AMOEBAX_MUSIC_H

#include <SDL_mixer.h>

namespace Amoebax
{
    ///
    /// \class Music.
    /// \brief Background music.
    ///
    class Music
    {
        public:
            ~Music (void);
            static Music *fromFile (const std::string &fileName);
            static bool isPaused (void);
            static bool isPlaying (void);
            static void pause (void);
            void play (int times = -1);
            static void resume (void);
            static void stop (void);

        private:
            // A music can only be create through fromFile().
            Music (void);

            // Musics can't be copied.
            ///
            /// \brief Copy constructor.
            ///
            /// \note This constructor is left inimplemented because we
            /// don't want copies of Music objects. Don't use it.
            ///
            Music (const Music &);

            ///
            /// \brief Assigment Operator.
            ///
            /// \note This operator is left inimplemented because we
            /// don't want copies of Music objects. Don't use it.
            ///
            Music &operator= (const Music &);

            /// The actual SDL music.
            Mix_Music *m_SDLMusic;
    };
}

#endif // !AMOEBAX_MUSIC_H
