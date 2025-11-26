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
#if !defined (AMOEBAX_SYSTEM_H)
#define AMOEBAX_SYSTEM_H

#include <functional>
#include <algorithm>
#include <vector>

#if !defined (_SDL_video_h)
#include <SDL_video.h>
#endif // !_SDL_video_h
#if !defined (_SDL_joystick_h)
#include <SDL_joystick.h>
#endif // !_SDL_joystick_h

namespace Amoebax
{
    // Forward declarations
    class IState;
    class Surface;

    ///
    /// \struct DeleteObject
    /// \brief Deletes a single object.
    ///
    /// This struct is intended to be used as a parameter to std::for_each()
    /// while trying to delete the elements of a container that
    /// were created using the new operator. Do not use with objects
    /// created using the new[] operator.
    ///
    template <class T>
    struct DeleteObject: std::unary_function <T, void>
    {
        ///
        /// \brief Deletes an object.
        ///
        /// \param object The object to delete.
        ///
        void
        operator() (T *object)
        {
            delete object;
        }
    };

    ///
    /// \class System
    /// \brief The system class that loads and runs the other subsystems.
    ///
    class System
    {
        public:
            /// No Fade between states.
            static const uint8_t FadeNone = 0;
            /// Fade in between the current state and the next.
            static const uint8_t FadeIn = 1 << 0;
            /// Fade out between the current state and the next.
            static const uint8_t FadeOut = 1 << 1;

            ~System (void);

            void applyVideoMode (void);
            void applyVolumeLevel (void);
            void enableUnicodeTranslation (bool enabled);
            static System &getInstance (void);
            float getScreenScaleFactor (void);
            SDL_Surface *getScreenSDLSurface (void);
            void init (void);
            void invalidateScreenRegion (SDL_Rect *region);
            bool isFullScreen (void);
            bool isSoundEnabled (void);
            void pause (void);
            void removeActiveState (bool fadeOut = true);
            void returnToMainMenu (void);
            void run (void);
            void setActiveState (IState *state,
                                 uint8_t fade = FadeIn | FadeOut);
            static void showFatalError (const std::string &error);

        private:
            ///
            /// \struct Region.
            /// \brief An screen region.
            ///
            struct Region
            {
                /// The top-left X coordinate of the region.
                int16_t x1;
                /// The top-left Y coordinate of the region.
                int16_t y1;
                /// The bottom-right X coordinate of the region.
                int16_t x2;
                /// The bottom-right Y coordinate of the region.
                int16_t y2;

                ///
                /// Default constructor.
                ///
                Region (void):
                    x1 (0),
                    y1 (0),
                    x2 (0),
                    y2 (0)
                {
                }
            };

            // Prevent the System class to be instantied without
            // calling getInstance().
            System (void);

            // Prevent the System class to be copied.

            ///
            /// \brief Copy constructor.
            ///
            /// \note This constructor is left unimplemented because we
            /// don't want copies of System objects. Don't use it.
            ///
            System (const System &);

            ///
            /// \brief Assigment operator.
            ///
            /// \note This operator is left unimplemented because we don't
            /// want copies of System objects. Don't use it.
            ///
            System &operator= (const System &);

            void changeVideoMode (void);
            bool isUnicodeTranslationEnabled (void) const;
            void invalidateWholeScreen (void);
            void redrawStateBackground (void);
            void setVideoMode (void);
            void toggleFullScreen (void);

            /// The currently active state.
            IState *m_ActiveState;
            /// The screen region invalidated.
            Region m_InvalidatedRegion;
            /// The list of open joysticks.
            std::vector<SDL_Joystick *> m_Joysticks;
            /// The previous active state.
            IState *m_PreviousActiveState;
            /// The main window's screen.
            SDL_Surface *m_Screen;
            /// The factor between the maximum screen size and the current size.
            float m_ScreenScaleFactor;
            /// Tells if the sound subsystem could be enabled.
            bool m_SoundEnabled;
            /// The list of active states.
            std::vector<IState *> m_States;
            /// The list of states to delete.
            std::vector<IState *> m_StatesToDelete;
            /// The only system instance.
            static System m_SystemInstance;
            /// Tells if the unicode translation is enabled.
            bool m_UnicodeTranslationEnabled;
#if defined (IS_GP2X_HOST)
            /// The image of the volume level display.
            std::auto_ptr<Surface> m_VolumeDisplay;
            /// The time to show the level display.
            int32_t m_VolumeDisplayTime;
            /// The volume level gauge bar.
            std::auto_ptr<Surface> m_VolumeLevel;
#endif // IS_GP2X_HOST
    };

    ///
    /// \brief Gets the only instance of System.
    ///
    /// \return The only available instance of System.
    /// \warning This code is not thread-safe.
    ///
    inline System &
    System::getInstance (void)
    {
        return m_SystemInstance;
    }

    ///
    /// \brief Invalidates a region of the screen.
    ///
    /// A region becomes invalidated when an state draws a Surface other
    /// than the background. Once per frame, the System class will tell
    /// the state which region of the screen was invalidated, so it can redraw
    /// the background there.
    ///
    /// \param region The new screen region that has become invalidated.
    ///
    inline void
    System::invalidateScreenRegion (SDL_Rect *region)
    {
        m_InvalidatedRegion.x1 = std::min (m_InvalidatedRegion.x1, region->x);
        m_InvalidatedRegion.y1 = std::min (m_InvalidatedRegion.y1, region->y);
        m_InvalidatedRegion.x2 = std::max (m_InvalidatedRegion.x2,
                                           static_cast<int16_t>(region->x +
                                                                region->w));
        m_InvalidatedRegion.y2 = std::max (m_InvalidatedRegion.y2,
                                           static_cast<int16_t>(region->y +
                                                                region->h));
    }

}

#endif // !AMOEBAX_SYSTEM_H
