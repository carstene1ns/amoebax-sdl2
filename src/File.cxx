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
#if defined (IS_WIN32_HOST)
#include <dirent.h>
#else // !IS_WIN32_HOST
#include <unistd.h>
#endif // IS_WIN32_HOST
#include <sstream>
#include "File.h"
#include "Options.h"

using namespace Amoebax;

#if defined (IS_WIN32_HOST)
static const char *k_DirectorySeparator = "\\";
#else // !IS_WIN32_HOST
static const char *k_DirectorySeparator = "/";
#endif // IS_WIN32_HOST

#if defined (IS_WIN32_HOST) || defined (IS_GP2X_HOST) || defined (IS_OSX_HOST)
static const char *k_DataDirectory = ".";
#else // !IS_WIN32_HOST && !IS_GP2X_HOST && !IS_OSX_HOST
static const char *k_DataDirectory = DATADIR;
#endif // IS_WIN32_HOST || IS_GP2X_HOST || IS_OSX_HOST


///
/// \brief Changes the current work directory.
///
/// \param directory The directory to change to.
/// \return \a true if the working directory could be changed, \a false
///         otherwise.
///
bool
File::ChangeWorkDirectory (const std::string &directory)
{
    // On Windows and UNIX platforms, it returns 0 on success.
    return chdir (directory.c_str ()) == 0;
}

///
/// \brief Gets the path to a data file.
///
/// \param subdirectory The subdirectory where the file is located
///                     inside the data directory.
/// \param fileName The name of the data file to get its path.
/// \return The absolute or relative path to the data file.
///
std::string
File::getDataFilePath (const std::string &subdirectory,
                       const std::string &fileName)
{
    std::string filePath (k_DataDirectory);
    filePath += k_DirectorySeparator;
    filePath += subdirectory;
    filePath += k_DirectorySeparator;
    filePath += fileName;

    return filePath;
}

///
/// \brief Gets the path to a font file.
///
/// \param fileName The file name of the graphics file to get its path.
/// \return The absolute or relative path to the font file.
///
std::string
File::getFontFilePath (const std::string &fileName)
{
    std::ostringstream fontFilePath;
    fontFilePath << getDataFilePath ("fonts", fileName);
    fontFilePath << Options::getInstance ().getScreenHeight ();
    fontFilePath << ".png";

    return fontFilePath.str ();
}

///
/// \brief Gets the path to a graphics file.
///
/// \param fileName The file name of the graphics file to get its path.
/// \return The absolute or relative path to the graphics file.
///
std::string
File::getGraphicsFilePath (const std::string &fileName)
{
    return getDataFilePath ("graphics", fileName);
}

///
/// \brief Gets the path to a music file.
///
/// \param fileName The file name of the music file to get its path.
/// \return The absolute or relative path to the graphics file.
///
std::string
File::getMusicFilePath (const std::string &fileName)
{
    return getDataFilePath ("music", fileName);
}

///
/// \brief Gets the path to a sound file.
///
/// \param fileName The file name of the sound file to get its path.
/// \return The absolute or relative path to the sound file.
///
std::string
File::getSoundFilePath (const std::string &fileName)
{
    return getDataFilePath ("sfx", fileName);
}
