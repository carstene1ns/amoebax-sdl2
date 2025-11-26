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
#if !defined (AMOEBAX_OPTIONS_MENU_STATE_H)
#define AMOEBAX_OPTIONS_MENU_STATE_H

#include "IState.h"
#include "IPlayer.h"
#include "Options.h"
#include "System.h"

namespace Amoebax
{
    ///
    /// \class OptionsMenuState.
    /// \brief The options menu state.
    ///
    class OptionsMenuState: public IState
    {
        public:
            OptionsMenuState (void);
            virtual ~OptionsMenuState (void);

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
                    /// \brief Makes the option to apply its value.
                    ///
                    virtual void apply (void) { }

                    ///
                    /// \brief Gets the title of the option.
                    ///
                    /// \return The option's title.
                    ///
                    inline const std::string &getTitle (void) { return m_Title; }

                    ///
                    /// \brief Actives the option.
                    ///
                    /// This is called when the option is currently
                    /// selected and the user presses the action button.
                    ///
                    /// The options that should be activated when the
                    /// action button is pressed must inherit this member
                    /// and perform its action here.
                    ///
                    virtual void operator() (void) { }

                    ///
                    /// \brief Tells if the option is selectable or not.
                    ///
                    /// \return \a true if the user can select this option,
                    ///         \a false otherwise.
                    ///
                    virtual bool isSelectable (void) const { return true; }

                    ///
                    /// \brief Selects the next option's value.
                    ///
                    /// This is called when the user pressed the
                    /// right direction button.
                    ///
                    /// The options that have multiple values should
                    /// change to the next logical value here.
                    ///
                    virtual void next (void) { }

                    ///
                    /// \brief Selects the previous option's value.
                    ///
                    /// This is called when the user pressed the
                    /// left direction button.
                    ///
                    /// The options that have multiple values should
                    /// change the the previous logical value here.
                    ///
                    virtual void previous (void) { }

                    ///
                    /// \brief Renders the option and its values.
                    ///
                    /// The options that have multiple value must render
                    /// its own title plus the currently selected values
                    /// centered on the screen.
                    /// The remaining options must only to render their
                    /// only title centered on the screen.
                    ///
                    /// \param y The vertical screen position to draw the
                    ///          option to.
                    /// \param font The font to use to render the title and
                    ///             the value, if any.
                    /// \param screen The SDL surface to render the option to.
                    ///
                    virtual void render (uint16_t y, Font *font, SDL_Surface *screen) = 0;

                private:
                    /// Option's title.
                    std::string m_Title;
            };

            ///
            /// \class ApplyOption.
            /// \brief Applies all options' changes.
            ///
            class ApplyOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The option's title.
                    /// \param options The list of options to apply.
                    ///
                    ApplyOption (const std::string &title,
                                 const std::vector<IOption *> &options):
                        IOption (title),
                        m_Options (options)
                    {
                    }
                    virtual void operator() (void);
                    virtual void render (uint16_t y, Font *font, SDL_Surface *screen);

                private:
                    /// The list of options to apply when activated.
                    const std::vector<IOption *> &m_Options;
            };

            ///
            /// \class BackOption.
            /// \brief Back menu option.
            ///
            class BackOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The option's title.
                    ///
                    BackOption (const std::string &title): IOption (title) { }
                    virtual void operator() (void);
                    virtual void render (uint16_t y, Font *font, SDL_Surface *screen);
            };

            ///
            /// \class ChangeControlKeysOption.
            /// \brief Change Control Keys menu option.
            ///
            class ChangeControlKeysOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The option's title.
                    /// \param leftControls The left player's controls.
                    /// \param rightControls The right player's controls.
                    ///
                    ChangeControlKeysOption (const std::string &title,
                                             Options::PlayerControls &leftControls,
                                             Options::PlayerControls &rightControls):
                        IOption (title),
                        m_LeftControls (leftControls),
                        m_RightControls (rightControls)
                    {
                    }

                    virtual void operator() (void);
                    virtual void render (uint16_t y, Font *font, SDL_Surface *screen);

                private:
                    /// The controls for the left player.
                    Options::PlayerControls &m_LeftControls;
                    /// The controls for the right player.
                    Options::PlayerControls &m_RightControls;
            };

            ///
            /// \class PlayerControlsOption.
            /// \brief Player Controls menu option.
            ///
            class PlayerControlsOption: public IOption
            {
                public:
                    ///
                    /// <\brief Default constructor.
                    ///
                    /// \param title The option's title.
                    /// \param side The player's side.
                    /// \param controls The player's controls to modify
                    ///
                    PlayerControlsOption (const std::string &title,
                                          IPlayer::PlayerSide side,
                                          Options::PlayerControls &controls):
                        IOption (title),
                        m_Controls (controls),
                        m_Side (side)
                    {
                    }

                    virtual void apply (void);
                    virtual void next (void);
                    virtual void previous (void);
                    virtual void render (uint16_t y, Font *font, SDL_Surface *screen);

                private:
                    /// The player controls.
                    Options::PlayerControls &m_Controls;
                    /// The player side.
                    IPlayer::PlayerSide m_Side;

            };

            ///
            /// \class ScreenModeOption.
            /// \brief Screen Mode menu option.
            ///
            class ScreenModeOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The option's title.
                    ///
                    ScreenModeOption (const std::string &title):
                        IOption (title),
                        m_FullScreen (System::getInstance().isFullScreen())
                    { }
                    virtual void apply (void);
                    virtual void next (void);
                    virtual void previous (void);
                    virtual void render (uint16_t y, Font *font, SDL_Surface *screen);

                private:
                    /// Tells if the current selected mode if full screen.
                    bool m_FullScreen;
            };

            ///
            /// \class ScreenResolutionOption.
            /// \brief Screen Resolution menu option.
            ///
            class ScreenResolutionOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The option's title.
                    ///
                    ScreenResolutionOption (const std::string &title);
                    virtual void apply (void);
                    virtual void next (void);
                    virtual void previous (void);
                    virtual void render (uint16_t y, Font *font, SDL_Surface *screen);

                private:
                    /// The currently selected screen resolution.
                    std::vector<std::pair<uint16_t, uint16_t> >::iterator m_CurrentMode;
                    /// The list of available screen resolutions.
                    std::vector<std::pair<uint16_t, uint16_t> > m_Modes;
            };

            ///
            /// \class SpaceOption.
            /// \brief Just a blank option to separate the back option.
            ///
            class SpaceOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    SpaceOption (void):
                        IOption ("")
                    { }

                    virtual bool isSelectable (void) const { return false; }
                    virtual void render (uint16_t y, Font *font, SDL_Surface *screen) { }
            };

            ///
            /// \class VolumeOption.
            /// \brief Volume menu option.
            ///
            class VolumeOption: public IOption
            {
                public:
                    ///
                    /// \brief Default constructor.
                    ///
                    /// \param title The options' title.
                    ///
                    VolumeOption (const std::string &title): IOption (title) { }
                    virtual void next (void);
                    virtual void previous (void);
                    virtual void render (uint16_t y, Font *font, SDL_Surface *screen);
            };

            ///
            /// \brief Copy constructor.
            ///
            /// \note This constructor is left unimplemented because we
            /// don't want copies of OptionsMenuState objects. Don't use it.
            ///
            OptionsMenuState (const OptionsMenuState &);

            ///
            /// \brief Assigment operator.
            ///
            /// \note This operator is left unimplemented because we don't
            /// want copied of OptionsMenuState objects. Don't use it.
            ///
            OptionsMenuState &operator= (const OptionsMenuState &);

            void activateMenuOption (void);
            void loadGraphicResources (void);
            void selectBackOption (void);
            void selectNextMenuOption (void);
            void selectPreviousMenuOption (void);

            /// Menu's background.
            std::auto_ptr<Surface> m_Background;
            /// Menu's font.
            std::auto_ptr<Font> m_Font;
            /// Menu's font selected.
            std::auto_ptr<Font> m_FontSelected;
            /// The left player's controls.
            Options::PlayerControls m_LeftControls;
            /// Menu Options.
            std::vector<IOption *> m_MenuOptions;
            /// The right player's controls.
            Options::PlayerControls m_RightControls;
            /// Current selected option.
            std::vector<IOption *>::iterator m_SelectedOption;

    };
}

#endif // !AMOEABX_OPTIONS_MENU_STATE_H
