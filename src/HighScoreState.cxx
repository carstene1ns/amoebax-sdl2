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
#include "Options.h"
#include "System.h"

using namespace Amoebax;

///
/// \brief Default constructor.
///
/// \param scoreToHighLight The score that should be highlighted.
///
HighScoreState::HighScoreState (int32_t scoreToHighLight):
    IState (),
    m_Background (nullptr),
    m_Font (nullptr),
    m_FontHighLight (nullptr),
    m_ScoreToHighLight (scoreToHighLight),
    m_StateRemoved (false)
{
    loadGraphicResources ();
}

///
/// \brief Gets the score to high light.
///
/// \return The score to high light.
///
int32_t
HighScoreState::getScoreToHighLight (void) const
{
    return m_ScoreToHighLight;
}

void
HighScoreState::joyMotion (uint8_t joystick, uint8_t axis, int16_t value)
{
}

void
HighScoreState::joyDown (uint8_t joystick, SDL_GameControllerButton button)
{
    removeHighScoreState ();
}

void
HighScoreState::joyUp (uint8_t joystick, SDL_GameControllerButton button)
{
}

void
HighScoreState::keyDown (uint32_t key)
{
    removeHighScoreState ();
}

void
HighScoreState::keyUp (uint32_t key)
{
}

///
/// \brief Loads graphic resources.
///
void
HighScoreState::loadGraphicResources (void)
{
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    m_Background.reset (
            Surface::fromFile (File::getGraphicsFilePath ("menuBackground.png")));
    m_Background->resize (screenScale);
    {

        std::unique_ptr<Surface> title (
                Surface::fromFile (File::getGraphicsFilePath ("highscores.png")));
        title->resize (screenScale);
        title->blit (Options::getInstance ().getScreenWidth () / 2 -
                     title->getWidth () / 2, 0, m_Background->toSDLSurface ());
    }

    // Load fonts.
    m_Font.reset (Font::fromFile (File::getFontFilePath ("fontMenu")));
    m_FontHighLight.reset (
            Font::fromFile (File::getFontFilePath ("fontMenuSelected")));
}

void
HighScoreState::redrawBackground (SDL_Rect *region, SDL_Surface *screen)
{
    m_Background->blit (region->x, region->y, region->w, region->h,
                        region->x, region->y, screen);

    // Draw the title and scores already, since they are part of the
    // background, as won't change.
    const std::list<HighScore> &scores =
        Options::getInstance ().getHighScoreList ();
    const float screenScale = System::getInstance ().getScreenScaleFactor ();
    uint16_t y = static_cast<uint16_t>(k_InitialYPosition * screenScale);
    for (std::list<HighScore>::const_iterator currentScore = scores.begin ();
         currentScore != scores.end (); ++currentScore)
    {
        Font *font = m_Font.get ();
        if ( getScoreToHighLight () ==
             static_cast<int32_t>(currentScore->first) )
        {
            font = m_FontHighLight.get ();
        }

        // Write the points righ-aligned.
        std::stringstream scoreString;
        scoreString << currentScore->first;
        uint16_t scoreWidth = font->getTextWidth (scoreString.str ());
        font->write (scoreString.str (),
                     static_cast<uint16_t>(k_ScorePosition * screenScale) -
                     scoreWidth, y, screen);

        // Write the name left-aligned.
        font->write(currentScore->second,
                    static_cast<uint16_t>(k_NamePosition * screenScale),
                      y, screen);
        y += font->getHeight ();
    }
}

void
HighScoreState::update (uint32_t elapsedTime)
{
}

///
/// \brief Removes the highs scores state.
///
/// Besides removing the currenly active states, checks if we didn't remove
/// it in the past. This can happen if two keys are pressed almost at the same
/// time, so this state receives the two keyDown() / joyDown() signals.
///
void
HighScoreState::removeHighScoreState (void)
{
    if ( !m_StateRemoved )
    {
        System::getInstance ().removeActiveState ();
        m_StateRemoved = true;
    }
}

void
HighScoreState::render (SDL_Surface *screen)
{
}

void
HighScoreState::videoModeChanged (void)
{
    loadGraphicResources ();
}
