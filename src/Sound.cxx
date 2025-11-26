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
#include <stdexcept>
#include "Sound.h"
#include "System.h"

using namespace Amoebax;

///
/// \brief Sound's default constructor.
///
Sound::Sound (void):
    m_Channel (-1),
    m_SDLSound (0)
{
}

///
/// \brief Sound's destructor.
///
Sound::~Sound (void)
{
    stop ();
    Mix_FreeChunk (m_SDLSound);
}

///
/// \brief Loads a sound from a file.
///
/// \param fileName The sound file to load.
/// \return A new Sound object with the sound file loaded.
///
Sound *
Sound::fromFile (const std::string &fileName)
{
    Sound *newSound = new Sound;
    if ( System::getInstance ().isSoundEnabled () )
    {
        newSound->m_SDLSound = Mix_LoadWAV (fileName.c_str ());
        if ( 0 == newSound->m_SDLSound )
        {
            delete newSound;
            throw std::runtime_error (Mix_GetError ());
        }
    }
    return newSound;
}

///
/// \brief Plays the sound.
///
/// \param times The number of times to play the sound. \a -1 means to
///              play the sound forever. Passing \a 1 plays the sound twice.
///
void
Sound::play (int times)
{
    // If the sound is NULL, then the sound subsystem wasn't initialized,
    // just ignore and continue.
    if ( 0 != m_SDLSound )
    {
        // Ignore errors. If the sound can't be player is bad, but the
        // game can continue...
        m_Channel = Mix_PlayChannel (-1, m_SDLSound, times);
    }
}

///
/// \brief Stops the sound.
///
void
Sound::stop (void)
{
    // TODO
}
