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
#if !defined (AMOEBAX_TRY_AGAIN_STATE_H)
#define AMOEBAX_TRY_AGAIN_STATE_H

#include "IState.h"

namespace Amoebax
{
    ///
    /// \class TryAgainState.
    /// \brief "Try Again" menu.
    ///
    class TryAgainState: public IState
    {
        public:
            TryAgainState (void);

            virtual void activate (void);
            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value);
            virtual void joyDown (uint8_t joystick, SDL_GameControllerButton button);
            virtual void joyUp (uint8_t joystick, SDL_GameControllerButton button);
            virtual void keyDown (uint32_t key);
            virtual void keyUp (uint32_t key);
            virtual void redrawBackground (SDL_Rect *region, SDL_Surface *screen);
            virtual void render (SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void videoModeChanged (void);

        private:
            ///
            /// \class IOption
            /// \brief A single menu option interface.
            ///
            class IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The options's title.
                    ///
                    IOption (const std::string &title): m_Title (title) { }

                    ///
                    /// \brief Destructor.
                    ///
                    inline virtual ~IOption (void) { }

                    ///
                    /// \brief Gets the option's title.
                    ///
                    /// \return The option's title.
                    ///
                    inline const std::string &getTitle (void) { return m_Title; }

                    ///
                    /// \brief Activates the option.
                    ///
                    /// This is called when the option is currently
                    /// selected and the user pressed the action button.
                    ///
                    /// The options should perform its action here.
                    ///
                    virtual void operator() (void) = 0;

                private:
                    /// Option's title.
                    std::string m_Title;
            };

            ///
            /// \class YesOption
            /// \brief Continues the game.
            ///
            class YesOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The option's title.
                    ///
                    YesOption (const std::string &title): IOption (title) { }
                    virtual void operator() (void);
            };

            ///
            /// \class NoOption.
            /// \brief Exits to main menu.
            ///
            class NoOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    NoOption (const std::string &title): IOption (title) { }
                    virtual void operator() (void);
            };

            void activateMenuOption (void);
            void loadGraphicResources (void);
            void selectNextMenuOption (void);
            void selectPreviousMenuOption (void);

            /// Background image.
            std::unique_ptr<Surface> m_Background;
            /// Menu's font.
            std::unique_ptr<Font> m_Font;
            /// Menu's font for selected items.
            std::unique_ptr<Font> m_FontSelected;
            /// Menu Options.
            std::vector<IOption *> m_MenuOptions;
            /// Current selected option.
            std::vector<IOption *>::iterator m_SelectedOption;
    };
}

#endif // !AMOEBAX_TRY_AGAIN_STATE_H
