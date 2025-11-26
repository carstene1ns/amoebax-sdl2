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
#if !defined (AMOEBAX_MAIN_MENU_STATE_H)
#define AMOEBAX_MAIN_MENU_STATE_H

#include "IState.h"

namespace Amoebax
{
    ///
    /// \class MainMenuState.
    /// \brief The main menu state.
    ///
    class MainMenuState: public IState
    {
        public:
            MainMenuState (void);
            virtual ~MainMenuState (void);

            virtual void activate (void);
            virtual void joyMotion (uint8_t joystick, uint8_t axis,
                                    int16_t value);
            virtual void joyDown (uint8_t joystick, uint8_t button);
            virtual void joyUp (uint8_t joystick, uint8_t button);
#if !defined (IS_GP2X_HOST)
            virtual void keyDown (uint32_t key);
            virtual void keyUp (uint32_t key);
#endif // !IS_GP2X_HOST
            virtual void redrawBackground (SDL_Rect *region, SDL_Surface *screen);
            virtual void render (SDL_Surface *screen);
            virtual void update (uint32_t elapsedTime);
            virtual void videoModeChanged (void);

        private:
            /// The time, in ms., until the demo state is triggered.
            static const int32_t k_TimeToDemo = 30000;

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
                    /// \param title The option's title.
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
                    /// The options should perfom its action here.
                    ///
                    virtual void operator() (void) = 0;

                private:
                    /// Option's title.
                    std::string m_Title;
            };

            ///
            /// \class CreditsOption.
            /// \brief Credits menu option.
            ///
            class CreditsOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The option's title.
                    ///
                    CreditsOption (const std::string &title): IOption (title) { }
                    virtual void operator() (void);
            };

            ///
            /// \class ExitOption.
            /// \brief Exit menu option.
            ///
            class ExitOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The option's title.
                    ///
                    ExitOption (const std::string &title): IOption (title) { }
                    virtual void operator() (void);
            };

            ///
            /// \class HighScoresOption.
            /// \brief High scores option.
            ///
            class HighScoresOption: public IOption
            {
                public:
                    ///
                    /// \brief Defaults constructor.
                    ///
                    /// \param title The option's title.
                    ///
                    HighScoresOption (const std::string &title):
                        IOption (title) { }
                    virtual void operator() (void);
            };

            ///
            /// \class NormalOption.
            /// \brief Normal menu option.
            ///
            class NormalOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The option's title.
                    ///
                    NormalOption (const std::string &title): IOption (title) { }
                    virtual void operator() (void);
            };

            ///
            /// \class OptionsOption.
            /// \brief Options menu option.
            ///
            class OptionsOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The option's title.
                    ///
                    OptionsOption (const std::string &title): IOption (title) { }
                    virtual void operator() (void);
            };

            ///
            /// \class TournamentOption.
            /// \brief Tournament menu option.
            ///
            class TournamentOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The option's title.
                    ///
                    TournamentOption (const std::string &title): IOption (title) { }
                    virtual void operator() (void);
            };

            ///
            /// \class TrainingOption.
            /// \brief Training menu option.
            ///
            class TrainingOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The option's title.
                    ///
                    TrainingOption (const std::string &title): IOption (title) { }
                    virtual void operator() (void);
            };

            ///
            /// \brief Copy constructor.
            ///
            /// \note This constructor is left unimplemented because we
            /// don't want copied of MainMenuState objects. Don't use it.
            ///
            MainMenuState (const MainMenuState &);

            ///
            /// \brief Assigment operator.
            ///
            /// \note This operator is left unimplemented because we don't
            /// want copied of MainMenuState objects. Don't use it.
            ///
            MainMenuState &operator= (const MainMenuState &);

            void activateMenuOption (void);
            int32_t getTimeToDemo (void) const;
            void loadGraphicResources (void);
            void resetTimeToDemo (void);
            void selectExitOption (void);
            void selectNextMenuOption (void);
            void selectPreviousMenuOption (void);
            void setTimeToDemo (int32_t time);

            /// Menu's background.
            std::auto_ptr<Surface> m_Background;
            /// Menu's Background music.
            std::auto_ptr<Music> m_BackgroundMusic;
            /// Menu's font.
            std::auto_ptr<Font> m_Font;
            /// Menu's font selected.
            std::auto_ptr<Font> m_FontSelected;
            /// Menu Options.
            std::vector<IOption *> m_MenuOptions;
            /// Current selected option.
            std::vector<IOption *>::iterator m_SelectedOption;
            /// The remaining time until the demo is activated.
            int32_t m_TimeToDemo;

    };
}

#endif // !AMOEABX_MAIN_MENU_STATE_H
