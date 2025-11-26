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
#include <cstdlib>
#include <iostream>
#include <limits>
#include <SDL.h>
#include <SDL_mixer.h>
#include <sstream>
#include <stdexcept>
#include <string>
#include <algorithm>

#if defined (IS_WIN32_HOST)
#include <windows.h>
#endif // IS_WIN32_HOST
#if defined (IS_OSX_HOST)
extern "C" void OSXAlert (const char *title, const char *message);
#endif // IS_OSX_HOST

#include "FrameManager.h"
#include "FadeInState.h"
#include "FadeOutState.h"
#include "File.h"
#include "IState.h"
#include "Options.h"
#include "PauseState.h"
#include "System.h"
#include "VideoErrorState.h"

using namespace Amoebax;

/// The buffer system to use for audio.
#if defined (IS_GP2X_HOST)
static const int k_AudioBuffers = 1024; // Otherwise it gets delayed.
#else // !IS_GP2X_HOST
static const int k_AudioBuffers = 4096;
#endif // IS_GP2X_HOST
/// The numnber of audio channels.
static const int k_AudioChannels = 2;
/// The audio format to use.
static const int k_AudioFormat = AUDIO_S16SYS;
/// The audio rate to use.
static const int k_AudioRate = 44100;
/// Maximum frame rate (in FPS.)
static const float k_FrameRate = 30.0f;
/// Maximum screen's vertical resolution (in pixels.)
static const float k_ScreenMaxHeight = 960.0f;
/// Maximum screen's horizontal resolution (in pixels.)
static const float k_ScreenMaxWidth = 1280.0f;
/// The time to show the volume level (in ms.)
static const int k_VolumeDisplayTime = 1000;

System System::m_SystemInstance;

#if defined (IS_WIN32_HOST)
///
/// \brief Sets the current directory to be the executable's directory.
///
/// This is used when on Windows to be at the same directory as the
/// executable, so we can load any data as a relative path.
///
static void
changeToExecutableDirectory (void)
{
    // Get the executable path.
    TCHAR executablePath[MAX_PATH];
    if ( GetModuleFileName (NULL, executablePath, MAX_PATH - 1) > 0 )
    {
        std::string executableDirectory (executablePath);
        std::string::size_type backslashPosition = executableDirectory.rfind ("\\");
        if ( std::string::npos != backslashPosition )
        {
            executableDirectory.erase (backslashPosition);
            File::ChangeWorkDirectory (executableDirectory);
        }
    }
}
#endif // IS_WIN32_HOST

///
/// \brief System object's default constructor.
///
System::System (void):
    m_ActiveState (0),
    m_InvalidatedRegion (),
    m_PreviousActiveState (0),
    m_Screen (0),
    m_ScreenScaleFactor (1.0f),
    m_SoundEnabled (false),
    m_States (0),
    m_StatesToDelete (0),
    m_UnicodeTranslationEnabled (false)
{
}

///
/// \brief System object's destructor.
///
System::~System (void)
{
    // Deletes all states.
    std::for_each (m_StatesToDelete.begin (), m_StatesToDelete.end (),
                   DeleteObject<IState> ());
    m_StatesToDelete.clear ();
    std::for_each (m_States.begin (), m_States.end (),
                   DeleteObject<IState> ());
    m_States.clear ();
    // Closes all opened joysticks.
    std::for_each (m_Joysticks.begin (), m_Joysticks.end (),
                   SDL_JoystickClose);
    // Shuts down all SDL systems.
    if ( m_SoundEnabled )
    {
        Mix_CloseAudio ();
    }
    SDL_Quit ();
}

///
/// \brief Applies the video mode in the options.
///
void
System::applyVideoMode (void)
{
    changeVideoMode ();
}

///
/// \brief Applies the volume level in the options.
///
void
System::applyVolumeLevel (void)
{
    if ( isSoundEnabled () )
    {
        Mix_Volume (-1, Options::getInstance ().getVolumeLevel () *
                        MIX_MAX_VOLUME / Options::getMaxVolumeLevel ());
        Mix_VolumeMusic (Options::getInstance ().getVolumeLevel () *
                         MIX_MAX_VOLUME / Options::getMaxVolumeLevel ());
    }
}

///
/// \brief Sets the new video mode and notifies all states.
///
/// This function should be used when the video mode options (resolution,
/// full screen, etc...) are changed. It sets again the new video mode
/// and notifies all states (not only the active).
///
void
System::changeVideoMode (void)
{
    // Sets the new video mode from the options.
    setVideoMode ();
    // Notifies all states.
    for ( std::vector<IState *>::iterator currentState = m_States.begin () ;
          currentState < m_States.end () ; ++currentState )
    {
        (*currentState)->videoModeChanged ();
    }
    // Invalidates the whole screen.
    invalidateWholeScreen ();
}

///
/// \brief Enables or disables unicode translations.
///
/// \param enabled If set to \a true, the unicode translation of key pressed
///                is activated and send through calls to
///                IState::unicodeCharacterPressed().
///
/// \note Enabling unicode translation can incur an overhead for each key
///       pressed. So enable with care.
///
void
System::enableUnicodeTranslation (bool enabled)
{
    SDL_EnableUNICODE (enabled ? 1 : 0);
    m_UnicodeTranslationEnabled = enabled;
}

///
/// \brief Gets the factor between the maxium screen resolution and the current.
///
/// \return A factor between the maximum resolution and the current resolution.
///
float
System::getScreenScaleFactor (void)
{
    return m_ScreenScaleFactor;
}

///
/// \brief Gets the SDL surface that represents the screen.
///
/// \return The SDL surface of the screen.
///
SDL_Surface *
System::getScreenSDLSurface (void)
{
    return m_Screen;
}

///
/// \brief Initializes the system.
///
/// If anything goes wrong, this function throws a runtime_error exception.
///
void
System::init (void)
{
#if defined (IS_OSX_HOST)
    // Tell SDL to send key events to Cocoa, so shortcuts will work.
    SDL_putenv ("SDL_ENABLEAPPEVENTS=1");
#endif // IS_OSX_HOST

#if defined (IS_WIN32_HOST)
    // When running under windows, set the current directory to be
    // the executable directory.
    changeToExecutableDirectory ();
#endif // IS_WIN32_HOST

    // Initialize pseudo-random generator.
    srand ( (unsigned)time (0));

    // Initialize SDL.
    if ( SDL_Init (SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_VIDEO) < 0 )
    {
        throw std::runtime_error (SDL_GetError ());
    }
    // Sets the video mode from the configuration parameters.
    setVideoMode ();
    // Set the window's title for windowed modes.
    SDL_WM_SetCaption (PACKAGE_NAME, PACKAGE_NAME);
    // Set the maximum frame rate.
    FrameManager::init (k_FrameRate);
    // Open the audio device, if enabled.
    if ( Options::getInstance ().isSoundEnabled () )
    {
        if ( Mix_OpenAudio (k_AudioRate, k_AudioFormat, k_AudioChannels,
                            k_AudioBuffers) < 0 )
        {
            // Just print a warning, not being able to open a sound
            // device is not a complete failure, the game is still playable.
            std::cerr << "Couldn't open sound system: " << Mix_GetError ();
            std::cerr << std::endl;
        }
        else
        {
            m_SoundEnabled = true;
            applyVolumeLevel ();
        }
    }

#if defined (IS_GP2X_HOST)
    // When the program is launched from the GP2X main menu, it is possible
    // that the SDL event queue registered the button down event of the
    // "B" button. To prevent the game to think the user pressed the
    // "B" button, I retrieve a single event from the event queue and
    // just ignore it.
    {
        SDL_Event event;
        SDL_PollEvent (&event);
    }

    // Open the only joystic.
    SDL_Joystick *joystick = SDL_JoystickOpen (0);
    if ( NULL == joystick )
    {
        throw std::runtime_error (SDL_GetError ());
    }
    m_Joysticks.push_back (joystick);

    // Load the volume level display and gauge bar.
    m_VolumeDisplay.reset (
            Surface::fromFile (File::getGraphicsFilePath ("volume.png")));
    m_VolumeLevel.reset (Surface::fromFile (
                File::getGraphicsFilePath ("volume-level.png")));
    m_VolumeDisplayTime = 0;
#else // !IS_GP2X_HOST
    int numJoysticks = SDL_NumJoysticks ();
    for (int currentJoystick = 0 ; currentJoystick < numJoysticks ;
         ++currentJoystick )
    {
        SDL_Joystick *joystick = SDL_JoystickOpen (currentJoystick);
        if ( NULL != joystick )
        {
            // The joystick needs to have at least two axes and one
            // button.
            if ( 1 < SDL_JoystickNumAxes (joystick) &&
                 0 < SDL_JoystickNumButtons (joystick) )
            {
                m_Joysticks.push_back (joystick);
            }
        }
    }
#endif // IS_GP2X_HOST
}

///
/// \brief Invalidates the whole screen.
///
/// When the whole screen is invalidated, the state will redraw
/// the whole background.
///
void
System::invalidateWholeScreen (void)
{
    SDL_Rect invalidatedRect;

    invalidatedRect.h = Options::getInstance ().getScreenHeight ();
    invalidatedRect.w = Options::getInstance ().getScreenWidth ();
    invalidatedRect.x = 0;
    invalidatedRect.y = 0;
    invalidateScreenRegion (&invalidatedRect);
}

///
/// \brief Tells if the video is in fullscreen mode.
///
/// \return true if the mode video is in fullscreen and falss otherwise.
///
bool
System::isFullScreen (void)
{
    return Options::getInstance().isFullScreen();
}

///
/// \brief Tells if the sound subsystem could be initialized.
///
/// \return \a true if the sound is enabled, \a false otherwise.
///
bool
System::isSoundEnabled (void)
{
    return m_SoundEnabled;
}

///
/// \brief Tells if unicode translation is enabled.
///
/// \return \a true if unicode translation is enabled, \a false otherwise.
///
bool
System::isUnicodeTranslationEnabled (void) const
{
    return m_UnicodeTranslationEnabled;
}

///
/// \brief Sets a new PauseState as the active state.
///
void
System::pause (void)
{
    if ( 0 != m_ActiveState && m_ActiveState->shouldBePaused () )
    {
        setActiveState (new PauseState (), FadeNone);
    }
}

///
/// \brief Removes the active state from the stack.
///
/// The state doesn't get deleted at this time, it's scheduled
/// to be deleted at the end of the current frame or at the end
/// of the system class.
///
/// \param fadeOut Tells if add a fade out when the state is removed.
///
void
System::removeActiveState (bool fadeOut)
{
    if ( !m_States.empty () )
    {
        IState *stateToDelete = m_States.back ();
        // Remove the state from the active states list.
        m_States.pop_back ();
        // And add it to the list of states to delete at the end of the frame.
        m_StatesToDelete.push_back (stateToDelete);
        if ( fadeOut )
        {
            setActiveState (new FadeOutState (), FadeNone);
        }
    }
}

///
/// \brief Removes all states until the only remaining state is the main menu.
///
/// The main menu should be the first state, since this function removes
/// all states until one is left.
///
void
System::returnToMainMenu (void)
{
    while ( m_States.size () > 2 )
    {
        removeActiveState (false);
    }
    removeActiveState (true);
}

///
/// \brief Redraws the state's background.
///
void
System::redrawStateBackground (void)
{
    if ( 0 != m_ActiveState &&
         m_InvalidatedRegion.x1 < m_InvalidatedRegion.x2 &&
         m_InvalidatedRegion.y1 < m_InvalidatedRegion.y2 )
    {
        SDL_Rect redrawRegion;
        redrawRegion.h = m_InvalidatedRegion.y2 - m_InvalidatedRegion.y1;
        redrawRegion.w = m_InvalidatedRegion.x2 - m_InvalidatedRegion.x1;
        redrawRegion.x = m_InvalidatedRegion.x1;
        redrawRegion.y = m_InvalidatedRegion.y1;

        m_ActiveState->redrawBackground (&redrawRegion, getScreenSDLSurface ());

        m_InvalidatedRegion.x1 = std::numeric_limits<int16_t>::max ();
        m_InvalidatedRegion.y1 = std::numeric_limits<int16_t>::max ();
        m_InvalidatedRegion.x2 = std::numeric_limits<int16_t>::min ();
        m_InvalidatedRegion.y2 = std::numeric_limits<int16_t>::min ();
    }
}

///
/// \brief System's main loop.
///
/// This is the loop that drivers the whole system.
/// The call to this function doesn't return until the
/// system must shown down, in which case the application
/// can exit.
///
void
System::run (void)
{
    assert (0 != getScreenSDLSurface () &&
            "Tried to run the system without initializing it.");
    bool end = false;
    do
    {
        // Get the active state or set it 0 if no state is active.
        if ( m_States.empty () )
        {
            m_ActiveState = 0;
        }
        else
        {
            m_ActiveState = m_States.back ();
        }

        // If we are going too fast, slow down so we won't burn the CPU.
        FrameManager::update ();

        // If there's no active state, then we are done.
        if ( 0 == m_ActiveState )
        {
            SDL_Event quitEvent;

            quitEvent.type = SDL_QUIT;
            SDL_PushEvent (&quitEvent);
        }
        // If the current active state is different from the previously
        // active state, then activate the current state.
        else if ( m_ActiveState != m_PreviousActiveState )
        {
            m_ActiveState->activate ();
            invalidateWholeScreen ();
        }

        // Process all pending events.
        SDL_Event event;
        while ( SDL_PollEvent (&event) )
        {
            if ( SDL_QUIT == event.type )
            {
                end = true;
            }
            else if ( 0 != m_ActiveState )
            {
                switch ( event.type )
                {
                    case SDL_JOYAXISMOTION:
                        m_ActiveState->joyMotion (event.jaxis.which,
                                                  event.jaxis.axis,
                                                  event.jaxis.value);
                        break;

                    case SDL_JOYBUTTONDOWN:
#if defined (IS_GP2X_HOST)
                        if ( GP2X_BUTTON_VOLUP == event.jbutton.button )
                        {
                            Options::getInstance ().incrementVolume ();
                            m_VolumeDisplayTime = k_VolumeDisplayTime;
                            applyVolumeLevel ();
                        }
                        else if ( GP2X_BUTTON_VOLDOWN == event.jbutton.button )
                        {
                            Options::getInstance ().decrementVolume ();
                            m_VolumeDisplayTime = k_VolumeDisplayTime;
                            applyVolumeLevel ();
                        }
                        else
#endif  // IS_GP2X_HOST
                        {
                            m_ActiveState->joyDown (event.jbutton.which,
                                                    event.jbutton.button);
                        }
                        break;

                    case SDL_JOYBUTTONUP:
                        m_ActiveState->joyUp (event.jbutton.which,
                                              event.jbutton.button);
                        break;

#if !defined (IS_GP2X_HOST)
                    case SDL_ACTIVEEVENT:
                        if ( event.active.state & SDL_APPACTIVE )
                        {
                            // Window is minimized.
                            if ( 0 == event.active.gain )
                            {
                                Music::pause ();
                                pause ();
                            }
                        }
                        break;

                    case SDL_KEYDOWN:
                        // ALT+RETURN is reserved to toggle full screen
                        if ( SDLK_RETURN == event.key.keysym.sym &&
                             (event.key.keysym.mod & KMOD_ALT) != 0 )
                        {
                            toggleFullScreen ();
                        }
                        else
                        {
                            m_ActiveState->keyDown (event.key.keysym.sym);
                        }
                        // Check if unicode translation is enabled
                        // and send the unicode code if so.
                        if ( isUnicodeTranslationEnabled () &&
                             0 != event.key.keysym.unicode )
                        {
                            m_ActiveState->unicodeCharacterPressed (event.key.keysym.unicode);
                        }
                        break;

                    case SDL_KEYUP:
                        m_ActiveState->keyUp (event.key.keysym.sym);
                        break;
#endif // !IS_GP2X_HOST
                }
            }
        }

        // Update and render the active state.
        if ( 0 != m_ActiveState )
        {
            // Update the state's logic.
            m_ActiveState->update (FrameManager::getElapsedTime ());
            // Remove any previously set clip rectangle.
            SDL_SetClipRect (getScreenSDLSurface (), NULL);
            // Draw the state's background for the invalided screen region.
            redrawStateBackground ();
            // Render the state.
            m_ActiveState->render (getScreenSDLSurface ());
        }

#if defined (IS_GP2X_HOST)
        // Show the current volume level if it has been changed.
        if ( m_VolumeDisplayTime > 0 )
        {
            // Remove any previously set clip rectangle.
            SDL_SetClipRect (getScreenSDLSurface (), NULL);

            // Show the display.
            uint16_t x = Options::getInstance ().getScreenWidth () / 2 -
                         m_VolumeDisplay->getWidth () / 2;
            uint16_t y = Options::getInstance ().getScreenHeight () / 2 -
                         m_VolumeDisplay->getHeight () / 2;
            m_VolumeDisplay->blit (x, y, getScreenSDLSurface ());

            // Show the current level in white (upper part of the
            // image) and the remaining up to the maximum level
            // in gray (lower part of the image.)
            //uint8_t volumeLevel = Options::getVolumeLevel ();
            uint16_t width = m_VolumeLevel->getWidth () *
                             Options::getInstance ().getVolumeLevel () /
                             Options::getMaxVolumeLevel ();
            if ( !isSoundEnabled () )
            {
                width = 0;
            }
            x = Options::getInstance ().getScreenWidth () / 2 -
                m_VolumeLevel->getWidth () / 2;
            y += m_VolumeDisplay->getHeight () - 10 -
                 m_VolumeLevel->getHeight () / 2;
            m_VolumeLevel->blit (0, 0,
                                 width, m_VolumeLevel->getHeight () / 2,
                                 x, y, getScreenSDLSurface ());

            x += width;
            m_VolumeLevel->blit (width, m_VolumeLevel->getHeight () / 2,
                                 m_VolumeLevel->getWidth () - width,
                                 m_VolumeLevel->getHeight () / 2,
                                 x, y, getScreenSDLSurface ());

            // Update the time to show the display.
            m_VolumeDisplayTime -= FrameManager::getElapsedTime ();
        }
#endif // IS_GP2X_HOST

        // Refresh the screen.
        SDL_Flip (getScreenSDLSurface ());

        // Deletes the not longer used states.
        if ( !m_StatesToDelete.empty () )
        {
            std::for_each (m_StatesToDelete.begin (), m_StatesToDelete.end (),
                           DeleteObject<IState> ());
            m_StatesToDelete.clear ();
        }

        // Set the active state as the previous active state.
        m_PreviousActiveState = m_ActiveState;
    }
    while ( !end );
}

///
/// \brief Sets the current active state.
///
/// The System class takes ownership of the \a state class
/// and is responsable to delete it when no longer needed.
///
/// \param state The state to set as the current active state.
/// \param fade Tells which type of fade to set when the state is active.
///
void
System::setActiveState (IState *state, uint8_t fade)
{
    assert ( 0 != state && "Tried to set a NULL state as the active.");

    m_States.push_back (state);
    if ( FadeIn == (fade & FadeIn) )
    {
        m_States.push_back (new FadeInState (state));
    }
    if ( FadeOut == (fade & FadeOut) )
    {
        m_States.push_back (new FadeOutState ());
    }
}

///
/// \brief Sets the video mode reading the parameters from the options.
///
void
System::setVideoMode (void)
{
    // Check if we should go to full screen.
    uint32_t videoFlags = SDL_SWSURFACE;
    if ( Options::getInstance ().isFullScreen () )
    {
        videoFlags |= SDL_FULLSCREEN;
    }
    // Sets the video mode.
    m_Screen = SDL_SetVideoMode (Options::getInstance ().getScreenWidth (),
                                 Options::getInstance ().getScreenHeight (),
                                 Options::getInstance ().getScreenDepth (),
                                 videoFlags);
    if ( 0 == m_Screen )
    {
        // First check if the video settings are the defaults (which should
        // be the most secure) and if they aren't, reset back to them and
        // try again.
        if ( !Options::getInstance ().isVideoOptionsAtDefault () )
        {
            Options::getInstance ().setDefaultVideoOptions ();
            setVideoMode ();
            // If the system was already initialized, then tell
            // the user that her video settings couldn't be set.
            if ( !m_States.empty () )
            {
                setActiveState (new VideoErrorState (m_States.back ()),
                                FadeNone);
            }
        }
        else
        {
            throw std::runtime_error (SDL_GetError ());
        }
    }
    // Get the current screen's scale factor.
    // Take into consideration the "odd" factor we need to have when
    // using a 1024x768 screen resolution.
    if ( 1024 == m_Screen->w )
    {
        m_ScreenScaleFactor = 0.75f;
    }
    else
    {
        m_ScreenScaleFactor = std::max (m_Screen->w / k_ScreenMaxWidth,
                                        m_Screen->h / k_ScreenMaxHeight);
    }
    // Hide the cursor on the screen. Do only if the video mode is set
    // to full screen, otherwise it feels awkward to make the cursor desappear
    // when it passes over the window...
    SDL_ShowCursor (Options::getInstance ().isFullScreen () ? SDL_DISABLE :
                                                              SDL_ENABLE);
}

///
/// \brief Pressents a fatal error to the user.
///
/// \param error The actual error message.
///
void
System::showFatalError (const std::string &error)
{
    std::string title ("There was a fatal error an the application could not continue");

    std::ostringstream fullMessage;
    fullMessage << "Reinstall the application and try again." << std::endl;
    fullMessage << "If the error still occurs, please contact the" << std::endl;
    fullMessage << "authors at <" << PACKAGE_BUGREPORT << "> ";
    fullMessage << "attaching the following error message: " << std::endl;
    fullMessage << std::endl << error << std::endl;

#if defined (IS_WIN32_HOST)
    MessageBox (NULL, (title + "\r\n" + fullMessage.str ()).c_str (),
                "Amoebax", MB_ICONERROR);
#elif defined (IS_OSX_HOST)
    OSXAlert (title.c_str (), fullMessage.str ().c_str ());
#else
    std::cerr << title << std::endl;
    std::cerr << fullMessage.str () << std::endl;
#endif
}

///
/// \brief Toggles between full screen and windowed mode.
///
void
System::toggleFullScreen (void)
{
    Options::getInstance ().setFullScreen (!Options::getInstance ().isFullScreen ());
    changeVideoMode ();
}
