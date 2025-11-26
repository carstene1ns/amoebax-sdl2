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
#include <sstream>
#include "File.h"
#include "System.h"
#include "NormalSetupState.h"
#include "NormalState.h"

using namespace Amoebax;

// The names of the characters.
static const std::string k_CharacterNames[2] = {"Kim", "Tom"};
// The number of character.
static const uint8_t k_NumCharacters = 2;
// The separation in pixels from the center to a character.
static const uint16_t k_CharacterSeparation = 200;
// The Y position of the names in pixels.
static const uint16_t k_NamesPositionY = 780;
// The Y position of the "select character" string.
static const uint16_t k_SelectCharacterPositionY = 120;

///
/// \brief Default constructor.
///
NormalSetupState::NormalSetupState (void):
    IState (),
    m_Background (nullptr),
    m_FontNormal (nullptr),
    m_FontSelected (nullptr),
    m_SelectedCharacter (0),
    m_StateRemoved (false)
{
    loadGraphicResources ();
}

void
NormalSetupState::activate (void)
{
}

///
/// \brief Gets the selected character's name.
///
/// \return The name of the selected character.
///
inline std::string
NormalSetupState::getCharacterName (void) const
{
    return k_CharacterNames[getSelectedCharacter ()];
}

///
/// \brief Gets the index of the selected character.
///
/// \return The index of the selected character.
///
inline uint8_t
NormalSetupState::getSelectedCharacter (void) const
{
    return m_SelectedCharacter;
}

void
NormalSetupState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
NormalSetupState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    switch (button)
    {
        case SDL_CONTROLLER_BUTTON_A:
        case SDL_CONTROLLER_BUTTON_X:
        case SDL_CONTROLLER_BUTTON_START:
            selectCharacter ();
            break;

        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            selectPreviousCharacter ();
            break;

        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            selectNextCharacter ();
            break;

        case SDL_CONTROLLER_BUTTON_B:
            removeState ();
            break;
    }
}

void
NormalSetupState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
NormalSetupState::keyDown (uint32_t key)
{
    switch (key)
    {
        case SDLK_ESCAPE:
            removeState ();
            break;

        case SDLK_LEFT:
            selectPreviousCharacter ();
            break;

        case SDLK_RETURN:
            selectCharacter ();
            break;

        case SDLK_RIGHT:
            selectNextCharacter ();
            break;
    }
}

void
NormalSetupState::keyUp (uint32_t key)
{
}

///
/// \brief Loads graphic resources.
///
void
NormalSetupState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    const float originalScale = 1.0f;
    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("menuBackground.png")));
    {
        std::unique_ptr<Surface> kim (Surface::fromFile (
                    File::getGraphicsFilePath ("Kim.png")));
        kim->blit (m_Background->getWidth () / 2 -
                   static_cast<uint16_t> (originalScale * k_CharacterSeparation) -
                   kim->getWidth () / 2,
                   m_Background->getHeight () / 2 -
                   kim->getHeight () / 2, m_Background->toSDLSurface ());

        std::unique_ptr<Surface> tom (Surface::fromFile (
                    File::getGraphicsFilePath ("Tom.png")));
        tom->blit (m_Background->getWidth () / 2 +
                   static_cast<uint16_t> (originalScale * k_CharacterSeparation) -
                   tom->getWidth () / 2,
                   m_Background->getHeight () / 2 -
                   tom->getHeight () / 2, m_Background->toSDLSurface ());
    }
    m_Background->resize (screenScale);

    m_FontNormal.reset (Font::fromFile (File::getFontFilePath ("fontMenu")));
    m_FontSelected.reset (
            Font::fromFile (File::getFontFilePath ("fontMenuSelected")));

    const std::string selectString = "select your character";
    m_FontNormal->write (selectString,
                         m_Background->getWidth () / 2 -
                         m_FontNormal->getTextWidth (selectString) / 2,
                         static_cast<uint16_t> (screenScale *
                                                k_SelectCharacterPositionY),
                         m_Background->toSDLSurface ());
}

void
NormalSetupState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
NormalSetupState::render (SDL_Surface *screen)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    Font *leftFont = m_FontNormal.get ();
    Font *rightFont = m_FontNormal.get ();

    if ( 0 == getSelectedCharacter () )
    {
        leftFont = m_FontSelected.get ();
    }
    else
    {
        rightFont = m_FontSelected.get ();
    }

    leftFont->write (k_CharacterNames[0],
                     m_Background->getWidth () / 2 -
                     static_cast<uint16_t> (screenScale * k_CharacterSeparation) -
                     leftFont->getTextWidth (k_CharacterNames[0]) / 2,
                     static_cast<uint16_t> (screenScale * k_NamesPositionY),
                     screen);
    rightFont->write (k_CharacterNames[1],
                     m_Background->getWidth () / 2 +
                     static_cast<uint16_t> (screenScale * k_CharacterSeparation) -
                     leftFont->getTextWidth (k_CharacterNames[1]) / 2,
                     static_cast<uint16_t> (screenScale * k_NamesPositionY),
                     screen);
}

///
/// \brief Selects the currently selected character as the player's character.
///
void
NormalSetupState::selectCharacter (void)
{
    removeState (false);
    System::getInstance ().setActiveState (
            new NormalState (getCharacterName ()), System::FadeNone);
}

///
/// \brief Selects the next available character.
///
void
NormalSetupState::selectNextCharacter (void)
{
    setSelectedCharacter ((getSelectedCharacter () + 1) % k_NumCharacters);
}

///
/// \brief Selects the previous available character.
///
void
NormalSetupState::selectPreviousCharacter (void)
{
    if ( 0 == getSelectedCharacter () )
    {
        setSelectedCharacter (k_NumCharacters - 1);
    }
    else
    {
        setSelectedCharacter (getSelectedCharacter () - 1);
    }
}

///
/// \brief Sets the index of the selected character.
///
/// \param character The index of the character to select.
///
inline void
NormalSetupState::setSelectedCharacter (uint8_t character)
{
    m_SelectedCharacter = character;
}

///
/// \brief Removes the state.
///
void
NormalSetupState::removeState (bool fadeOff)
{
    if ( !m_StateRemoved )
    {
        System::getInstance ().removeActiveState (fadeOff);
        m_StateRemoved = true;
    }
}

void
NormalSetupState::update (uint32_t elapsedTime)
{
}

void
NormalSetupState::videoModeChanged (void)
{
    loadGraphicResources ();
}
