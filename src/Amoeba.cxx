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
#include "Amoeba.h"

using namespace Amoebax;

///
/// \brief Constructor.
///
/// \param colour The amoeba's colour.
///
Amoeba::Amoeba (Colour colour):
    m_Colour (colour),
    m_Dying (false),
    m_State (StateNone),
    m_Visible (true),
    m_X (0),
    m_Y (0)
{
}

///
/// \brief Destructor.
///
Amoeba::~Amoeba (void)
{
}
