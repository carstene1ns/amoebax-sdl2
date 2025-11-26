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
#include <algorithm>
#include <assert.h>
#include <cmath>
#include "ChainLabel.h"
#include "File.h"
#include "Grid.h"
#include "GridStatus.h"
#include "PairGenerator.h"
#include "System.h"

using namespace Amoebax;

// Constants.
static const float k_Cos[360] = {1.000000f, 0.999848f, 0.999391f, 0.998630f,
                                 0.997564f, 0.996195f, 0.994522f, 0.992546f,
                                 0.990268f, 0.987688f, 0.984808f, 0.981627f,
                                 0.978148f, 0.974370f, 0.970296f, 0.965926f,
                                 0.961262f, 0.956305f, 0.951057f, 0.945519f,
                                 0.939693f, 0.933580f, 0.927184f, 0.920505f,
                                 0.913545f, 0.906308f, 0.898794f, 0.891007f,
                                 0.882948f, 0.874620f, 0.866025f, 0.857167f,
                                 0.848048f, 0.838671f, 0.829038f, 0.819152f,
                                 0.809017f, 0.798636f, 0.788011f, 0.777146f,
                                 0.766044f, 0.754710f, 0.743145f, 0.731354f,
                                 0.719340f, 0.707107f, 0.694658f, 0.681998f,
                                 0.669131f, 0.656059f, 0.642788f, 0.629320f,
                                 0.615661f, 0.601815f, 0.587785f, 0.573576f,
                                 0.559193f, 0.544639f, 0.529919f, 0.515038f,
                                 0.500000f, 0.484810f, 0.469472f, 0.453990f,
                                 0.438371f, 0.422618f, 0.406737f, 0.390731f,
                                 0.374607f, 0.358368f, 0.342020f, 0.325568f,
                                 0.309017f, 0.292372f, 0.275637f, 0.258819f,
                                 0.241922f, 0.224951f, 0.207912f, 0.190809f,
                                 0.173648f, 0.156434f, 0.139173f, 0.121869f,
                                 0.104528f, 0.087156f, 0.069756f, 0.052336f,
                                 0.034899f, 0.017452f, 0.000000f, -0.017452f,
                                 -0.034899f, -0.052336f, -0.069756f, -0.087156f,
                                 -0.104528f, -0.121869f, -0.139173f, -0.156434f,
                                 -0.173648f, -0.190809f, -0.207912f, -0.224951f,
                                 -0.241922f, -0.258819f, -0.275637f, -0.292372f,
                                 -0.309017f, -0.325568f, -0.342020f, -0.358368f,
                                 -0.374607f, -0.390731f, -0.406737f, -0.422618f,
                                 -0.438371f, -0.453990f, -0.469472f, -0.484810f,
                                 -0.500000f, -0.515038f, -0.529919f, -0.544639f,
                                 -0.559193f, -0.573576f, -0.587785f, -0.601815f,
                                 -0.615661f, -0.629320f, -0.642788f, -0.656059f,
                                 -0.669131f, -0.681998f, -0.694658f, -0.707107f,
                                 -0.719340f, -0.731354f, -0.743145f, -0.754710f,
                                 -0.766044f, -0.777146f, -0.788011f, -0.798636f,
                                 -0.809017f, -0.819152f, -0.829038f, -0.838671f,
                                 -0.848048f, -0.857167f, -0.866025f, -0.874620f,
                                 -0.882948f, -0.891007f, -0.898794f, -0.906308f,
                                 -0.913545f, -0.920505f, -0.927184f, -0.933580f,
                                 -0.939693f, -0.945519f, -0.951057f, -0.956305f,
                                 -0.961262f, -0.965926f, -0.970296f, -0.974370f,
                                 -0.978148f, -0.981627f, -0.984808f, -0.987688f,
                                 -0.990268f, -0.992546f, -0.994522f, -0.996195f,
                                 -0.997564f, -0.998630f, -0.999391f, -0.999848f,
                                 -1.000000f, -0.999848f, -0.999391f, -0.998630f,
                                 -0.997564f, -0.996195f, -0.994522f, -0.992546f,
                                 -0.990268f, -0.987688f, -0.984808f, -0.981627f,
                                 -0.978148f, -0.974370f, -0.970296f, -0.965926f,
                                 -0.961262f, -0.956305f, -0.951057f, -0.945519f,
                                 -0.939693f, -0.933580f, -0.927184f, -0.920505f,
                                 -0.913545f, -0.906308f, -0.898794f, -0.891007f,
                                 -0.882948f, -0.874620f, -0.866025f, -0.857167f,
                                 -0.848048f, -0.838671f, -0.829038f, -0.819152f,
                                 -0.809017f, -0.798636f, -0.788011f, -0.777146f,
                                 -0.766044f, -0.754710f, -0.743145f, -0.731354f,
                                 -0.719340f, -0.707107f, -0.694658f, -0.681998f,
                                 -0.669131f, -0.656059f, -0.642788f, -0.629320f,
                                 -0.615661f, -0.601815f, -0.587785f, -0.573576f,
                                 -0.559193f, -0.544639f, -0.529919f, -0.515038f,
                                 -0.500000f, -0.484810f, -0.469472f, -0.453990f,
                                 -0.438371f, -0.422618f, -0.406737f, -0.390731f,
                                 -0.374607f, -0.358368f, -0.342020f, -0.325568f,
                                 -0.309017f, -0.292372f, -0.275637f, -0.258819f,
                                 -0.241922f, -0.224951f, -0.207912f, -0.190809f,
                                 -0.173648f, -0.156434f, -0.139173f, -0.121869f,
                                 -0.104528f, -0.087156f, -0.069756f, -0.052336f,
                                 -0.034899f, -0.017452f, -0.000000f, 0.017452f,
                                 0.034899f, 0.052336f, 0.069756f, 0.087156f,
                                 0.104528f, 0.121869f, 0.139173f, 0.156434f,
                                 0.173648f, 0.190809f, 0.207912f, 0.224951f,
                                 0.241922f, 0.258819f, 0.275637f, 0.292372f,
                                 0.309017f, 0.325568f, 0.342020f, 0.358368f,
                                 0.374607f, 0.390731f, 0.406737f, 0.422618f,
                                 0.438371f, 0.453990f, 0.469472f, 0.484810f,
                                 0.500000f, 0.515038f, 0.529919f, 0.544639f,
                                 0.559193f, 0.573576f, 0.587785f, 0.601815f,
                                 0.615661f, 0.629320f, 0.642788f, 0.656059f,
                                 0.669131f, 0.681998f, 0.694658f, 0.707107f,
                                 0.719340f, 0.731354f, 0.743145f, 0.754710f,
                                 0.766044f, 0.777146f, 0.788011f, 0.798636f,
                                 0.809017f, 0.819152f, 0.829038f, 0.838671f,
                                 0.848048f, 0.857167f, 0.866025f, 0.874620f,
                                 0.882948f, 0.891007f, 0.898794f, 0.906308f,
                                 0.913545f, 0.920505f, 0.927184f, 0.933580f,
                                 0.939693f, 0.945519f, 0.951057f, 0.956305f,
                                 0.961262f, 0.965926f, 0.970296f, 0.974370f,
                                 0.978148f, 0.981627f, 0.984808f, 0.987688f,
                                 0.990268f, 0.992546f, 0.994522f, 0.996195f,
                                 0.997564f, 0.998630f, 0.999391f, 0.999848f};

static const float k_Sin[360] = {0.000000f, 0.017452f, 0.034899f, 0.052336f,
                                 0.069756f, 0.087156f, 0.104528f, 0.121869f,
                                 0.139173f, 0.156434f, 0.173648f, 0.190809f,
                                 0.207912f, 0.224951f, 0.241922f, 0.258819f,
                                 0.275637f, 0.292372f, 0.309017f, 0.325568f,
                                 0.342020f, 0.358368f, 0.374607f, 0.390731f,
                                 0.406737f, 0.422618f, 0.438371f, 0.453990f,
                                 0.469472f, 0.484810f, 0.500000f, 0.515038f,
                                 0.529919f, 0.544639f, 0.559193f, 0.573576f,
                                 0.587785f, 0.601815f, 0.615661f, 0.629320f,
                                 0.642788f, 0.656059f, 0.669131f, 0.681998f,
                                 0.694658f, 0.707107f, 0.719340f, 0.731354f,
                                 0.743145f, 0.754710f, 0.766044f, 0.777146f,
                                 0.788011f, 0.798636f, 0.809017f, 0.819152f,
                                 0.829038f, 0.838671f, 0.848048f, 0.857167f,
                                 0.866025f, 0.874620f, 0.882948f, 0.891007f,
                                 0.898794f, 0.906308f, 0.913545f, 0.920505f,
                                 0.927184f, 0.933580f, 0.939693f, 0.945519f,
                                 0.951057f, 0.956305f, 0.961262f, 0.965926f,
                                 0.970296f, 0.974370f, 0.978148f, 0.981627f,
                                 0.984808f, 0.987688f, 0.990268f, 0.992546f,
                                 0.994522f, 0.996195f, 0.997564f, 0.998630f,
                                 0.999391f, 0.999848f, 1.000000f, 0.999848f,
                                 0.999391f, 0.998630f, 0.997564f, 0.996195f,
                                 0.994522f, 0.992546f, 0.990268f, 0.987688f,
                                 0.984808f, 0.981627f, 0.978148f, 0.974370f,
                                 0.970296f, 0.965926f, 0.961262f, 0.956305f,
                                 0.951057f, 0.945519f, 0.939693f, 0.933580f,
                                 0.927184f, 0.920505f, 0.913545f, 0.906308f,
                                 0.898794f, 0.891007f, 0.882948f, 0.874620f,
                                 0.866025f, 0.857167f, 0.848048f, 0.838671f,
                                 0.829038f, 0.819152f, 0.809017f, 0.798636f,
                                 0.788011f, 0.777146f, 0.766044f, 0.754710f,
                                 0.743145f, 0.731354f, 0.719340f, 0.707107f,
                                 0.694658f, 0.681998f, 0.669131f, 0.656059f,
                                 0.642788f, 0.629320f, 0.615661f, 0.601815f,
                                 0.587785f, 0.573576f, 0.559193f, 0.544639f,
                                 0.529919f, 0.515038f, 0.500000f, 0.484810f,
                                 0.469472f, 0.453990f, 0.438371f, 0.422618f,
                                 0.406737f, 0.390731f, 0.374607f, 0.358368f,
                                 0.342020f, 0.325568f, 0.309017f, 0.292372f,
                                 0.275637f, 0.258819f, 0.241922f, 0.224951f,
                                 0.207912f, 0.190809f, 0.173648f, 0.156434f,
                                 0.139173f, 0.121869f, 0.104528f, 0.087156f,
                                 0.069756f, 0.052336f, 0.034899f, 0.017452f,
                                 0.000000f, -0.017452f, -0.034899f, -0.052336f,
                                 -0.069756f, -0.087156f, -0.104528f, -0.121869f,
                                 -0.139173f, -0.156434f, -0.173648f, -0.190809f,
                                 -0.207912f, -0.224951f, -0.241922f, -0.258819f,
                                 -0.275637f, -0.292372f, -0.309017f, -0.325568f,
                                 -0.342020f, -0.358368f, -0.374607f, -0.390731f,
                                 -0.406737f, -0.422618f, -0.438371f, -0.453990f,
                                 -0.469472f, -0.484810f, -0.500000f, -0.515038f,
                                 -0.529919f, -0.544639f, -0.559193f, -0.573576f,
                                 -0.587785f, -0.601815f, -0.615661f, -0.629320f,
                                 -0.642788f, -0.656059f, -0.669131f, -0.681998f,
                                 -0.694658f, -0.707107f, -0.719340f, -0.731354f,
                                 -0.743145f, -0.754710f, -0.766044f, -0.777146f,
                                 -0.788011f, -0.798636f, -0.809017f, -0.819152f,
                                 -0.829038f, -0.838671f, -0.848048f, -0.857167f,
                                 -0.866025f, -0.874620f, -0.882948f, -0.891007f,
                                 -0.898794f, -0.906308f, -0.913545f, -0.920505f,
                                 -0.927184f, -0.933580f, -0.939693f, -0.945519f,
                                 -0.951057f, -0.956305f, -0.961262f, -0.965926f,
                                 -0.970296f, -0.974370f, -0.978148f, -0.981627f,
                                 -0.984808f, -0.987688f, -0.990268f, -0.992546f,
                                 -0.994522f, -0.996195f, -0.997564f, -0.998630f,
                                 -0.999391f, -0.999848f, -1.000000f, -0.999848f,
                                 -0.999391f, -0.998630f, -0.997564f, -0.996195f,
                                 -0.994522f, -0.992546f, -0.990268f, -0.987688f,
                                 -0.984808f, -0.981627f, -0.978148f, -0.974370f,
                                 -0.970296f, -0.965926f, -0.961262f, -0.956305f,
                                 -0.951057f, -0.945519f, -0.939693f, -0.933580f,
                                 -0.927184f, -0.920505f, -0.913545f, -0.906308f,
                                 -0.898794f, -0.891007f, -0.882948f, -0.874620f,
                                 -0.866025f, -0.857167f, -0.848048f, -0.838671f,
                                 -0.829038f, -0.819152f, -0.809017f, -0.798636f,
                                 -0.788011f, -0.777146f, -0.766044f, -0.754710f,
                                 -0.743145f, -0.731354f, -0.719340f, -0.707107f,
                                 -0.694658f, -0.681998f, -0.669131f, -0.656059f,
                                 -0.642788f, -0.629320f, -0.615661f, -0.601815f,
                                 -0.587785f, -0.573576f, -0.559193f, -0.544639f,
                                 -0.529919f, -0.515038f, -0.500000f, -0.484810f,
                                 -0.469472f, -0.453990f, -0.438371f, -0.422618f,
                                 -0.406737f, -0.390731f, -0.374607f, -0.358368f,
                                 -0.342020f, -0.325568f, -0.309017f, -0.292372f,
                                 -0.275637f, -0.258819f, -0.241922f, -0.224951f,
                                 -0.207912f, -0.190809f, -0.173648f, -0.156434f,
                                 -0.139173f, -0.121869f, -0.104528f, -0.087156f,
                                 -0.069756f, -0.052336f, -0.034899f, -0.017452f};

// The speed of the horizontal movement of the queue.
static const float k_QueueHorizontalSpeed = 0.10f;
// The speed of the vertical movement of the queue.
static const float k_QueueVerticalSpeed = 0.25f;
// The speed of the satellite rotation, in degress/ms.
static const float k_RotationSpeed = 0.5f;

///
/// \brief Constructor.
///
/// \param gridPositionX The X position of the grid's top-left corner.
/// \param gridPositionY The Y position of the grid's top-left corner.
/// \param queuePositionX The X position of queue's top-left corner,
///                       where the amoebas wait to fall.
/// \param queuePositionY The Y position of queue's top-left corner,
///                       where the amoebas wait to fall.
/// \param waitingGhostPositionX The X position of the top-left corner
///                              where the ghost amoebas wait to fall.
/// \param waitingGhostPositionY The Y position of the top-left corner
///                              where the ghost amoebas wait to fall.
/// \param amoebaSize The size of a single amoeba. The amoebas must be
///                   squared, so the width and the height are the same.
/// \param queueSide The side where the queue is located at.
/// \param score The initial grid's score.
/// \param layout The grid's layout.
///
Grid::Grid (uint16_t gridPositionX, uint16_t gridPositionY,
            uint16_t queuePositionX, uint16_t queuePositionY,
            uint16_t waitingGhostPositionX, uint16_t waitingGhostPositionY,
            uint16_t amoebaSize, QueueSide queueSide, uint32_t score,
            Layout layout):
    m_ActiveAmoebas (0),
    m_AmoebaSize (amoebaSize),
    m_BlinkTime (0),
    m_ChainLabels (0),
    m_CurrentStepChain (0),
    m_CurrentRotationDegrees (0.0f),
    m_DegreesUntilRotationDone (0.0f),
    m_DyingAmoebas (0),
    m_DyingTime (0),
    m_FallingPair (),
    m_Filled (false),
    m_FirstFallingPair (true),
    m_FloatingAmoebas (0),
    m_FloatingAmoebasVerticalOffset (0),
    m_Generator (0),
    m_Grid (k_GridHeight * k_GridWidth, static_cast<Amoeba *>(0)),
    m_GridPositionX (gridPositionX),
    m_GridPositionY (gridPositionY),
    m_HasNewFallingPair (false),
    m_InactiveAmoebas (0),
    m_Layout (layout),
    m_MaxFallingSpeed (false),
    m_OpponentGhostAmoebas (0),
    m_Queue (0),
    m_QueuedAmoebas (0),
    m_QueueIsMoving (false),
    m_QueueSide (queueSide),
    m_QueuePositionX (queuePositionX),
    m_QueuePositionY (queuePositionY),
    m_RemainingQueueHorizontalOffset (0),
    m_RemainingQueueVerticalOffset (0),
    m_RotationDirection (1.0f),
    m_Score (score),
    m_SilhouetteFrame (0),
    m_SilhouetteFrameDirection (-1),
    m_SilhouetteTime (0),
    m_WaitingInitialAmoebas (true),
    m_WaitingGhostAmoebas (0),
    m_WaitingGhostAmoebasNumber (0),
    m_WaitingGhostPositionX (waitingGhostPositionX),
    m_WaitingGhostPositionY (waitingGhostPositionY)
{
    // Load sounds.
    m_DieSound.reset (Sound::fromFile (File::getSoundFilePath ("die.wav")));

    // Create the waiting ghost amoebas.
    // There's only 6 waiting amoebas, but the "neighbour" state
    // of each ghost amoebas tells how many "real" ghosts will fall from
    // this amoeba (max. 5 for each.)
    for ( uint8_t currentAmoeba = 0 ; currentAmoeba < 6 ; ++currentAmoeba )
    {
        m_WaitingGhostAmoebas.push_back (new Amoeba (Amoeba::ColourGhost));
        m_WaitingGhostAmoebas[currentAmoeba]->setState (Amoeba::StateTopRightBottomLeft);
        if ( Grid::LayoutVertical == getLayout () )
        {
            m_WaitingGhostAmoebas[currentAmoeba]->setY (m_WaitingGhostPositionY);
            m_WaitingGhostAmoebas[currentAmoeba]->setX (m_WaitingGhostPositionX +
                                                        getAmoebaSize () *
                                                        currentAmoeba);
        }
        else
        {
            if ( Grid::QueueSideLeft == getQueueSide () )
            {
                m_WaitingGhostAmoebas[currentAmoeba]->setX (m_WaitingGhostPositionX -
                                                            getAmoebaSize ());
                m_WaitingGhostAmoebas[currentAmoeba]->setY (m_WaitingGhostPositionY +
                                                            getAmoebaSize () *
                                                            currentAmoeba);

            }
            else
            {
                m_WaitingGhostAmoebas[currentAmoeba]->setX (m_WaitingGhostPositionX);
                m_WaitingGhostAmoebas[currentAmoeba]->setY (m_WaitingGhostPositionY -
                                                            getAmoebaSize () *
                                                            (currentAmoeba + 1));
            }
        }
    }
}

///
/// \brief Destructor.
///
Grid::~Grid (void)
{
    std::for_each (m_ActiveAmoebas.begin (), m_ActiveAmoebas.end (),
                   DeleteObject<Amoeba> ());
    m_ActiveAmoebas.clear ();

    std::for_each (m_ChainLabels.begin (), m_ChainLabels.end (),
                   DeleteObject<ChainLabel> ());
    m_ChainLabels.clear ();

    std::for_each (m_InactiveAmoebas.begin (), m_InactiveAmoebas.end (),
                   DeleteObject<Amoeba> ());
    m_InactiveAmoebas.clear ();

    std::for_each (m_QueuedAmoebas.begin (), m_QueuedAmoebas.end (),
                   DeleteObject<Amoeba> ());
    m_QueuedAmoebas.clear ();

    std::for_each (m_WaitingGhostAmoebas.begin (), m_WaitingGhostAmoebas.end (),
                   DeleteObject<Amoeba> ());
    m_WaitingGhostAmoebas.clear ();
}

///
/// \brief Adds a new step chain label.
///
/// An step chain label is the text that appears on the grid
/// when a chain is made to let know the user which step the
/// chain was made in. The label will move a little to the top
/// and then will disappear.
///
/// \param stepChain The step in which the chain was made in. It starts
///                  from 1, not 0.
/// \param x The screen X position where the label should appear centered at.
/// \param y The screen Y position where the label should appear centered at.
///
void
Grid::addChainLabel (uint8_t stepChain, int16_t x, int16_t y)
{
    m_ChainLabels.push_back (new ChainLabel (stepChain, x, y,
                                             y - getAmoebaSize ()));
}

///
/// \brief Adds a new amoebas pair.
///
/// \param main The main amoeba of the pair.
/// \param satellite The satellite amoeba of the pair. This is the one that
///                  will rotate around \a main.
///
void
Grid::addNewPair (Amoeba *main, Amoeba *satellite)
{
    // Just add the to the list of waiting amoebas.
    m_InactiveAmoebas.push_back (main);
    m_InactiveAmoebas.push_back (satellite);
    // Set up the initial amoebas.
    // We need to how 6 amoebas for the set up: The falling pair and
    // the two waiting pairs.
    if ( m_WaitingInitialAmoebas && 8 == m_InactiveAmoebas.size () )
    {
        m_WaitingInitialAmoebas = false;
        getNextPair ();
        getNextPair ();
        setupFallingPair ();
    }
}

///
/// \brief Removes all dying amoebas.
///
/// Once all dying amoebas are deleted checks for floating amoebas and if
/// no floating amoeba is found then sets up the falling pair.
///
void
Grid::clearDyingAmoebas (void)
{
    m_DieSound->play ();

    uint8_t amoebasErased = 0;
    for (std::vector<FallingAmoeba>::iterator currentAmoeba =
            m_DyingAmoebas.begin () ;
         currentAmoeba != m_DyingAmoebas.end () ; ++currentAmoeba )
    {
        if ( Amoeba::ColourGhost != currentAmoeba->amoeba->getColour () )
        {
            ++amoebasErased;
        }
        setAmoebaAt (currentAmoeba->x, currentAmoeba->y, 0);
        m_ActiveAmoebas.remove (currentAmoeba->amoeba);
        delete currentAmoeba->amoeba;
    }
    m_DyingAmoebas.clear ();

    // Activates all chain label.
    std::for_each (getChainLabels ().begin (), getChainLabels ().end (),
                   std::mem_fun (&ChainLabel::activate));

    // Increment the score based on the number of non-ghost amoebas erased
    // and the current step chain (starting from 0.)
    uint16_t score = ((2 * amoebasErased - 4) *
                      10) << getCurrentStepChain ();
    incrementScore (score);
    incrementCurrentStepChain ();
    // Compute the number of ghost amoebas the score generates, subtracts
    // the number of waiting amoebas and then set the remaining
    // ghost amoebas as ghost amoebas for the opponent.
    uint8_t newGhostAmoebas = static_cast<uint16_t>(std::ceil (score / 70.0f));
    uint8_t remainingGhostAmoebas = removeGhostAmoebas (newGhostAmoebas);
    setOpponentGhostAmoebas (remainingGhostAmoebas);

    findFloatingAmoebas ();
    // If there's no floating amoebas, set up the falling pair.
    if ( m_FloatingAmoebas.empty () )
    {
        // If the positions above the grid of the 3rd column (from
        // the right) is occuped by an amoeba, then the grid is
        // considered filled.  If any amobea is sitting over the grid in
        // any other column, this is not considered filled.
        if ( 0 != getAmoebaAt (3, k_FirstVisibleHeight - 1) )
        {
            markAsFilled ();
        }
        else
        {
            scheduleFallingPair ();
        }
    }
}

///
/// \brief Finds floating amoebas.
///
/// A floating amoeba is an amoeaba that just lost its supporting amoeba
/// and hence "is floating" on the air and must start to fall.
///
/// Checks each and every amoeba, except for those in the last line, to
/// see if it's a floating amoeba and adds it to the m_FloatingAmoeba
/// list if so.
///
void
Grid::findFloatingAmoebas (void)
{
    // Now check which amoebas are "floating" and make them fall.
    m_FloatingAmoebasVerticalOffset = 0;
    for ( int16_t row = k_GridHeight - 2 ; row >= 0 ; --row )
    {
        for ( int16_t column = 0 ; column < k_GridWidth ; ++column )
        {
            Amoeba *amoeba = getAmoebaAt (column, row);
            if ( 0 != amoeba && 0 == getAmoebaAt (column, row + 1) )
            {
                // This amoeba no longer occupes the current position.
                setAmoebaAt (column, row, 0);
                // Now is a falling amoeba.
                FallingAmoeba floatingAmoeba;
                floatingAmoeba.amoeba = amoeba;
                floatingAmoeba.x = column;
                floatingAmoeba.y = row;
                m_FloatingAmoebas.push_back (floatingAmoeba);
                // Set the state of the current and neighbour amoebas.
                amoeba->setState (Amoeba::StateNone);
                setAmoebaStateAt (column, row, true);
            }
        }
    }
}

///
/// \brief Gets the amoeba at a given grid's position.
///
/// \param x The X position of the grid to get the amoeba from.
/// \param y The Y position of the grid to get the amoeba from.
/// \return The amoeba at position \p x and \p y or 0 if there's
///         no amoeba at this position or the position is incorrect (i.e.,
///         out of grid's limits.)
///
Amoeba *
Grid::getAmoebaAt (int16_t x, int16_t y) const
{
    if ( 0 <= x && x < k_GridWidth &&
         0 <= y && y < k_GridHeight )
    {
        return m_Grid[y * k_GridWidth + x];
    }
    return 0;
}

///
/// \brief Gets the current satellite rotation degree.
///
/// \return The current satellite rotation degree.
///
inline int16_t
Grid::getCurrentRotationDegree (void) const
{
    return static_cast<int16_t> (m_CurrentRotationDegrees);
}

///
/// \brief Gets the main amoeba of the falling pair.
///
/// \return The main falling amoeba of the falling pair.
///
Grid::FallingAmoeba
Grid::getFallingMainAmoeba (void) const
{
    return m_FallingPair.main;
}

///
/// \brief Gets the satellite amoeba of the falling pair.
///
/// \return The satellite falling amoeba of the falling pair.
///
Grid::FallingAmoeba
Grid::getFallingSatelliteAmoeba (void) const
{
    return m_FallingPair.satellite;
}

///
/// \brief Gets the main amoeba that is going to fall after the next pair.
///
/// Since this amoeba doesn't have a position, yet, we set it to
/// bet at the center of the grid at the top position.
///
/// \return The main amoeba that is going to fall after the next pair.
///
Grid::FallingAmoeba
Grid::getFollowingFallingMainAmoeba (void) const
{
    FallingAmoeba followingMain;

    followingMain.x = k_GridWidth / 2;
    followingMain.y = -1;
    std::list<FallingPair>::const_iterator pair = m_Queue.begin ();
    ++pair;
    assert (m_Queue.end () != pair &&
            "The pairs queue doesn't have two pairs" );
    followingMain.amoeba = pair->main.amoeba;

    return followingMain;
}

///
/// \brief Gets the satellite amoeba that is going to fall after the next pair.
///
/// Since this amoeba doesn't have a position, yet, we set it to
/// bet at the center of the grid at the top position.
///
/// \return The satellite amoeba that is going to fall after the next pair.
///
Grid::FallingAmoeba
Grid::getFollowingFallingSatelliteAmoeba (void) const
{
    FallingAmoeba followingSatellite;

    followingSatellite.x = k_GridWidth / 2;
    followingSatellite.y = -1;
    std::list<FallingPair>::const_iterator pair = m_Queue.begin ();
    ++pair;
    assert (m_Queue.end () != pair &&
            "The pairs queue doesn't have two pairs" );
    followingSatellite.amoeba = pair->satellite.amoeba;

    return followingSatellite;
}

///
/// \brief Gets the main amoeba that is going to fall next.
///
/// Since this amoeba doesn't have a position, yet, we set it to
/// be at the center of the grid at the top position.
///
/// \return The main amoeba that is going to fall next.
///
Grid::FallingAmoeba
Grid::getNextFallingMainAmoeba (void) const
{
    FallingAmoeba nextMain;

    nextMain.x = k_GridWidth / 2;
    nextMain.y = -1;
    std::list<FallingPair>::const_iterator pair = m_Queue.begin ();
    assert (m_Queue.end () != pair && "The pairs queue is empty" );
    nextMain.amoeba = pair->main.amoeba;

    return nextMain;
}

///
/// \brief Gets the main amoeba that is going to fall next.
///
/// Since this amoeba doesn't have a position, yet, we set it to
/// be at the center of the grid at the top position.
///
/// \return The main amoeba that is going to fall next.
///
Grid::FallingAmoeba
Grid::getNextFallingSatelliteAmoeba (void) const
{
    FallingAmoeba nextSatellite;

    nextSatellite.x = k_GridWidth / 2;
    nextSatellite.y = -2;
    std::list<FallingPair>::const_iterator pair = m_Queue.begin ();
    assert (m_Queue.end () != pair && "The pairs queue is empty" );
    nextSatellite.amoeba = pair->satellite.amoeba;

    return nextSatellite;
}

///
/// \brief Gets the next pair from the inactive list and sets it to the queue.
///
/// The next pair will be the last element in the queue, so it should
/// be called when the first element of the queue becomes the falling pair.
///
/// If the inactive amoebas list becomes empty, this function request
/// to the pair generator a new pair.
///
void
Grid::getNextPair (void)
{
    assert ( 0 != m_Generator &&
            "Tried to get the next pair without a generator.");
    assert ( 2 <= m_InactiveAmoebas.size () &&
            "Tried to get the next pair, but the inactive list is empty.");

    // Gets the next pair from the inactive list.
    Amoeba *main = m_InactiveAmoebas.front ();
    m_InactiveAmoebas.pop_front ();
    Amoeba *satellite = m_InactiveAmoebas.front ();
    m_InactiveAmoebas.pop_front ();
    // And set them to queued.
    m_QueuedAmoebas.push_back (main);
    m_QueuedAmoebas.push_back (satellite);
    // Sets they position to be the last pair on the queue.
    // Take into account the side where the queue is located at
    // to choose the offset to apply to this pair.
    uint16_t lastPairOffsetX = getAmoebaSize () / 2;
    if ( LayoutVertical == getLayout () )
    {
        if ( Grid::QueueSideLeft == getQueueSide () )
        {
            lastPairOffsetX = -lastPairOffsetX;
        }
        main->setX (getQueuePositionX () + lastPairOffsetX);
        main->setY (getQueuePositionY () + getAmoebaSize () * 3);
        satellite->setX (getQueuePositionX () + lastPairOffsetX);
        satellite->setY (getQueuePositionY () + getAmoebaSize () * 2);
    }
    else
    {
        if (Grid::QueueSideLeft == getQueueSide () )
        {
            main->setX (getQueuePositionX () - getAmoebaSize () * 4);
            main->setY (getQueuePositionY () + lastPairOffsetX);
            satellite->setX (getQueuePositionX () - getAmoebaSize () * 3);
            satellite->setY (getQueuePositionY () + lastPairOffsetX);
        }
        else
        {
            main->setX (getQueuePositionX () + getAmoebaSize () * 3);
            main->setY (getQueuePositionY () - getAmoebaSize () - lastPairOffsetX);
            satellite->setX (getQueuePositionX () + getAmoebaSize () * 2);
            satellite->setY (getQueuePositionY () - getAmoebaSize () - lastPairOffsetX);
        }
    }
    // And set up the falling pair structure for when they fall.
    FallingPair fallingPair;
    fallingPair.main.amoeba = main;
    fallingPair.main.x = 3;
    fallingPair.main.y = k_FirstVisibleHeight - 2;
    fallingPair.satellite.amoeba = satellite;
    fallingPair.satellite.x = 3;
    fallingPair.satellite.y = k_FirstVisibleHeight - 3;
    fallingPair.verticalOffset = 0;
    fallingPair.fallingTime = k_DefaultFallingTime;
    m_Queue.push_back (fallingPair);
    // Request a new pair if we are out of inactive pairs.
    if ( m_InactiveAmoebas.empty () )
    {
        m_Generator->generate ();
    }
}

///
/// \brief Gets the side of the queue.
///
/// \return The side of the grid where the queue is located at.
///
Grid::QueueSide
Grid::getQueueSide (void) const
{
    return m_QueueSide;
}

///
/// \brief Gets the queue's top-left corner X position.
///
/// \return The X position of queue's top-left corner.
///
uint16_t
Grid::getQueuePositionX (void) const
{
    return m_QueuePositionX;
}

///
/// \brief Gets the queue's top-left corner Y position.
///
/// \return The Y position of queue's top-left corner.
///
uint16_t
Grid::getQueuePositionY (void) const
{
    return m_QueuePositionY;
}

///
/// \brief Gets the current grid's state.
///
/// \return The grid's state.
///
GridStatus
Grid::getState (void) const
{
    std::vector<Amoeba::Colour> state (k_GridWidth * k_GridHeight,
                                       Amoeba::ColourNone);
    assert (state.size () == m_Grid.size () &&
            "The state's size and the grid's size doesn't match.");
    for ( uint16_t currentAmoeba = 0 ; currentAmoeba < state.size () ;
          ++currentAmoeba )
    {
        Amoeba *amoeba = m_Grid.at (currentAmoeba);
        if ( 0 != amoeba )
        {
            state.at (currentAmoeba) = amoeba->getColour ();
        }
    }
    return GridStatus (state);
}

///
/// \brief Gets the waiting ghost's top-left corner X position.
///
/// \return The X position of the waitings ghost's top-left corner.
///
uint16_t
Grid::getWaitingGhostPositionX (void) const
{
    return m_WaitingGhostPositionX;
}

///
/// \brief Gets the waiting ghost's top-left corner Y position.
///
/// \return The Y position of the waitings ghost's top-left corner.
///
uint16_t
Grid::getWaitingGhostPositionY (void) const
{
    return m_WaitingGhostPositionY;
}

///
/// \brief Checks if an amoeba at a give position is the same colour of another.
///
/// \param x The grid's X position of the amoeba to check its colour.
/// \param y The grid's Y position of the amoeba to check its colour.
/// \param amoeba The amoeba to check if its colour is the same as the
///               amoeba at \a x i \a y.
/// \return \a true if the amoeba at \a x and \a y if of the same colour as
///         \a amoeba.
///
bool
Grid::isOfSameColour (int16_t x, int16_t y, Amoeba *amoeba) const
{
    assert ( 0 != amoeba && "Tried to check the colour of a null amoeba.");

    Amoeba *amoebaInGrid = getAmoebaAt (x, y);
    if ( 0 != amoebaInGrid )
    {
        return amoebaInGrid->getColour () == amoeba->getColour ();
    }
    return false;
}

///
/// \brief Tells if the queue is moving and needs to be updated.
///
/// \return \a true if the amoebas in the queue are moving and needs to
///         be updated, \a false otherwise.
///
inline bool
Grid::isQueueMoving (void) const
{
    return m_QueueIsMoving;
}

///
/// \brief Tells if the satellite amoeba is rotating.
///
/// \return \a true if the satellite amoeba is rotating.
///         \a false otherwise.
///
inline bool
Grid::isSatelliteRotating (void) const
{
    return 0 != static_cast<int16_t> (m_DegreesUntilRotationDone);
}

///
/// \brief Increments the number of ghosts amoebas.
///
/// \param amount The amount to increment the number of ghost amoebas
///               that are waiting to fall into the grid.
///
void
Grid::incrementNumberOfWaitingGhosts (uint8_t amount)
{
    setNumberOfWaitingGhosts (getNumberOfWaitingGhosts () + amount);
    updateWaitingGhosts ();
}

///
/// \brief Sets the waiting ghost as falling amoebas.
///
void
Grid::makeGhostsFall (void)
{
    std::vector<uint8_t> emptyPositions (k_GridWidth);
    uint8_t remainingGhosts = getNumberOfWaitingGhosts ();
    int8_t remainingPositions = 0;
    int8_t y = -1;

    while ( remainingGhosts > 0 )
    {
        if ( 0 == remainingPositions )
        {
            for ( uint8_t currentPosition = 0 ; currentPosition < k_GridWidth ;
                  ++currentPosition )
            {
                emptyPositions.at (currentPosition) = currentPosition;
            }
            y -= 1;
            remainingPositions = k_GridWidth;
        }
        uint8_t horizontalPositionIndex = rand () % remainingPositions;
        int8_t x = emptyPositions.at (horizontalPositionIndex);
        emptyPositions.at (horizontalPositionIndex) =
        emptyPositions.at (remainingPositions - 1);
        --remainingPositions;

        FallingAmoeba ghostAmoeba;
        ghostAmoeba.amoeba = new Amoeba (Amoeba::ColourGhost);
        ghostAmoeba.y = y;
        ghostAmoeba.x = x;
        m_ActiveAmoebas.push_back (ghostAmoeba.amoeba);
        m_FloatingAmoebas.push_back (ghostAmoeba);

        --remainingGhosts;
    }
    setNumberOfWaitingGhosts (0);
    updateWaitingGhosts ();
}

///
/// \brief Makes a group of equal colour amoebas recursively.
///
/// \param group The group vector to save the members found to.
/// \param ghosts The ghosts amoebas that are in contact with the group.
/// \param x The X position to look for a posible group candidate.
/// \param y The Y position to look for a possible group candidate.
/// \param visitedPositions The set of the already visited positions by the
///                         algorithm. Initially should be empty.
/// \param colour The colour the amoeba should be to be part of the group.
/// \param initialAmoeba Tells if the amoeba \a x and \a y should be the
///        first amoeba of the group.
///
void
Grid::makeGroup (std::vector<FallingAmoeba> &group,
                 std::vector<FallingAmoeba> &ghosts, int16_t x, int16_t y,
                 std::set<std::pair<int16_t, int16_t> > &visitedPositions,
                 Amoeba::Colour colour, bool initialAmoeba)
{
    // Only check this possition if we didn't visited it yet.
    if ( visitedPositions.end () ==
            visitedPositions.find (std::make_pair (x, y)) )
    {
        // If there's any amoeba at the given position, check if is a member
        // of the group.
        Amoeba *amoeba = getAmoebaAt (x, y);
        if ( 0 != amoeba && !amoeba->isDying () )
        {
            // The initial amoeba is always a member of the group, so
            // we get its colour and make it the group's colour.
            if ( initialAmoeba )
            {
                colour = amoeba->getColour ();
            }
            // Can't make groups of ghost amoebas. Set it to the
            // ghosts groups and don't look any further.
            if ( amoeba->getColour () == Amoeba::ColourGhost )
            {
                FallingAmoeba ghost;
                ghost.amoeba = amoeba;
                ghost.x = x;
                ghost.y = y;
                ghosts.push_back (ghost);

                // Add the current position to the already visited positions.
                visitedPositions.insert (std::make_pair (x, y));
            }
            else if ( colour == amoeba->getColour () )
            {
                // Once a member is identified, add it to the group.
                FallingAmoeba groupMember;
                groupMember.amoeba = amoeba;
                groupMember.x = x;
                groupMember.y = y;
                group.push_back (groupMember);

                // Add the current position to the already visited positions.
                visitedPositions.insert (std::make_pair (x, y));

                // Check neighbours.
                makeGroup (group, ghosts, x, y - 1, visitedPositions, colour, false);
                makeGroup (group, ghosts, x + 1, y, visitedPositions, colour, false);
                makeGroup (group, ghosts, x, y + 1, visitedPositions, colour, false);
                makeGroup (group, ghosts, x - 1, y, visitedPositions, colour, false);
            }
        }
    }
}

///
/// \brief Checks all grid's position and tries to make chains.
///
/// A chain is nothing more than 4 or more amoebas joined in a single
/// group.
///
void
Grid::makeChain (void)
{
    // First delete all groups.
    for ( int16_t row = 0 ; row < k_GridHeight ; ++row )
    {
        for ( int16_t column = 0 ; column < k_GridWidth ; ++column )
        {
            std::set<std::pair<int16_t, int16_t> > visitedPositions;
            std::vector<FallingAmoeba> group;
            std::vector<FallingAmoeba> ghosts;
            makeGroup (group, ghosts, column, row, visitedPositions);
            if ( 4 <= group.size () )
            {
                int16_t meanXPosition = 0;
                int16_t meanYPosition = 0;

                m_DyingTime = k_DefaultDyingTime;
                m_BlinkTime = k_DefaultBlinkTime;
                for (std::vector<FallingAmoeba>::iterator currentAmoeba =
                        group.begin () ;
                     currentAmoeba != group.end () ; ++currentAmoeba )
                {
                    meanXPosition += currentAmoeba->x;
                    meanYPosition += currentAmoeba->y;
                    currentAmoeba->amoeba->setDying (true);
                    m_DyingAmoebas.push_back (*currentAmoeba);
                }

                for (std::vector<FallingAmoeba>::iterator currentGhost =
                        ghosts.begin () ;
                     currentGhost != ghosts.end () ; ++currentGhost )
                {
                    currentGhost->amoeba->setDying (true);
                    m_DyingAmoebas.push_back (*currentGhost);
                }

                meanXPosition /= group.size ();
                meanYPosition /= group.size ();
                addChainLabel (getCurrentStepChain () + 1,
                               getGridPositionX () +
                               meanXPosition * getAmoebaSize () +
                               getAmoebaSize () / 2,
                               getGridPositionY () +
                               (meanYPosition - k_FirstVisibleHeight) *
                               getAmoebaSize () + getAmoebaSize () / 2);
            }
        }
    }

    findFloatingAmoebas ();
    // If there's no floating amoebas, set up the falling pair.
    if ( m_FloatingAmoebas.empty () && m_DyingAmoebas.empty () )
    {
        if ( getNumberOfWaitingGhosts () == 0 )
        {
                // If the positions above the grid of the 3rd column (from
                // the right) is occuped by an amoeba, then the grid is
                // considered filled.  If any amobea is sitting over the grid in
                // any other column, this is not considered filled.
                if ( 0 != getAmoebaAt (3, k_FirstVisibleHeight - 1) )
                {
                    markAsFilled ();
                }
                else
                {
                    scheduleFallingPair ();
                }
        }
        else
        {
            makeGhostsFall ();
        }
    }
}

///
/// \brief Marks the grid as filled.
///
/// The grid must be marked as filled when the falling amoebas
/// can't enter the grid.
///
/// \see isFilled().
///
void
Grid::markAsFilled (void)
{
    m_Filled = true;
}

///
/// \brief Moves the falling amoeba pair a position to the left.
///
void
Grid::moveLeft (void)
{
    if ( 0 < std::min (m_FallingPair.main.x,
                       m_FallingPair.satellite.x) &&
         0 == getAmoebaAt (m_FallingPair.main.x - 1, m_FallingPair.main.y) &&
         0 == getAmoebaAt (m_FallingPair.satellite.x - 1,
                           m_FallingPair.satellite.y) )
    {
        --m_FallingPair.main.x;
        --m_FallingPair.satellite.x;
    }
}

///
/// \brief Moves the falling amoeba pair a position to the right.
///
void
Grid::moveRight (void)
{
    if ( k_GridWidth - 1  > std::max (m_FallingPair.main.x,
                                      m_FallingPair.satellite.x) &&
        0 == getAmoebaAt (m_FallingPair.main.x + 1, m_FallingPair.main.y) &&
        0 == getAmoebaAt (m_FallingPair.satellite.x + 1,
                          m_FallingPair.satellite.y) )
    {
        ++m_FallingPair.main.x;
        ++m_FallingPair.satellite.x;
    }
}

///
/// \brief Removes some waiting ghost amoebas.
///
/// \param newGhostAmoebas The number of waiting ghost amoebas to remove.
/// \return The number of ghost amoebas remaining after removing
///         \p newGhostAmoebas ghost amoebas.
///
uint8_t
Grid::removeGhostAmoebas (uint8_t newGhostAmoebas)
{
    uint8_t remainingGhostAmoebas = 0;
    if ( getNumberOfWaitingGhosts () < newGhostAmoebas )
    {
        remainingGhostAmoebas = newGhostAmoebas - getNumberOfWaitingGhosts ();
        setNumberOfWaitingGhosts (0);
    }
    else
    {
        setNumberOfWaitingGhosts (getNumberOfWaitingGhosts () - newGhostAmoebas);
    }
    updateWaitingGhosts ();
    return remainingGhostAmoebas;
}

///
/// \brief Rotates the falling pair clockwise.
///
void
Grid::rotateClockwise (void)
{
    if ( !isSatelliteRotating () )
    {
        m_DegreesUntilRotationDone = 90.0f;
        m_RotationDirection = -1.0f;

        if ( m_FallingPair.satellite.y < m_FallingPair.main.y )
        {
            if ( k_GridWidth - 1 > m_FallingPair.main.x &&
                    0 == getAmoebaAt (m_FallingPair.main.x + 1, m_FallingPair.main.y) )
            {
                m_FallingPair.satellite.x++;
                m_FallingPair.satellite.y++;
            }
            else if ( 1 < m_FallingPair.main.x &&
                    0 == getAmoebaAt (m_FallingPair.main.x - 1, m_FallingPair.main.y) )
            {
                m_FallingPair.satellite.y++;
                m_FallingPair.main.x--;
            }
            else
            {
                // Can't rotate
                m_DegreesUntilRotationDone = 0.0f;
            }
        }
        else if ( m_FallingPair.satellite.y > m_FallingPair.main.y )
        {
            if ( 0 <  m_FallingPair.main.x &&
                    0 == getAmoebaAt (m_FallingPair.main.x - 1, m_FallingPair.main.y) )
            {
                m_FallingPair.satellite.x--;
                m_FallingPair.satellite.y--;
            }
            else if ( k_GridWidth - 1 > m_FallingPair.main.x &&
                    0 == getAmoebaAt (m_FallingPair.main.x + 1, m_FallingPair.main.y) )
            {
                m_FallingPair.satellite.y--;
                m_FallingPair.main.x++;
            }
            else
            {
                // Can't rotate
                m_DegreesUntilRotationDone = 0.0f;
            }
        }
        else if ( m_FallingPair.satellite.x > m_FallingPair.main.x )
        {
            if ( 0 == getAmoebaAt (m_FallingPair.main.x, m_FallingPair.main.y + 1) )
            {
                m_FallingPair.satellite.x--;
                m_FallingPair.satellite.y++;
            }
            else
            {
                // Can't rotate
                m_DegreesUntilRotationDone = 0.0f;
            }
        }
        else
        {
            m_FallingPair.satellite.x++;
            m_FallingPair.satellite.y--;
        }
    }
}

///
/// \brief Rotates the falling pair counterclockwise.
///
void
Grid::rotateCounterClockwise (void)
{
    if ( !isSatelliteRotating () )
    {
        m_DegreesUntilRotationDone = 90.0f;
        m_RotationDirection = 1.0f;
        if ( m_FallingPair.satellite.y < m_FallingPair.main.y )
        {
            if ( 0 < m_FallingPair.main.x &&
                    0 == getAmoebaAt (m_FallingPair.main.x - 1, m_FallingPair.main.y) )
            {
                m_FallingPair.satellite.x--;
                m_FallingPair.satellite.y++;
            }
            else if ( k_GridWidth - 1 > m_FallingPair.main.x &&
                    0 == getAmoebaAt (m_FallingPair.main.x + 1, m_FallingPair.main.y) )
            {
                m_FallingPair.satellite.y++;
                m_FallingPair.main.x++;
            }
            else
            {
                // Can't rotate
                m_DegreesUntilRotationDone = 0.0f;
            }
        }
        else if ( m_FallingPair.satellite.y > m_FallingPair.main.y )
        {
            if ( k_GridWidth - 1 >  m_FallingPair.main.x &&
                    0 == getAmoebaAt (m_FallingPair.main.x + 1, m_FallingPair.main.y) )
            {
                m_FallingPair.satellite.x++;
                m_FallingPair.satellite.y--;
            }
            else if ( 1 < m_FallingPair.main.x &&
                    0 == getAmoebaAt (m_FallingPair.main.x - 1, m_FallingPair.main.y) )
            {
                m_FallingPair.satellite.y--;
                m_FallingPair.main.x--;
            }
            else
            {
                // Can't rotate
                m_DegreesUntilRotationDone = 0.0f;
            }
        }
        else if ( m_FallingPair.satellite.x < m_FallingPair.main.x )
        {
            if ( 0 == getAmoebaAt (m_FallingPair.main.x, m_FallingPair.main.y + 1) )
            {
                m_FallingPair.satellite.x++;
                m_FallingPair.satellite.y++;
            }
            else
            {
                // Can't rotate
                m_DegreesUntilRotationDone = 0.0f;
            }
        }
        else
        {
            m_FallingPair.satellite.x--;
            m_FallingPair.satellite.y--;
        }
    }
}

///
/// \brief Rotates the satellite amoeba.
///
/// \param degreesToRotate The degrees to make rotate the satellite amoeba.
///
void
Grid::rotateSatelliteAmoeba (float degreesToRotate)
{
    if ( degreesToRotate > m_DegreesUntilRotationDone )
    {
        degreesToRotate = m_DegreesUntilRotationDone;
    }

    m_DegreesUntilRotationDone -= degreesToRotate;
    degreesToRotate *= m_RotationDirection;
    m_CurrentRotationDegrees += degreesToRotate;
    if ( m_CurrentRotationDegrees > 360.0f )
    {
        m_CurrentRotationDegrees -= 360.0f;
    }
    else if ( m_CurrentRotationDegrees < 0 )
    {
        m_CurrentRotationDegrees += 360.0f;
    }
    setFallingAmoebaScreenPosition (m_FallingPair.satellite,
                                    m_FallingPair.verticalOffset,
                                    isSatelliteRotating ());
}

///
/// \brief Sets the amoeba at a given grid's position.
///
/// \param x The X position to set the amoeba to.
/// \param y The Y position to set the amoeba to.
/// \param amoeba The amoeba to set at grid's position \p x and \p y.
///
void
Grid::setAmoebaAt (int16_t x, int16_t y, Amoeba *amoeba)
{
    if ( 0 <= x && x < k_GridWidth &&
         0 <= y && y < k_GridHeight )
    {
        m_Grid[y * k_GridWidth + x] = amoeba;
    }
}

///
/// \brief Sets the status of an amoeba at a given position.
///
/// This checks the neighbors of the amoeba at grid's position \a x and \a y
/// to check how many amoebas of the same colour there are.
///
/// \param x The grid's X position of the amoeba to set the state to.
/// \param y The grid's Y position of the amoeba to set the state to.
/// \param neighbors If set to \a true, this function sets the state of
///                  the neighbors amoebas.
///
void
Grid::setAmoebaStateAt (int16_t x, int16_t y, bool neighbors)
{
    Amoeba *currentAmoeba = getAmoebaAt (x, y);
    if ( 0 != currentAmoeba &&
         Amoeba::ColourGhost != currentAmoeba->getColour () )
    {
        int state = Amoeba::StateNone;
        if ( isOfSameColour (x, y - 1, currentAmoeba) )
        {
            state |= Amoeba::StateTop;
        }
        if ( isOfSameColour (x + 1, y, currentAmoeba) )
        {
            state |= Amoeba::StateRight;
        }
        if ( isOfSameColour (x, y + 1, currentAmoeba) )
        {
            state |= Amoeba::StateBottom;
        }
        if ( isOfSameColour (x - 1, y, currentAmoeba) )
        {
            state |= Amoeba::StateLeft;
        }
        currentAmoeba->setState (static_cast<Amoeba::State>(state));
    }

    // If we must check neighbors, set the state of the direct
    // neighbors (i.e., no diagonals).
    if ( neighbors )
    {
        setAmoebaStateAt (x, y - 1, false);
        setAmoebaStateAt (x + 1, y, false);
        setAmoebaStateAt (x, y + 1, false);
        setAmoebaStateAt (x - 1, y, false);
    }
}

///
/// \brief Gets the next pair from the inactive list and sets it to the queue.
///
/// The next pair will be the last element in the queue, so it should
/// be called when the first element of the queue becomes the falling pair.
///
/// If the inactive amoebas list becomes empty, this function request
/// to the pair generator a new pair.
///
/// This function is used when moving the queue step by step instead of
/// just setting the queued amoeba behind the scene.
///
void
Grid::scheduleFallingPair (void)
{
    assert ( 0 != m_Generator &&
            "Tried to get the next pair without a generator.");
    assert ( 2 <= m_InactiveAmoebas.size () &&
            "Tried to get the next pair, but the inactive list is empty.");

    // Gets the next pair from the inactive list.
    Amoeba *main = m_InactiveAmoebas.front ();
    m_InactiveAmoebas.pop_front ();
    Amoeba *satellite = m_InactiveAmoebas.front ();
    m_InactiveAmoebas.pop_front ();
    // And set them to queued.
    m_QueuedAmoebas.push_back (main);
    m_QueuedAmoebas.push_back (satellite);
    // Sets they position to be the last pair on the queue.
    // Take into account the side where the queue is located at
    // to choose the offset to apply to this pair.
    uint16_t lastPairOffsetX = getAmoebaSize () / 2;
    if ( LayoutVertical == getLayout () )
    {
        if ( Grid::QueueSideLeft == getQueueSide () )
        {
            lastPairOffsetX = -lastPairOffsetX;
        }
        main->setX (getQueuePositionX () + lastPairOffsetX);
        main->setY (getQueuePositionY () + getAmoebaSize () * 5);
        satellite->setX (getQueuePositionX () + lastPairOffsetX);
        satellite->setY (getQueuePositionY () + getAmoebaSize () * 4);
    }
    else
    {
        if (Grid::QueueSideLeft == getQueueSide () )
        {
            main->setX (getQueuePositionX () - getAmoebaSize () * 6);
            main->setY (getQueuePositionY () + lastPairOffsetX);
            satellite->setX (getQueuePositionX () - getAmoebaSize () * 5);
            satellite->setY (getQueuePositionY () + lastPairOffsetX);
        }
        else
        {
            main->setX (getQueuePositionX () + getAmoebaSize () * 5);
            main->setY (getQueuePositionY () - getAmoebaSize () - lastPairOffsetX);
            satellite->setX (getQueuePositionX () + getAmoebaSize () * 4);
            satellite->setY (getQueuePositionY () - getAmoebaSize () - lastPairOffsetX);
        }
    }
    // And set up the falling pair structure for when they fall.
    FallingPair fallingPair;
    fallingPair.main.amoeba = main;
    fallingPair.main.x = 3;
    fallingPair.main.y = k_FirstVisibleHeight - 2;
    fallingPair.satellite.amoeba = satellite;
    fallingPair.satellite.x = 3;
    fallingPair.satellite.y = k_FirstVisibleHeight - 3;
    fallingPair.verticalOffset = 0;
    fallingPair.fallingTime = k_DefaultFallingTime;
    m_Queue.push_back (fallingPair);
    // Request a new pair if we are out of inactive pairs.
    if ( m_InactiveAmoebas.empty () )
    {
        m_Generator->generate ();
    }

    // Tells that the queue is moving.
    m_RemainingQueueHorizontalOffset = getAmoebaSize () / 2;
    m_RemainingQueueVerticalOffset = 2 * getAmoebaSize ();
    m_QueueIsMoving = true;
}

///
/// \brief Sets the global screen position to a falling amoeba.
///
/// \param fallingAmoeba The falling amoeba to set its screen position.
/// \param verticalOffset The offset to add to the amoeba's Y coordinate.
/// \param rotating Tells if the amoeba is rotating and has to take into
///                 account the current rotation degrees.  Used for the
///                 satellite.
///
void
Grid::setFallingAmoebaScreenPosition (FallingAmoeba &fallingAmoeba,
                                      int16_t verticalOffset, bool rotating)
{
    int16_t x = getGridPositionX ();
    int16_t y = getGridPositionY ();
    if (Grid::LayoutVertical == getLayout ())
    {
        y += verticalOffset  - getAmoebaSize () * k_FirstVisibleHeight;
        if ( rotating )
        {
            float rotationX = k_Cos[getCurrentRotationDegree ()];
            float rotationY = k_Sin[getCurrentRotationDegree ()];
            int16_t directionX = 0;
            int16_t directionY = 0;
            if ( 0 <= getCurrentRotationDegree () &&
                      getCurrentRotationDegree () < 90 )
            {
                if ( m_RotationDirection < 0.0f )
                {
                    directionX = -1;
                    directionY = 0;
                }
                else
                {
                    directionX = 0;
                    directionY = 1;
                }
            }
            else if ( 90 <= getCurrentRotationDegree () &&
                            getCurrentRotationDegree () < 180 )
            {
                if ( m_RotationDirection < 0.0f )
                {
                    directionX = 0;
                    directionY = 1;
                }
                else
                {
                    directionX = 1;
                    directionY = 0;
                }
            }
            else if ( 180 <= getCurrentRotationDegree () &&
                             getCurrentRotationDegree () < 270 )
            {
                if ( m_RotationDirection < 0.0f )
                {
                    directionX = 1;
                    directionY = 0;
                }
                else
                {
                    directionX = 0;
                    directionY = -1;
                }
            }
            else if ( getCurrentRotationDegree () >= 270 )
            {
                if ( m_RotationDirection < 0.0f )
                {
                    directionX = 0;
                    directionY = -1;
                }
                else
                {
                    directionX = -1;
                    directionY = 0;
                }
            }

            x += static_cast<uint16_t> ((fallingAmoeba.x + directionX) *
                    getAmoebaSize () +
                    getAmoebaSize () * rotationX);
            y += static_cast<uint16_t> ((fallingAmoeba.y + directionY) *
                    getAmoebaSize () -
                    getAmoebaSize () * rotationY);
        }
        else
        {
            x += fallingAmoeba.x * getAmoebaSize ();
            y += fallingAmoeba.y * getAmoebaSize ();
        }
    }
    else
    {
        float rotationY = k_Cos[getCurrentRotationDegree ()];
        float rotationX = k_Sin[getCurrentRotationDegree ()];
        int16_t directionY = 0;
        int16_t directionX = 0;
        if ( rotating )
        {
            if ( 0 <= getCurrentRotationDegree () &&
                    getCurrentRotationDegree () < 90 )
            {
                if ( m_RotationDirection < 0.0f )
                {
                    directionY = -1;
                    directionX = 0;
                }
                else
                {
                    directionY = 0;
                    directionX = 1;
                }
            }
            else if ( 90 <= getCurrentRotationDegree () &&
                    getCurrentRotationDegree () < 180 )
            {
                if ( m_RotationDirection < 0.0f )
                {
                    directionY = 0;
                    directionX = 1;
                }
                else
                {
                    directionY = 1;
                    directionX = 0;
                }
            }
            else if ( 180 <= getCurrentRotationDegree () &&
                    getCurrentRotationDegree () < 270 )
            {
                if ( m_RotationDirection < 0.0f )
                {
                    directionY = 1;
                    directionX = 0;
                }
                else
                {
                    directionY = 0;
                    directionX = -1;
                }
            }
            else if ( getCurrentRotationDegree () >= 270 )
            {
                if ( m_RotationDirection < 0.0f )
                {
                    directionY = 0;
                    directionX = -1;
                }
                else
                {
                    directionY = -1;
                    directionX = 0;
                }
            }
        }


        if (Grid::QueueSideLeft == getQueueSide ())
        {
            if (rotating)
            {
                x -= (fallingAmoeba.y + 1 + directionX) * getAmoebaSize () -
                    static_cast<uint16_t> (getAmoebaSize () * rotationX);
                y += (fallingAmoeba.x + directionY) * getAmoebaSize () +
                    static_cast<uint16_t> (getAmoebaSize () * rotationY);
            }
            else
            {

                x -= (fallingAmoeba.y + 1) * getAmoebaSize ();
                y += fallingAmoeba.x * getAmoebaSize ();
            }
            x -= verticalOffset - getAmoebaSize () * k_FirstVisibleHeight;
        }
        else
        {
            if ( rotating )
            {
                x += (fallingAmoeba.y + directionX) * getAmoebaSize () -
                    static_cast<uint16_t> (getAmoebaSize () * rotationX);
                y -= (fallingAmoeba.x + 1 + directionY) * getAmoebaSize () +
                    static_cast<uint16_t> (getAmoebaSize () * rotationY);
            }
            else
            {
                x += fallingAmoeba.y * getAmoebaSize ();
                y -= (fallingAmoeba.x + 1) * getAmoebaSize ();
            }
            x += verticalOffset - getAmoebaSize () * k_FirstVisibleHeight;
        }
    }

    fallingAmoeba.amoeba->setX (x);
    fallingAmoeba.amoeba->setY (y);
}

///
/// \brief Sets the generator.
///
/// When the list of waiting amoebas reaches zero, this generator
/// will be the used to generate a new pair.
///
/// \param generator The generator that the class is linked to and
///                  will use to request more pairs.
///
void
Grid::setGenerator (PairGenerator *generator)
{
    m_Generator = generator;
}

///
/// \brief Sets the falling speed to maximum.
///
void
Grid::setMaxFallingSpeed (void)
{
    m_MaxFallingSpeed = true;
}

///
/// \brief Sets the status of the new falling pair.
///
/// \param hasNewFallingPair Set it to \a true when a new falling pair
///                          starts to fall. Otherwise set it to \a false.
/// \see hasNewFallingPair().
///
inline void
Grid::setHasNewFallingPair (bool hasNewFallingPair)
{
    m_HasNewFallingPair = hasNewFallingPair;
}

///
/// \brief Sets the next falling pair.
///
/// Gets the next falling pair from the list of waiting pairs and
/// moves the last element to be the next.
///
void
Grid::setupFallingPair (void)
{
    // Get the two first queued amoebas and set it to be active.
    m_ActiveAmoebas.push_back (m_QueuedAmoebas.front ());
    m_QueuedAmoebas.pop_front ();
    m_ActiveAmoebas.push_back (m_QueuedAmoebas.front ());
    m_QueuedAmoebas.pop_front ();

    m_FallingPair = m_Queue.front ();
    m_Queue.pop_front ();
    // Set the screen position without any vertical offset. The grid
    // position is set at getNextPair().
    setFallingAmoebaScreenPosition (m_FallingPair.main, 0, false);
    setFallingAmoebaScreenPosition (m_FallingPair.satellite, 0, false);

    // Reset the current rotation.
    m_CurrentRotationDegrees = 90.0f;
    m_DegreesUntilRotationDone = 0.0f;
    m_RotationDirection = 1.0f;

    // Now set the remainig pair to be the next to fall and
    // add a new pair.
    FallingPair remainingPair (m_Queue.front ());
    if ( Grid::LayoutVertical == getLayout () )
    {
        remainingPair.main.amoeba->setX (getQueuePositionX ());
        remainingPair.main.amoeba->setY (getQueuePositionY () + getAmoebaSize ());
        remainingPair.satellite.amoeba->setX (getQueuePositionX ());
        remainingPair.satellite.amoeba->setY (getQueuePositionY ());
    }
    else
    {
        if (Grid::QueueSideLeft == getQueueSide () )
        {
            remainingPair.main.amoeba->setX (getQueuePositionX () - getAmoebaSize () * 2);
            remainingPair.main.amoeba->setY (getQueuePositionY ());
            remainingPair.satellite.amoeba->setX (getQueuePositionX () - getAmoebaSize ());
            remainingPair.satellite.amoeba->setY (getQueuePositionY ());
        }
        else
        {
            remainingPair.main.amoeba->setX (getQueuePositionX () + getAmoebaSize ());
            remainingPair.main.amoeba->setY (getQueuePositionY () - getAmoebaSize ());
            remainingPair.satellite.amoeba->setX (getQueuePositionX ());
            remainingPair.satellite.amoeba->setY (getQueuePositionY () - getAmoebaSize ());
        }
    }
    getNextPair ();
    setCurrentStepChain (0);
    setHasNewFallingPair (true);
}

///
/// \brief Sets the falling speed to normal.
///
void
Grid::setNormalFallingSpeed (void)
{
    m_MaxFallingSpeed = false;
}


///
/// \brief Sets the current silhoutte frame.
///
/// \param frame The silhouette frame to set as current.
///
inline void
Grid::setSilhouetteFrame (int8_t frame)
{
    m_SilhouetteFrame = frame;
}

///
/// \brief Updates the grid.
///
/// Moves the amoebax according to its state and the time elapsed from the last
/// time this function was called, as specified in \a elapsedTime.
///
/// \param elapsedTime The elapsed time between the previous and current call,
///                    in milliseconds.
///
void
Grid::update (uint32_t elapsedTime)
{
    // Only update if we are no waiting the initial amoebas
    // and the grid is not filled.
    if ( !m_WaitingInitialAmoebas && !isFilled () )
    {
        // By default, we don't have a new falling pair unless
        // we must set up a new pair to fall or is the first one.
        setHasNewFallingPair (false || m_FirstFallingPair);
        // The first pair is done!
        m_FirstFallingPair = false;
        // No ghost amoebas for our opponent.
        setOpponentGhostAmoebas (0);

        // Remove old label chain labels.
        for ( std::list<ChainLabel *>::iterator currentLabel =
                m_ChainLabels.begin () ;
              currentLabel != m_ChainLabels.end () ; )
        {
            if ( !(*currentLabel)->isAlive () )
            {
                currentLabel = m_ChainLabels.erase (currentLabel);
            }
            else
            {
                ++currentLabel;
            }
        }
        // And update the remaining.
        for ( std::list<ChainLabel *>::iterator currentLabel =
                m_ChainLabels.begin () ;
              currentLabel != m_ChainLabels.end () ;
              ++currentLabel )
        {
            (*currentLabel)->update (elapsedTime);
        }

        if ( m_FloatingAmoebas.empty () && m_DyingAmoebas.empty () &&
             !isQueueMoving () )
        {
            m_SilhouetteTime -= elapsedTime;
            if ( 0 > m_SilhouetteTime )
            {
                m_SilhouetteTime = k_SilhouetteTime;
                setSilhouetteFrame (getSilhouetteFrame () +
                        m_SilhouetteFrameDirection);
                if ( 0 > getSilhouetteFrame () ||
                        k_MaxSilhouetteFrames == getSilhouetteFrame () )
                {
                    m_SilhouetteFrameDirection = -m_SilhouetteFrameDirection;
                    setSilhouetteFrame (getSilhouetteFrame () +
                            m_SilhouetteFrameDirection);
                }
            }

            if ( isSatelliteRotating () )
            {
                float degreesToRotate = k_RotationSpeed * elapsedTime;
                rotateSatelliteAmoeba (degreesToRotate);
            }

            m_FallingPair.fallingTime -= elapsedTime;
            if ( 0 >= m_FallingPair.fallingTime ||
                 m_MaxFallingSpeed )
            {
                m_FallingPair.fallingTime = k_DefaultFallingTime;
                m_FallingPair.verticalOffset += getAmoebaSize () / 2;
                if ( m_MaxFallingSpeed )
                {
                    m_FallingPair.verticalOffset += getAmoebaSize () / 2;
                    incrementScore ();
                }
            }


            if ( m_FallingPair.verticalOffset >= getAmoebaSize () )
            {
                m_FallingPair.verticalOffset -= getAmoebaSize ();
                ++m_FallingPair.main.y;
                ++m_FallingPair.satellite.y;
            }

            if ( k_GridHeight - 1 == m_FallingPair.main.y ||
                 k_GridHeight - 1 == m_FallingPair.satellite.y ||
                 0 != getAmoebaAt (m_FallingPair.main.x,
                                   m_FallingPair.main.y + 1) ||
                 0 != getAmoebaAt (m_FallingPair.satellite.x,
                                   m_FallingPair.satellite.y + 1) )
            {
                // Set to no silhouette frame.
                setSilhouetteFrame (0);
                m_SilhouetteFrameDirection = 1;
                // Set the screen position without any vertical offset.
                setFallingAmoebaScreenPosition (m_FallingPair.main, 0, false);
                setFallingAmoebaScreenPosition (m_FallingPair.satellite, 0, false);
                // Set the amoeba's position to be occuped.
                setAmoebaAt (m_FallingPair.main.x, m_FallingPair.main.y,
                        m_FallingPair.main.amoeba);
                setAmoebaAt (m_FallingPair.satellite.x,
                        m_FallingPair.satellite.y,
                        m_FallingPair.satellite.amoeba);
                setAmoebaStateAt (m_FallingPair.main.x,
                        m_FallingPair.main.y);
                setAmoebaStateAt (m_FallingPair.satellite.x,
                        m_FallingPair.satellite.y);
                // If either the satelite or the main amoeba is floating
                // make it to behaviour as it should.
                findFloatingAmoebas ();
                // Check if we have any chain that we can make if no
                // floating amoeba is around.
                if ( m_FloatingAmoebas.empty () )
                {
                    makeChain ();
                }
                setNormalFallingSpeed ();
            }
            else
            {
                setFallingAmoebaScreenPosition (m_FallingPair.main,
                                                m_FallingPair.verticalOffset,
                                                false);
                setFallingAmoebaScreenPosition (m_FallingPair.satellite,
                                                m_FallingPair.verticalOffset,
                                                isSatelliteRotating ());
            }
        }

        else if ( !m_FloatingAmoebas.empty () )
        {
            m_FloatingAmoebasVerticalOffset += getAmoebaSize () / 2;
            std::list<FallingAmoeba>::iterator floatingAmoeba =
                m_FloatingAmoebas.begin ();
            while ( floatingAmoeba != m_FloatingAmoebas.end () )
            {
                if ( m_FloatingAmoebasVerticalOffset >= getAmoebaSize () )
                {
                    ++floatingAmoeba->y;
                    if ( k_GridHeight - 1 == floatingAmoeba->y ||
                         0 != getAmoebaAt (floatingAmoeba->x,
                                           floatingAmoeba->y + 1) )
                    {
                        if ( floatingAmoeba->y >= 0 )
                        {
                            setFallingAmoebaScreenPosition (*floatingAmoeba, 0,
                                                            false);
                            setAmoebaAt (floatingAmoeba->x, floatingAmoeba->y,
                                         floatingAmoeba->amoeba);
                            setAmoebaStateAt (floatingAmoeba->x,
                                              floatingAmoeba->y);
                        }
                        else
                        {
                            m_ActiveAmoebas.remove (floatingAmoeba->amoeba);
                            delete floatingAmoeba->amoeba;
                        }
                        floatingAmoeba =
                            m_FloatingAmoebas.erase (floatingAmoeba);
                    }
                    else
                    {
                        ++floatingAmoeba;
                    }
                }
                else
                {
                    setFallingAmoebaScreenPosition (*floatingAmoeba,
                            m_FloatingAmoebasVerticalOffset, false);
                    ++floatingAmoeba;
                }
            }

            if ( m_FloatingAmoebasVerticalOffset >= getAmoebaSize () )
            {
                m_FloatingAmoebasVerticalOffset -= getAmoebaSize ();
            }

            if ( m_FloatingAmoebas.empty () )
            {
                makeChain ();
            }
        }

        else if ( !m_DyingAmoebas.empty () )
        {
            m_DyingTime -= elapsedTime;
            if ( 0 >= m_DyingTime )
            {
                clearDyingAmoebas ();
            }
            else
            {
                m_BlinkTime -= elapsedTime;
                if ( 0 >= m_BlinkTime )
                {
                    m_BlinkTime = k_DefaultBlinkTime;
                    for (std::vector<FallingAmoeba>::iterator currentAmoeba =
                            m_DyingAmoebas.begin () ;
                        currentAmoeba != m_DyingAmoebas.end () ; ++currentAmoeba )
                    {
                        currentAmoeba->amoeba->setVisible (
                                !currentAmoeba->amoeba->isVisible ());
                    }
                }
            }
        }

        else if ( isQueueMoving () )
        {
            updateQueue (elapsedTime);
        }
    }
}

///
/// \brief Updates the moving queue.
///
/// \param elapsedTime The time elapsed from the last call.
///
void
Grid::updateQueue (uint32_t elapsedTime)
{
    uint16_t queueHorizontalOffset =
        static_cast<uint16_t> (k_QueueVerticalSpeed * elapsedTime);
    if ( queueHorizontalOffset > m_RemainingQueueHorizontalOffset )
    {
        queueHorizontalOffset = m_RemainingQueueHorizontalOffset;
    }
    m_RemainingQueueHorizontalOffset -= queueHorizontalOffset;

    uint16_t queueVerticalOffset =
        static_cast<uint16_t> (k_QueueVerticalSpeed * elapsedTime);
    if ( queueVerticalOffset > m_RemainingQueueVerticalOffset )
    {
        queueVerticalOffset = m_RemainingQueueVerticalOffset;
    }
    m_RemainingQueueVerticalOffset -= queueVerticalOffset;

    if ( Grid::LayoutHorizontal == getLayout () )
    {
        if ( Grid::QueueSideLeft == getQueueSide () )
        {
            queueVerticalOffset = -queueVerticalOffset;
        }
        else
        {
            queueHorizontalOffset = -queueHorizontalOffset;
        }
    }
    else
    {
        if ( Grid::QueueSideLeft == getQueueSide () )
        {
            queueHorizontalOffset = -queueHorizontalOffset;
        }
    }

    std::list<Amoeba *>::iterator currentAmoeba = m_QueuedAmoebas.begin () ;
    for ( int amoebaNum = 0 ; currentAmoeba != m_QueuedAmoebas.end () ;
          ++currentAmoeba, ++amoebaNum )
    {
        if ( Grid::LayoutHorizontal == getLayout () )
        {
            (*currentAmoeba)->setX ((*currentAmoeba)->getX () -
                                    queueVerticalOffset);
            if ( 2 == amoebaNum || 3 == amoebaNum )
            {
                (*currentAmoeba)->setY ((*currentAmoeba)->getY () -
                                        queueHorizontalOffset);
            }
        }
        else
        {
            (*currentAmoeba)->setY ((*currentAmoeba)->getY () -
                                    queueVerticalOffset);

            if ( 2 == amoebaNum || 3 == amoebaNum )
            {
                (*currentAmoeba)->setX ((*currentAmoeba)->getX () -
                                        queueHorizontalOffset);
            }
        }
    }


    if ( m_RemainingQueueHorizontalOffset == 0 &&
         m_RemainingQueueVerticalOffset == 0 )
    {
        m_QueueIsMoving = false;

        // Get the two first queued amoebas and set it to be active.
        m_ActiveAmoebas.push_back (m_QueuedAmoebas.front ());
        m_QueuedAmoebas.pop_front ();
        m_ActiveAmoebas.push_back (m_QueuedAmoebas.front ());
        m_QueuedAmoebas.pop_front ();

        m_FallingPair = m_Queue.front ();
        m_Queue.pop_front ();
        // Set the screen position without any vertical offset. The grid
        // position is set at getNextPair().
        setFallingAmoebaScreenPosition (m_FallingPair.main, 0, false);
        setFallingAmoebaScreenPosition (m_FallingPair.satellite, 0, false);

        // Reset the current rotation.
        m_CurrentRotationDegrees = 90.0f;
        m_DegreesUntilRotationDone = 0.0f;
        m_RotationDirection = 1.0f;

        setCurrentStepChain (0);
        setHasNewFallingPair (true);
    }
}

///
/// \brief Updates the state of the waiting ghosts amoebas.
///
/// Updates the state of the amoebas that will be drawn as the
/// waiting status. The "neighbour" state of those amoebas will
/// decide the "weight" that each ghost amoeba has.
///
void
Grid::updateWaitingGhosts (void)
{
    int8_t remainingGhost = getNumberOfWaitingGhosts ();
    for ( uint8_t currentAmoeba = 0 ; currentAmoeba < k_GridWidth ; ++currentAmoeba )
    {
        uint8_t weight = 0;
        if ( remainingGhost > 0 )
        {
            if ( remainingGhost <= (k_GridWidth - currentAmoeba) )
            {
                weight = 1;
            }
            else
            {
                weight = static_cast<uint8_t>(std::ceil (remainingGhost /
                                                        (static_cast<double>(k_GridWidth) - currentAmoeba)));
            }
            remainingGhost -= weight;
        }
        m_WaitingGhostAmoebas[currentAmoeba]->setState (
                static_cast<Amoeba::State> (Amoeba::StateTopRightBottomLeft - weight));
    }
}
