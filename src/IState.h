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
#if !defined (AMOEBAX_ISTATE_H)
#define AMOEBAX_ISTATE_H

#include <memory>
#include <stdint.h>
#include "Font.h"
#include "Music.h"
#include "Sound.h"
#include "Surface.h"
#if defined (IS_GP2X_HOST)
#include "gp2x.h"
#endif // IS_GP2X_HOST

// Forward declarations.
#if !defined (_SDL_video_h)
struct SDL_Rect;
struct SDL_Surface;
#endif // !_SDL_video_h

namespace Amoebax
{
    ///
    /// \class IState.
    /// \brief Game state interface.
    ///
    /// This virtual class is the parent of all game's states: main
    /// menu, configuration menu, in-game play, etc…
    ///
    /// It only serves as a way for the System class to manage them
    /// all with an unique interface.
    ///
    class IState
    {
        public:
            ///
            /// \brief IState default constructor.
            ///
            IState (void) { }

            ///
            /// \brief IState destructor.
            ///
            inline virtual ~IState (void) { }

            ///
            /// \brief The state becomes active.
            ///
            /// This is called by the System class when
            /// the state becomes the active state (i.e., is
            /// going to be executed for the first time or after
            /// another state is removed from the stack of states
            /// and this one becomes the active.)
            ///
            virtual void activate (void) { }

            ///
            /// \brief A joystick axis motion event was received.
            ///
            /// \param joystick The joystick index that caused the event.
            /// \param axis The axis index that caused the event.
            /// \param value The current axis value. The range is between
            ///              -32768 and 32767.
            ///
            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value) = 0;

            ///
            /// \brief A joystick button down event was received.
            ///
            /// \param joystick The joystick index that caused the event.
            /// \param button The button that was pressed.
            ///
            virtual void joyDown (uint8_t joystick, uint8_t button) = 0;

            ///
            /// \brief A joystick button was released.
            ///
            /// \param joystick The joysticj index that caused the event.
            /// \param button The button that was released.
            ///
            virtual void joyUp (uint8_t joystick, uint8_t button) = 0;

#if !defined (IS_GP2X_HOST)
            ///
            /// \brief A key down event was received.
            ///
            /// \param key The symbol of the key that was pressed.
            ///            See SDLKey for possible values.
            ///
            virtual void keyDown (uint32_t key) = 0;

            ///
            /// \brief A key release event was received.
            ///
            /// \param key The symbol of the key that was released.
            ///            See SDLKey for possible values.
            ///
            virtual void keyUp (uint32_t key) = 0;
#endif // !IS_GP2X_HOST

            ///
            /// \brief Redraw a part of the background.
            ///
            /// This is called by the System class each frame to
            /// tell the state which part of the background must
            /// be redraw because was drawn on top of it by
            /// other surfaces other than the background.
            ///
            /// \param region The region of the background to redraw.
            /// \param screen The screen surface to where redraw the background.
            ///
            virtual void redrawBackground (SDL_Rect *region,
                                           SDL_Surface *screen) = 0;

            ///
            /// \brief Tells if the state should be paused or now.
            ///
            /// When the system is going to be paused, calls this
            /// function to know if the state should be paused or not.
            /// Normally, this is true for "game" states because it doesn't
            /// make sense to "pause" a menu.
            ///
            /// \return \a true if the state should be paused if it's
            ///         active and the system must pause, \a false otherwise.
            ///
            inline virtual bool shouldBePaused (void) { return false; }

            ///
            /// \brief A key was converted to an unicode code.
            ///
            /// This is called by the System class when a \a true
            /// is passed to enableUnicodeTranslation() member function
            /// of System.
            ///
            /// When a key press could be transformed to a unicode code
            /// successfully, this function is called with the code.
            ///
            /// \note Translating to unicode incurs an overhead per key press,
            ///       so use with care.
            ///
            /// \param code The unicode character of the key pressed.
            ///
            virtual void unicodeCharacterPressed (uint16_t code) { }

            ///
            /// \brief The state should update its logic.
            ///
            /// This function is called once per frame to give
            /// to the state the opportunity to update its own logic.
            /// During this time, the state should not render its state
            /// and should not take "too long", since while updating no
            /// events can be processed.
            ///
            /// \param elapsedTime The time that elapsed from the last call
            ///                    to this function.
            ///
            virtual void update (uint32_t elapsedTime) = 0;

            ///
            /// \brief The state should render itself.
            ///
            /// This function is called, obviously, once per frame to
            /// give to the state to opportunity of rendering itself.
            ///
            /// This function is separate from the update() function
            /// because it can be possible for a given state to render
            /// the previous state (think a pause state that renders a
            /// part of the game but not updating the logic.)
            ///
            /// \param screen The screen surface to where render the state.
            ///
            virtual void render (SDL_Surface *screen) = 0;

            ///
            /// \brief The video mode has changed.
            ///
            /// This function is called whenever the video mode has changed,
            /// so the states can reload the graphics appropiate for the
            /// new mode.
            ///
            virtual void videoModeChanged (void) = 0;
    };
}

#endif // !AMOEBAX_ISTATE_H
