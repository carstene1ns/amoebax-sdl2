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
#include "TournamentSetupState.h"
#include "TournamentState.h"

using namespace Amoebax;

static const uint16_t k_CharacterImageX = 993;
static const uint16_t k_CharacterImageY = 412;
static const uint8_t k_ComputerFacesRow = 2;
static const uint16_t k_ComputerFacesRowY = 730;
static const uint8_t k_ComputerCharactersColumns = 6;
static const uint16_t k_ComputerCharactersLabelY = 645;
static const uint16_t k_FaceHeight = 128;
static const uint16_t k_FaceSpan = 154;
static const uint8_t k_FacesPerRow = 7;
static const uint16_t k_FaceStartX = 160;
static const uint16_t k_FaceWidth = 128;
static const uint8_t k_FemaleFacesRow = 0;
static const uint16_t k_FemaleFacesRowY = 350;
static const uint8_t k_HumanCharactersColumns = 4;
static const uint16_t k_HumanCharactersLabelY = 265;
static const uint8_t k_MaleFacesRow = 1;
static const uint16_t k_MaleFacesRowY = 504;
static const uint8_t k_NumCharacters = 14;
static const uint8_t k_NumCharactersRows = 3;
static const uint16_t k_SelectPlayerX = 160;
static const uint16_t k_SelectPlayerY = 190;
static const int32_t k_TimeToWait = 2500;

///
/// \brief Default constructor.
///
/// \param players The number of players.
///
TournamentSetupState::TournamentSetupState (uint8_t players):
    IState (),
    m_Background (nullptr),
    m_CurrentPlayer (0),
    m_Font (nullptr),
    m_NumPlayers (players),
    m_Characters (),
    m_SelectedCharacters (),
    m_SelectedCol (0),
    m_SelectedRow (0),
    m_Selection (nullptr),
    m_TimeToWait (k_TimeToWait)
{
    m_Characters[0].name = "Kim";
    m_Characters[1].name = "Sasha";
    m_Characters[2].name = "Brooke";
    m_Characters[3].name = "Lem";
    m_Characters[4].name = "Tom";
    m_Characters[5].name = "Ed";
    m_Characters[6].name = "Gary";
    m_Characters[7].name = "Nicholas";
    m_Characters[8].name = "K.Quita";
    m_Characters[9].name = "Angus";
    m_Characters[10].name = "Kerberos";
    m_Characters[11].name = "Spike";
    m_Characters[12].name = "Mr.Bones";
    m_Characters[13].name = "Pen";
    loadGraphicResources ();
}

void
TournamentSetupState::activate (void)
{
}

///
/// \brief Tells if all characters are selected.
///
/// \return \a true if all players have selected a characters,
///         \a false otherwise.
///
inline bool
TournamentSetupState::allCharactersSelected (void) const
{
    return getCurrentPlayer () == getNumPlayers ();
}

///
/// \brief Deselects the last selected character.
///
/// If there's now selected character, this function return to the main
/// menu.
///
void
TournamentSetupState::deselectCharacter (void)
{
    if ( m_SelectedCharacters.empty () )
    {
        System::getInstance ().returnToMainMenu ();
    }
    else
    {
        setSelectedCol (m_SelectedCharacters.back ().col);
        setSelectedRow (m_SelectedCharacters.back ().row);
        m_SelectedCharacters.pop_back ();
        setPreviousPlayer ();
    }
}

///
/// \brief Gets the player that currently selects a character.
///
/// \return The player index in the range of 0..getNumPlayers()-1.
///
inline uint8_t
TournamentSetupState::getCurrentPlayer (void) const
{
    return m_CurrentPlayer;
}

///
/// \brief Gets the number of players that will select a character.
///
/// \return The number of players that need to select a player.
///
inline uint8_t
TournamentSetupState::getNumPlayers (void) const
{
    return m_NumPlayers;
}

///
/// \brief Gets the time to wait before changing to the tournament state.
///
/// \return The number of milliseconds to wait before we can change
///         the current state to the tournament state.
///
inline int32_t
TournamentSetupState::getTimeToWait (void) const
{
    return m_TimeToWait;
}

///
/// \brief Gets the unscaled X position of a column of the character grid.
///
/// \param col The column to compute the X position of.
/// \return The unscaled X position of column \p col.
///
inline uint16_t
TournamentSetupState::getXPositionOfCol (uint8_t col) const
{
    return k_FaceStartX + k_FaceSpan * col;
}

///
/// \brief Gets the unscaled Y position of a row of the characters grid.
///
/// \param row The row to compute the Y position of.
/// \return The unscaled Y position of row \p row.
///
uint16_t
TournamentSetupState::getYPositionOfRow (uint8_t row) const
{
    uint16_t y;
    switch ( row )
    {
        case k_FemaleFacesRow:
            y = k_FemaleFacesRowY;
            break;

        case k_MaleFacesRow:
            y = k_MaleFacesRowY;
            break;

        case k_ComputerFacesRow:
            y = k_ComputerFacesRowY;
            break;

        default:
            y = k_FemaleFacesRowY;
            break;
    }

    return y;
}

///
/// \brief Tells if the currently selected grid position is valid.
///
/// A valid position is any position where there's no selected character.
///
bool
TournamentSetupState::isSelectedPositionValid (void) const
{
    for ( std::vector<SelectedCharacter>::const_iterator currentCharacter = m_SelectedCharacters.begin () ;
          currentCharacter != m_SelectedCharacters.end () ;
          ++currentCharacter )
    {
        if ( currentCharacter->col == getSelectedCol () &&
             currentCharacter->row == getSelectedRow () )
        {
            return false;
        }
    }
    return true;
}

void
TournamentSetupState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
TournamentSetupState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    switch (button)
    {
        case SDL_CONTROLLER_BUTTON_A:
        case SDL_CONTROLLER_BUTTON_X:
        case SDL_CONTROLLER_BUTTON_START:
            selectCharacter ();
            break;

        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            selectNextRow ();
            break;

        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            selectPreviousCol ();
            break;

        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            selectNextCol ();
            break;

        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            selectPreviousRow ();
            break;

        case SDL_CONTROLLER_BUTTON_B:
            deselectCharacter ();
            break;
    }
}

void
TournamentSetupState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
TournamentSetupState::keyDown (uint32_t key)
{
    switch (key)
    {
        case SDLK_DOWN:
            selectNextRow ();
            break;

        case SDLK_ESCAPE:
            deselectCharacter ();
            break;

        case SDLK_LEFT:
            selectPreviousCol ();
            break;

        case SDLK_RETURN:
            selectCharacter ();
            break;

        case SDLK_RIGHT:
            selectNextCol ();
            break;

        case SDLK_UP:
            selectPreviousRow ();
            break;
    }
}

void
TournamentSetupState::keyUp (uint32_t key)
{
}

///
/// \brief Gets the currently selected column.
///
/// \return The index of the column of the currently selected character.
///
inline uint8_t
TournamentSetupState::getSelectedCol (void) const
{
    return m_SelectedCol;
}

///
/// \brief Gets the currently selected character.
///
/// \return The currently selected character.
///
inline const TournamentSetupState::CharacterInfo &
TournamentSetupState::getSelectedCharacter (void) const
{
    return m_Characters[getSelectedCharacterIndex ()];
}

///
/// \brief Gets the index of the currently selected character.
///
/// \return The index of the currently selected character.
///
inline uint8_t
TournamentSetupState::getSelectedCharacterIndex (void) const
{
    return getSelectedRow () * 4 + getSelectedCol ();
}

///
/// \brief Gets the currently selelected row.
///
/// \return The index of the column of the selected character.
///
inline uint8_t
TournamentSetupState::getSelectedRow (void) const
{
    return m_SelectedRow;
}

///
/// \brief Loads graphic resources.
///
void
TournamentSetupState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    const float originalScale = 1.0f;
    const uint16_t faceHeight = k_FaceHeight;
    const uint16_t faceWidth =  k_FaceWidth ;
    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("menuBackground.png")));
    {
        std::unique_ptr<Surface> faces (
                Surface::fromFile (File::getGraphicsFilePath ("faces.png")));
        for ( uint16_t currentCharacter = 0 ; currentCharacter < 4 ;
              currentCharacter++ )
        {
            faces->blit ((currentCharacter % k_FacesPerRow) * faceWidth, 0,
                         faceWidth, faceHeight,
                         static_cast<uint16_t> (
                             getXPositionOfCol (currentCharacter) *
                             originalScale),
                         static_cast<uint16_t> (k_FemaleFacesRowY *
                                                originalScale),
                         m_Background->toSDLSurface());
        }
        for ( uint16_t currentCharacter = 4 ; currentCharacter < 8 ;
              currentCharacter++ )
        {
            faces->blit ((currentCharacter % k_FacesPerRow) * faceWidth,
                         (currentCharacter / k_FacesPerRow) * faceHeight,
                         faceWidth, faceHeight,
                         static_cast<uint16_t> (
                             getXPositionOfCol (currentCharacter - 4) *
                             originalScale),
                         static_cast<uint16_t> (k_MaleFacesRowY *
                                                originalScale),
                         m_Background->toSDLSurface());
        }
        for ( uint16_t currentCharacter = 1 ; currentCharacter < 7 ;
              currentCharacter++ )
        {
            faces->blit ((currentCharacter % k_FacesPerRow) * faceWidth,
                         faceHeight, faceWidth, faceHeight,
                         static_cast<uint16_t> (
                             getXPositionOfCol (currentCharacter - 1) *
                             originalScale),
                         static_cast<uint16_t> (k_ComputerFacesRowY *
                                                originalScale),
                         m_Background->toSDLSurface());
        }
    }
    {
        std::unique_ptr<Surface> title (
                Surface::fromFile (File::getGraphicsFilePath ("humanplayers.png")));
        title->blit (static_cast<uint16_t> (k_FaceStartX * originalScale),
                     static_cast<uint16_t> (k_HumanCharactersLabelY *
                                            originalScale),
                     m_Background->toSDLSurface ());
    }
    {
        std::unique_ptr<Surface> title (
                Surface::fromFile (File::getGraphicsFilePath ("computerplayers.png")));
        title->blit (static_cast<uint16_t> (k_FaceStartX * originalScale),
                     static_cast<uint16_t> (k_ComputerCharactersLabelY *
                                            originalScale),
                     m_Background->toSDLSurface ());
    }
    {
        std::unique_ptr<Surface> title (
                Surface::fromFile (File::getGraphicsFilePath ("tournament.png")));
        title->blit (m_Background->getWidth () / 2 -
                     title->getWidth () / 2, 0, m_Background->toSDLSurface ());
    }
    m_Background->resize (screenScale);

    for ( uint32_t currentCharacter = 0 ; currentCharacter < k_NumCharacters ;
          ++currentCharacter )
    {
        m_Characters[currentCharacter].image.reset (
                Surface::fromFile (File::getGraphicsFilePath (
                        m_Characters[currentCharacter].name + ".png")));
        m_Characters[currentCharacter].image->resize (screenScale);
    }

    m_Selection.reset (
            Surface::fromFile (File::getGraphicsFilePath ("faceSelection.png")));
    m_Selection->resize (screenScale);

    m_Font.reset (
            Font::fromFile (File::getFontFilePath ("fontMenu")));
}

void
TournamentSetupState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);
}

void
TournamentSetupState::render (SDL_Surface *screen)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();

    if ( !allCharactersSelected () )
    {
        std::ostringstream selectPlayerString;
        selectPlayerString << "Select Player ";
        selectPlayerString << getCurrentPlayer() + 1;
        m_Font->write (selectPlayerString.str(),
                       static_cast<uint16_t> (k_SelectPlayerX * screenScale),
                       static_cast<uint16_t> (k_SelectPlayerY * screenScale),
                       screen);
    }

    // Render the currently selected character's image and name.
    uint16_t x =
        static_cast<uint16_t> (k_CharacterImageX * screenScale -
                               getSelectedCharacter ().image->getWidth () / 2);
    uint16_t y =
        static_cast<uint16_t> (k_CharacterImageY * screenScale -
                               getSelectedCharacter ().image->getHeight () / 2);
    getSelectedCharacter ().image->blit (x, y, screen);
    uint16_t textWidth = m_Font->getTextWidth (getSelectedCharacter ().name);
    m_Font->write (getSelectedCharacter ().name,
                   static_cast<uint16_t> (k_CharacterImageX * screenScale -
                                          textWidth / 2),
                   y + getSelectedCharacter ().image->getHeight (), screen);


    // Render the already selected characters.
    for ( std::vector<SelectedCharacter>::const_iterator currentCharacter = m_SelectedCharacters.begin () ;
          currentCharacter != m_SelectedCharacters.end () ;
          ++currentCharacter )
    {
        x = static_cast<uint16_t> (getXPositionOfCol (currentCharacter->col) *
                                   screenScale);
        y = static_cast<uint16_t> (getYPositionOfRow (currentCharacter->row) *
                                   screenScale);

        m_Selection->blit (m_Selection->getWidth () - m_Selection->getHeight (),
                           0,
                           m_Selection->getHeight (), m_Selection->getHeight (),
                           x, y, screen);
    }
    // Render the character selection on the character grid.
    if ( !allCharactersSelected () )
    {
        x = static_cast<uint16_t> (getXPositionOfCol (getSelectedCol ()) *
                                   screenScale);
        y = static_cast<uint16_t> (getYPositionOfRow (getSelectedRow ()) *
                                   screenScale);
        m_Selection->blit (m_Selection->getHeight () * getCurrentPlayer (), 0,
                           m_Selection->getHeight (), m_Selection->getHeight (),
                           x, y, screen);
    }
}

///
/// \brief Selects the currently selected character as the player's character.
///
void
TournamentSetupState::selectCharacter (void)
{
    // If all characters are selected, then we just move
    // forward to the next state.
    if ( allCharactersSelected () )
    {
        setTimeToWait (0);
        return;
    }

    SelectedCharacter selectedCharacter;
    selectedCharacter.col = getSelectedCol ();
    selectedCharacter.index = getSelectedCharacterIndex ();
    selectedCharacter.row = getSelectedRow ();
    m_SelectedCharacters.push_back (selectedCharacter);

    setNextPlayer ();
    if ( allCharactersSelected () )
    {
        setTimeToWait (k_TimeToWait);
    }
    else
    {
        selectFirstCharacter ();
    }
}

///
/// \brief Sets the first available character as the selected.
///
void
TournamentSetupState::selectFirstCharacter (void)
{
    if ( allCharactersSelected () )
    {
        return;
    }

    setSelectedCol (0);
    setSelectedRow (0);
    if ( !isSelectedPositionValid () )
    {
        selectNextCol ();
    }
}

///
/// \brief Selects the next available column.
///
/// If the next row is behind the last one, the first column
/// of the next row is selected.
///
void
TournamentSetupState::selectNextCol (void)
{
    if ( allCharactersSelected () )
    {
        return;
    }

    if ( (getSelectedRow () != k_ComputerFacesRow &&
          getSelectedCol () < (k_HumanCharactersColumns - 1)) ||
         (getSelectedRow () == k_ComputerFacesRow &&
          getSelectedCol () < (k_ComputerCharactersColumns - 1)) )
    {
        setSelectedCol (getSelectedCol () + 1);
    }
    else
    {
        selectNextRow ();
        setSelectedCol (0);
    }
    if ( !isSelectedPositionValid () )
    {
        selectNextCol ();
    }
}

///
/// \brief Selects the next available row.
///
/// If the next row is after the last one, the first row is selected.
///
void
TournamentSetupState::selectNextRow (void)
{
    if ( allCharactersSelected () )
    {
        return;
    }

    setSelectedRow ((getSelectedRow () + 1) % k_NumCharactersRows);
    if ( getSelectedRow () != k_ComputerFacesRow &&
         getSelectedCol () > (k_HumanCharactersColumns - 1) )
    {
        setSelectedCol (k_HumanCharactersColumns - 1);
    }
    if ( !isSelectedPositionValid () )
    {
        selectNextCol ();
    }
}

///
/// \brief Selects the previous available column.
///
/// If the previous column is before the first one, the last column
/// of the previous column is selected.
///
void
TournamentSetupState::selectPreviousCol (void)
{
    if ( allCharactersSelected () )
    {
        return;
    }

    if ( getSelectedCol () > 0 )
    {
        setSelectedCol (getSelectedCol () - 1);
    }
    else
    {
        if ( getSelectedRow () == k_FemaleFacesRow )
        {
            setSelectedCol (k_ComputerCharactersColumns - 1);
        }
        else
        {
            setSelectedCol (k_HumanCharactersColumns - 1);
        }
        selectPreviousRow ();
    }
    if ( !isSelectedPositionValid () )
    {
        selectPreviousCol ();
    }
}

///
/// \brief Selectes the previous row.
///
/// If the previous row is before the first, the last column is selected.
///
void
TournamentSetupState::selectPreviousRow (void)
{
    if ( allCharactersSelected () )
    {
        return;
    }

    if ( getSelectedRow () > 0 )
    {
        setSelectedRow (getSelectedRow () - 1);
        // Since the computer characters row is the last one, we
        // can be sure that the row we are now is a human characters row.
        if ( getSelectedCol () > (k_HumanCharactersColumns - 1) )
        {
            setSelectedCol (k_HumanCharactersColumns - 1);
        }
    }
    else
    {
        setSelectedRow (k_ComputerFacesRow);
    }
    if ( !isSelectedPositionValid () )
    {
        selectNextCol ();
    }
}

///
/// \brief Increments the current player.
///
inline void
TournamentSetupState::setNextPlayer (void)
{
    ++m_CurrentPlayer;
}

///
/// \brief Decrements the current player.
///
inline void
TournamentSetupState::setPreviousPlayer (void)
{
    --m_CurrentPlayer;
}

///
/// \brief Sets the currently selected column.
///
/// \param col The column to set as selected.
///
inline void
TournamentSetupState::setSelectedCol (uint8_t col)
{
    m_SelectedCol = col;
}

///
/// \brief Sets the currently selected row.
///
/// \param row The row to set as selected.
///
inline void
TournamentSetupState::setSelectedRow (uint8_t row)
{
    m_SelectedRow = row;
}

///
/// \brief Sets the time to wait before changing to the tournament state.
///
/// \param timeToWait The time to wait in milliseconds.
///
inline void
TournamentSetupState::setTimeToWait (int32_t timeToWait)
{
    m_TimeToWait = timeToWait;
}

void
TournamentSetupState::update (uint32_t elapsedTime)
{
    if ( allCharactersSelected () )
    {
        setTimeToWait (getTimeToWait () - elapsedTime);
        if ( getTimeToWait () <= 0 )
        {
            std::vector <TournamentState::Player> tournamentPlayers;
            for (std::vector <SelectedCharacter>::const_iterator currentPlayer = m_SelectedCharacters.begin() ;
                 currentPlayer != m_SelectedCharacters.end() ; ++currentPlayer)
            {
                TournamentState::Player player;
                player.characterIndex = currentPlayer->index;
                player.name = m_Characters[currentPlayer->index].name;
                player.isComputerPlayer = k_ComputerFacesRow == currentPlayer->row;
                player.computerPlayerLevel = currentPlayer->col;
                tournamentPlayers.push_back (player);
            }
            System::getInstance ().removeActiveState (false);
            System::getInstance ().setActiveState (
                    new TournamentState (tournamentPlayers));
        }
    }

}

void
TournamentSetupState::videoModeChanged (void)
{
    loadGraphicResources ();
}
