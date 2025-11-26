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
#if defined (HAVE_CONFIG_H)
#include <config.h>
#endif // HAVE_CONFIG_H
#include <assert.h>
#include <SDL_mixer.h>
#include <stdexcept>
#include "Music.h"
#include "System.h"

using namespace Amoebax;

///
/// \brief Music's default constructor.
///
Music::Music (void):
    m_SDLMusic (0)
{
}

///
/// \brief Music's destructor.
///
Music::~Music (void)
{
    Mix_FreeMusic (m_SDLMusic);
}

///
/// \brief Loads a music from a file.
///
/// \param fileName The music file to load.
/// \return A new Music object with the music file loaded.
///
Music *
Music::fromFile (const std::string &fileName)
{
    Music *newMusic = new Music;
    if ( System::getInstance ().isSoundEnabled () )
    {
        newMusic->m_SDLMusic = Mix_LoadMUS (fileName.c_str ());
        if ( 0 == newMusic->m_SDLMusic )
        {
            delete newMusic;
            throw std::runtime_error (Mix_GetError ());
        }
    }
    return newMusic;
}

///
/// \brief Tells if the music is paused or not.
///
/// \return \a true if the music is paused, \a false otherwise.
///
bool
Music::isPaused (void)
{
    return 1 == Mix_PausedMusic ();
}

///
/// \brief Tells if a music is playing or not.
///
/// \note Does not check if the channel has been paused.
///
/// \return \a true if a music is playing, \a false otherwise.
///
bool
Music::isPlaying (void)
{
    return 1 == Mix_PlayingMusic ();
}

///
/// \brief Pauses the music playing.
///
void
Music::pause (void)
{
    if ( isPlaying () )
    {
        Mix_PauseMusic ();
    }
}

///
/// \brief Plays the music.
///
/// \param times The number of times to play the music. \a -1 means to
///              play the music forever.
///
void
Music::play (int times)
{
    // If the music is NULL, then the sound subsystem wasn't initialized,
    // just ignore and continue.
    if ( 0 != m_SDLMusic )
    {
        // Ignore errors. If the music can't be played is bad, but the
        // game can continue...
        Mix_PlayMusic (m_SDLMusic, times);
    }
}

///
/// \brief Unpause the music.
///
/// \note This is safe to use on halted, paused, and already
///       playing music.
///
void
Music::resume (void)
{
    Mix_ResumeMusic ();
}

///
/// \brief Stops the music.
///
void
Music::stop (void)
{
    Mix_HaltMusic ();
}
