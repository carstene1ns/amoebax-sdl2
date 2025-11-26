//
// Cross-platform free Puyo-Puyo clone.
// Copyright (C) 2006, 2007 Emma's software
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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
#if !defined (AMOEBAX_CONTROL_SETUP_STATE_H)
#define AMOEBAX_CONTROL_SETUP_STATE_H

#include <vector>
#include "Font.h"
#include "IState.h"
#include "Joystick.h"
#include "Options.h"
#include "Surface.h"

namespace Amoebax
{
    ///
    /// \class ControlSetupState
    /// \brief Allows to change the players' control.
    ///
    class ControlSetupState: public IState
    {
        public:
            ControlSetupState (Options::PlayerControls &leftControls,
                               Options::PlayerControls &rightControls);

            virtual void joyMotion (uint8_t joystick, uint8_t axis, int16_t value);
            virtual void joyDown (uint8_t joystick, uint8_t button);
            virtual void joyUp (uint8_t joystick, uint8_t button);
#if !defined (IS_GP2X_HOST)
            virtual void keyDown (uint32_t key);
            virtual void keyUp (uint32_t key);
#endif // !IS_GP2X_HOST
            virtual void redrawBackground (SDL_Rect *region, SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void render (SDL_Surface *screen);
            virtual void videoModeChanged (void);

        private:
            void applyPlayersControls (void);
            void activateCurrentControl (void);
            void assignJoystickToCurrentControl (int16_t joystick);
            void assignKeyToCurrentControl (uint32_t key);
            void cancelWaitingForInput (void);
            uint16_t getSelectedControl (void) const;
            uint16_t getSelectedGlobalControl (void) const;
            uint16_t getSelectedPlayer (void) const;
            int32_t getVisibleTime (void) const;
            void drawControls (bool keyboard, uint16_t offset, uint16_t x,
                               float screenScale, SDL_Surface *screen);
            void drawJoystickControl (int16_t control, uint16_t x,
                                      uint16_t controlIndex, float scale,
                                      Font *font,
                                      SDL_Surface *screen);
            void drawKeyboardControl (uint32_t control, uint16_t x,
                                      uint16_t controlIndex, float scale,
                                      Font *font,
                                      SDL_Surface *screen);
            bool isCurrentControlVisible (void) const;
            bool isJoystickIndexValid (uint8_t joystick) const;
            bool isKeyValidForControl (uint32_t key) const;
            bool isRegularMenuSelected (void) const;
            bool isWaitingJoystickInput (void) const;
            bool isWaitingKeyboardInput (void) const;
            bool isWaitingForInput (void) const;
            void loadGraphicResources ();
            void readPlayersControls (void);
            void removeState (void);
            void selectCancelOrExit (void);
            void selectNextControl (void);
            void selectNextPlayer (void);
            void selectPreviousControl (void);
            void selectPreviousPlayer (void);
            void setCurrentControlVisible (bool visible);
            void setSelectedControl (uint16_t control);
            void setSelectedPlayer (uint16_t player);
            void setVisibleTime (int32_t time);
            void setWaitingForInput (bool waiting);

            /// The background image.
            std::auto_ptr<Surface> m_Background;
            /// Tells if the current selected control is visible.
            bool m_CurrentControlVisible;
            /// The normal text font.
            std::auto_ptr<Font> m_Font;
            /// The selected text font.
            std::auto_ptr<Font> m_FontSelected;
            /// All players joystick controls' pointers.
            std::vector<int16_t *> m_JoyControls;
            /// All players keyboard controls' pointers.
            std::vector<uint32_t *> m_KeyControls;
            /// The left player's controls.
            Options::PlayerControls m_LeftPlayerControls;
            /// The original left player's controls.
            Options::PlayerControls &m_LeftPlayerControlsOriginal;
            /// The right player's controls.
            Options::PlayerControls m_RightPlayerControls;
            /// The original right player's controls.
            Options::PlayerControls &m_RightPlayerControlsOriginal;
            /// The currently selected control index.
            uint16_t m_SelectedControl;
            /// The currently selected player.
            uint16_t m_SelectedPlayer;
            /// Tells if the state was removed.
            bool m_StateRemoved;
            /// The time the current selected control is visible.
            int32_t m_VisibleTime;
            /// Tells if a control is waiting input.
            bool m_WaitingForInput;
    };
}

#endif // !AMOEBAX_CONTROL_SETUP_STATE_H
