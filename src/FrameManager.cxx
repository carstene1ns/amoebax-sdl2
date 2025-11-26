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
#include <SDL.h>
#include "FrameManager.h"

using namespace Amoebax;

// Class static members.
uint32_t FrameManager::m_ElapsedTime = 0;
float FrameManager::m_FrameRate = 1000.0f / 10.0f;
uint32_t FrameManager::m_LastTicks = 0;
uint32_t FrameManager::m_MeanElapsedTime = 0;
uint32_t FrameManager::m_NumUpdates = 0;
uint32_t FrameManager::m_TotalElapsedTime = 0;

// The numner of milliseconds the elased time can be.
static const uint32_t k_MinElapsedTime = 10;
// The number of milliseconds to wait to compute the mean elapsed time.
static const uint32_t k_TimeToComputeMean = 4096;

namespace
{
    ///
    /// \brief Clamps a value between a minimum and a maximum.
    ///
    /// \param value The value to clamp.
    /// \param min The minimum value that \p value can have.
    /// \param max The maximum value that \p value can have.
    /// \return \p max if \p value is greater than \p max.
    ///         \p min if \p value is less than \p min.
    ///         \p value otherwise.
    ///
    template <typename type> type
    clamp (type value, type min, type max)
    {
        if ( value < min )
        {
            return min;
        }
        else if ( value > max )
        {
            return max;
        }
        return value;
    }
}

///
/// \brief Gets the number of milliseconds from the start of the program.
///
/// \return The milliseconds since the start of the program.
///
uint32_t
FrameManager::getCurrentTime (void)
{
    return SDL_GetTicks ();
}

///
/// \brief Gets the number of milliseconds between the two previous updates.
///
/// \return The milliseconds between the two last consecutive calls to update().
///
uint32_t
FrameManager::getElapsedTime (void)
{
    return m_ElapsedTime;
}

///
/// \brief Initializes the frame manager.
///
/// \param frameRate The expected frame rate. Default value is 10 FPS.
///
void
FrameManager::init (float frameRate)
{
    m_ElapsedTime = 0;
    m_FrameRate = 1000.0f / frameRate;
    m_LastTicks = getCurrentTime ();
    m_MeanElapsedTime = static_cast<uint32_t> (m_FrameRate);
    m_NumUpdates = 0;
    m_TotalElapsedTime = 0;
}

///
/// \brief Updates the frame manager.
///
/// If the time elapsed from the last call to this function
/// is not the expected time for the give frame rate in init(),
/// this function makes the application wait the time necessary to accomplish
/// the exoected frame rate.
///
void
FrameManager::update ()
{
    uint32_t elapsedTime = getCurrentTime () - m_LastTicks;
    // Accumulate the elapsed time period and the number of updates.
    m_TotalElapsedTime += elapsedTime;
    ++m_NumUpdates;
    // Clamp the elapsed time between a minimum and the mean of the
    // previous computed period, to prevent a peak elapsed time much
    // greater (like, 600ms or so)
    m_ElapsedTime = clamp (elapsedTime, k_MinElapsedTime, m_MeanElapsedTime);
    // If the period is over, compute its mean elapsed time.
    if ( m_TotalElapsedTime > k_TimeToComputeMean )
    {
        m_MeanElapsedTime = m_TotalElapsedTime / m_NumUpdates + k_MinElapsedTime;
        m_TotalElapsedTime = 0;
        m_NumUpdates = 0;
    }
    m_LastTicks = getCurrentTime ();
    if ( static_cast<uint32_t>(m_FrameRate) > elapsedTime )
    {
        SDL_Delay (static_cast<uint32_t>(m_FrameRate) - m_ElapsedTime);
    }
}
