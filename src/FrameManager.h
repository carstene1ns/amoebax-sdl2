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
#if !defined (AMOEBAX_FRAME_MANAGER_H)
#define AMOEBAX_FRAME_MANAGER_H

#include <stdint.h>

namespace Amoebax
{
    ///
    /// \class FrameManager.
    /// \brief Manages the system's frame rate.
    /// \note This class is not thread safe.
    ///
    class FrameManager
    {
        public:
            static uint32_t getCurrentTime (void);
            static uint32_t getElapsedTime (void);
            static void init (float frameRate = 10.0f);
            static void update (void);

        private:
            // This class it not instanciable.
            ///
            /// \brief Default constructor.
            ///
            FrameManager (void);
            ///
            /// \brief Destructor.
            ///
            ~FrameManager (void);
            ///
            /// \brief Copy constructor.
            ///
            /// \note This constructor is left unimplemented because we
            /// don't want copied of FrameManager. Don't use it.
            ///
            /// \param object The source object to make a copy from.
            ///
            FrameManager (const FrameManager &object);
            ///
            /// \brief Assigment operator.
            ///
            /// \note This operator is left unimplemented because we
            /// don't want copied of FrameManager. Don't use it.
            ///
            /// \param object The source object to make a copy from.
            /// \return A reference to the current object.
            ///
            FrameManager &operator= (const FrameManager &object);

            /// The number of elapsed milliseconds from the last update.
            static uint32_t m_ElapsedTime;
            /// Expected frame rate.
            static float m_FrameRate;
            /// The number of clock ticks from the last update.
            static uint32_t m_LastTicks;
            /// The mean elapsed time from the previous period.
            static uint32_t m_MeanElapsedTime;
            /// The number of time the update() function was called the last period.
            static uint32_t m_NumUpdates;
            /// The number if elapsed milliseconds from the last mean computation.
            static uint32_t m_TotalElapsedTime;
    };
}

#endif // !AMOEBAX_FRAME_MANAGER_H
