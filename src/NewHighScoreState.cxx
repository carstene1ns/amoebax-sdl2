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
#include <sstream>
#include "File.h"
#include "HighScoreState.h"
#include "NewHighScoreState.h"
#include "Options.h"
#include "System.h"

using namespace Amoebax;

///
/// \brief Default constructor.
///
/// \param score The score that is a new high score.
///
NewHighScoreState::NewHighScoreState (uint32_t score):
    IState (),
    m_Background (nullptr),
    m_BackgroundMusic (Music::fromFile (File::getMusicFilePath ("Congratulations.ogg"))),
    m_CursorValueIndex (0),
    m_CursorValues ("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!?*-_. "),
    m_CursorVisible (true),
    m_CursorVisibleTime (k_CursorVisibleTime),
    m_HighLightFont (nullptr),
    m_Name (""),
    m_NameFont (nullptr),
    m_Score (score)
{
    loadGraphicResources ();
}

///
/// \brief Destructor.
///
NewHighScoreState::~NewHighScoreState (void)
{
}

///
/// \brief Adds the name to the high score list.
///
/// When the name as been added to the high score list, this state is removed
/// from the system and the HighScoreState gets pushed to the System's stack.
///
void
NewHighScoreState::acceptName (void)
{
    if ( m_Name.length () > 0 )
    {
        Options::getInstance ().setHighScore (getScore (), m_Name);
        System::getInstance ().removeActiveState ();
        System::getInstance ()
            .setActiveState (new HighScoreState (getScore ()));
    }
}

void
NewHighScoreState::activate (void)
{
    if ( Music::isPaused () )
    {
        Music::resume ();
    }
    else
    {
        m_BackgroundMusic->play ();
    }
}

///
/// \brief Adds the cursor character to the name and resets the cursor.
///
void
NewHighScoreState::addNewCharacter (void)
{
    if ( m_Name.length () < k_NameMaxLength )
    {
        m_Name += getCursorValue ();
        m_CursorValueIndex = 0;
        resetCursorVisibility ();
    }
}

///
/// \brief Tells if the cursor should be rendered or not.
///
/// \return \a true if the cursor is visible and should be rendered,
///         \a false otherwise.
///
bool
NewHighScoreState::isCursorVisible (void) const
{
    return m_CursorVisible && m_Name.length () < k_NameMaxLength;
}

///
/// \brief Gets the current value of the cursor.
///
/// The value of the cursor is the character that will added to
/// the name entered when selecting the next character to enter.
///
/// \return The currently selected character for the cursor.
///
const std::string
NewHighScoreState::getCursorValue (void) const
{
    return m_CursorValues.substr (m_CursorValueIndex, 1);
}

///
/// \brief Gets the time before change cursor's visibility.
///
/// \return The time, in milliseconds, that the cursor will retain its current
///         visibility state.
///
int32_t
NewHighScoreState::getCursorVisibleTime (void) const
{
    return m_CursorVisibleTime;
}

///
/// \brief Gets the score to set as high score.
///
/// \return The score that is a new high score.
///
uint32_t
NewHighScoreState::getScore (void) const
{
    return m_Score;
}

void
NewHighScoreState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
NewHighScoreState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    switch (button)
    {
        case SDL_CONTROLLER_BUTTON_A:
        case SDL_CONTROLLER_BUTTON_X:
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            addNewCharacter ();
            break;

        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            selectNextCursorValue ();
            break;

        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        case SDL_CONTROLLER_BUTTON_B:
            removePreviousCharacter ();
            break;

        case SDL_CONTROLLER_BUTTON_START:
            acceptName ();
            break;

        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            selectPreviousCursorValue ();
            break;
    }
}

void
NewHighScoreState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
NewHighScoreState::keyDown (uint32_t key)
{
    switch ( key )
    {
        case SDLK_DOWN:
            selectNextCursorValue ();
            break;

        case SDLK_ESCAPE:
            System::getInstance ().removeActiveState ();
            break;

        case SDLK_LEFT:
        case SDLK_BACKSPACE:
            removePreviousCharacter ();
            break;

        case SDLK_RETURN:
            acceptName ();
            break;

        case SDLK_RIGHT:
            addNewCharacter ();
            break;

        case SDLK_UP:
            selectPreviousCursorValue ();
            break;
    }

// TODO: old unicode stuff
#if 0
    // FIXME: We are only interessted with ASCII values.
    if ( 0 == (code & 0xff80) )
    {
        char character[2] = {toupper (static_cast<char>(code & 0x7f)), '\0'};
        std::string::size_type characterPos =
            m_CursorValues.find (std::string (character));
        if ( std::string::npos != characterPos )
        {
            m_CursorValueIndex = characterPos;
            addNewCharacter ();
        }
    }
#endif
}

void
NewHighScoreState::keyUp (uint32_t key)
{
}

///
/// \brief Loads all graphics resources.
///
void
NewHighScoreState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();

    // Load fonts.
    m_HighLightFont.reset (
            Font::fromFile (File::getFontFilePath ("fontMenuSelected")));
    m_NameFont.reset (Font::fromFile (File::getFontFilePath ("fontMenu")));

    // Load background image.
    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("newhighscore.png")));
    m_Background->resize (screenScale);

    // Write the score into the background.
    std::stringstream scoreString;
    scoreString << getScore ();
    m_NameFont->write (scoreString.str (),
                       static_cast<uint16_t>(k_ScorePosition * screenScale),
                       m_Background->toSDLSurface ());
}

void
NewHighScoreState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

///
/// \brief Removes the last character added to the name.
///
/// The cursor index is set to the index of the removed character and its
/// visibility is reset.
///
void
NewHighScoreState::removePreviousCharacter (void)
{
    if ( m_Name.length () > 0 )
    {
        const std::string::size_type newLength = m_Name.length () - 1;
        m_CursorValueIndex = m_CursorValues.find (m_Name.substr (newLength, 1));
        m_Name = m_Name.substr (0, newLength);
    }
    else
    {
        m_CursorValueIndex = 0;
    }
    resetCursorVisibility ();
}

void
NewHighScoreState::render (SDL_Surface *screen)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    m_NameFont->write (m_Name,
                       static_cast<uint16_t>(k_NamePosition * screenScale),
                       screen);
    if ( isCursorVisible () )
    {
        const uint16_t nameWidth = m_NameFont->getTextWidth (m_Name);
        const uint16_t cursorPosition =
            Options::getInstance ().getScreenWidth () / 2 + nameWidth / 2;
        m_HighLightFont->write (getCursorValue (), cursorPosition,
                                static_cast<uint16_t>(k_NamePosition *
                                                      screenScale),
                                screen);
    }
}

///
/// \brief Resets the cursor visibility.
///
/// Sets the cursor visible and the time to be visible to the default value.
///
void
NewHighScoreState::resetCursorVisibility (void)
{
    setCursorVisible (true);
    setCursorVisibleTime (k_CursorVisibleTime);
}

///
/// \brief Selects the next value for the cursor.
///
/// If the cursor is already at the last possible value, it wraps around
/// at the begining.
///
/// The cursor is set to be visible and the visibility time is reset to
/// the default.
///
void
NewHighScoreState::selectNextCursorValue (void)
{
    ++m_CursorValueIndex;
    if ( m_CursorValueIndex > m_CursorValues.length () )
    {
        m_CursorValueIndex = 0;
    }
    resetCursorVisibility ();
}

///
/// \brief Selects the previous value for the cursor.
///
/// If the cursor is already at the first possible value, it wraps around
/// at the end.
///
/// The cursor is set to be visible and the visibility time is reset to
/// the default.
///
void
NewHighScoreState::selectPreviousCursorValue (void)
{
    if ( m_CursorValueIndex > 0 )
    {
        --m_CursorValueIndex;
    }
    else
    {
        m_CursorValueIndex = m_CursorValues.length () - 1;
    }
    resetCursorVisibility ();
}

///
/// \brief Sets the visibility status of the cursor.
///
/// \param visible Set it to \a true to render the cursor. Set it to \a false
///                to hide the cursor.
///
void
NewHighScoreState::setCursorVisible (bool visible)
{
    m_CursorVisible = visible;
}

///
/// \brief Sets the time to retain cursor's visibility state.
///
/// \param time The time, in milliseconds, that the cursor will retain its
///             current visibility state.
///
void
NewHighScoreState::setCursorVisibleTime (int32_t time)
{
    m_CursorVisibleTime = time;
}

void
NewHighScoreState::update (uint32_t elapsedTime)
{
    setCursorVisibleTime (getCursorVisibleTime () - elapsedTime);
    if ( getCursorVisibleTime () < 0 )
    {
        setCursorVisible (!isCursorVisible ());
        do
        {
            setCursorVisibleTime (getCursorVisibleTime () +
                                  k_CursorVisibleTime);
        }
        while ( getCursorVisibleTime () < 0 );
    }
}

void
NewHighScoreState::videoModeChanged (void)
{
    loadGraphicResources ();
}
