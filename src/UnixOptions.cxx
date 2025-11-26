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
#include <cstdlib>
#include <fstream>
#include <libgen.h>
#include <sstream>
#include <unistd.h>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include "UnixOptions.h"

using namespace Amoebax;

///
/// \brief UnixOptions default constructor.
///
UnixOptions::UnixOptions (void):
    Options (),
    m_OptionsAlreadyLoaded (false)
{
}

///
/// \brief UnixOptions destructor.
///
UnixOptions::~UnixOptions (void)
{
    writeOptions ();
}

#if defined (IS_GP2X_HOST)
///
/// \brief Forces to write the current options to the file.
///
void
UnixOptions::flush (void)
{
    writeOptions ();
    sync ();
}
#endif // IS_GP2X_HOST

bool
UnixOptions::getBooleanValue (const std::string &section,
                              const std::string &name,
                              bool defaultValue)
{
    bool value = defaultValue;
    unsigned int integerValue = getIntegerValue (section, name, 0xdeadbeef);
    if ( 0xdeadbeef != integerValue )
    {
        value = 0 == integerValue ? false : true;
    }
    return value;
}

int
UnixOptions::getIntegerValue (const std::string &section,
                              const std::string &name,
                              int defaultValue)
{
    const std::string notFoundString ("0xdeadbeef");
    std::string stringValue = getStringValue (section, name, notFoundString);
    if ( notFoundString != stringValue )
    {
            return atoi (stringValue.c_str ());
    }

    return defaultValue;
}

std::string
UnixOptions::getStringValue (const std::string &section,
                             const std::string &name,
                             const std::string &defaultValue)
{
    if ( !m_OptionsAlreadyLoaded )
    {
        readOptions ();
    }
    std::map<std::string, Section>::const_iterator sectionIterator = m_Sections.find (section);
    if ( m_Sections.end () != sectionIterator )
    {
        const Section &currentSection = sectionIterator->second;
        std::map<std::string, std::string>::const_iterator optionIterator = currentSection.find (name);
        if ( currentSection.end () != optionIterator )
        {
            return std::string (optionIterator->second);
        }
    }

    return defaultValue;
}

///
/// \brief Gets the fill path to the directory to use for options.
///
/// If the directory doesn't exists, this functions makes it.
///
/// \return The full path to the configuration directory.
///
std::string
UnixOptions::getOptionsDirectory (void)
{
#if defined (IS_GP2X_HOST)
    return std::string (".");
#else // IS_GP2X_HOST
    char *configHome = getenv ("XDG_CONFIG_HOME");
    std::string configDirectory (NULL == configHome ? "" : configHome);
    if ( 0 == configDirectory.length () )
    {
        configDirectory += getenv ("HOME");
        configDirectory += "/.config";
    }
    configDirectory += "/amoebax";
    makeDirectory (configDirectory);

    return configDirectory;
#endif // !IS_GP2X_HOST
}

///
/// \brief Gets the full path name of the file to use to read and write options.
///
/// \return The full path file name to use for options.
///
std::string
UnixOptions::getOptionsFileName (void)
{
    return getOptionsDirectory () + "/options.conf";
}

///
/// \brief Recursively makes a directory path.
///
/// \param directory The directory path to make.
///
void
UnixOptions::makeDirectory (const std::string &directory)
{
    struct stat statInfo;
    char *directoryPath = strdup (directory.c_str ());
    char *directoryToCheck = directoryPath;
    std::vector<std::string> directoriesToMake;

    while ( -1 == stat (directoryToCheck, &statInfo) )
    {
        directoriesToMake.push_back (std::string (directoryToCheck));
        directoryToCheck = dirname (directoryToCheck);
    }
    free (directoryPath);

    for ( std::vector<std::string>::reverse_iterator directoryName = directoriesToMake.rbegin () ;
          directoryName != directoriesToMake.rend () ; ++directoryName )
    {
        mkdir (directoryName->c_str (), 0700);
    }
}

///
/// \brief Reads the options from a configuration file.
///
void
UnixOptions::readOptions (void)
{
    std::string fileName = getOptionsFileName ();
    std::ifstream file (fileName.c_str ());
    std::string line;
    Section *currentSection = 0;
    while ( std::getline (file, line) )
    {
        if ( 0 < line.length () &&
             line[0] != '#' && line[1] != ';' )
        {
            if ( line[0] == '[' )
            {
                std::string section = trim (line.substr (1, line.find(']') - 1));
                currentSection = &(m_Sections[section]);
            }
            else if ( 0 != currentSection )
            {
                unsigned int positionEqual = line.find ('=');
                std::string name = trim (line.substr (0, positionEqual));
                std::string value = trim (line.substr (positionEqual+1));

                (*currentSection)[name] = value;
            }
        }
    }
    m_OptionsAlreadyLoaded = true;
}

void
UnixOptions::setBooleanValue (const std::string &section,
                              const std::string &name,
                              bool value)
{
    setIntegerValue (section, name, value ? 1 : 0);
}

void
UnixOptions::setIntegerValue (const std::string &section,
                              const std::string &name,
                              int value)
{
    std::stringstream ss;
    ss << value;
    setStringValue (section, name, ss.str ());
}

void
UnixOptions::setStringValue (const std::string &section,
                             const std::string &name,
                             const std::string &value)
{
    if ( !m_OptionsAlreadyLoaded )
    {
        readOptions ();
    }
    m_Sections[section][name] = value;
}

///
/// \brief Trims an string.
///
/// \param source The source string to trim.
/// \param delims The characters to strip out from \a sources.
/// \return An string without the leading and trailing characters specified
///         in \a delims.
///
std::string
UnixOptions::trim (const std::string &source, const char *delims)
{
    std::string result (source);
    std::string::size_type index = result.find_last_not_of (delims);
    if ( std::string::npos != index )
    {
        result.erase (++index);
    }
    index = result.find_first_not_of (delims);
    if ( std::string::npos != index )
    {
        result.erase (0, index);
    }
    else
    {
        result.erase ();
    }

    return result;
}

///
/// \brief Write the options to the options file.
///
void
UnixOptions::writeOptions (void)
{
    std::ofstream file (getOptionsFileName ().c_str ());
    for ( std::map<std::string, Section>::const_iterator sectionIterator = m_Sections.begin () ;
          sectionIterator != m_Sections.end () ; ++sectionIterator )
    {
        file << "[" << sectionIterator->first << "]" << std::endl;
        const Section &currentSection = sectionIterator->second;
        for ( std::map<std::string, std::string>::const_iterator currentOption = currentSection.begin () ;
              currentOption != currentSection.end () ; ++currentOption )
        {
            file << currentOption->first << " = " << currentOption->second << std::endl;
        }
        file << std::endl;
    }
    file.close ();
}
