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
#include <string>
#include <windows.h>
#include "Win32Options.h"

using namespace Amoebax;

//
// Constants.
//
static const char *k_RegistrySubkey = "Software\\Emma's Software\\Amoebax\\";

///
/// \brief Win32Options default constructor.
///
Win32Options::Win32Options (void):
    Options ()
{
}

bool
Win32Options::getBooleanValue (const std::string &section,
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
Win32Options::getIntegerValue (const std::string &section,
                               const std::string &name,
                               int defaultValue)
{
    int value = defaultValue;

    std::string regPath (k_RegistrySubkey);
    regPath += section;

    HKEY regKey;
    if ( ERROR_SUCCESS == RegOpenKeyEx (HKEY_CURRENT_USER, regPath.c_str (),
                                        0, KEY_READ, &regKey) )
    {
        DWORD valueSize = sizeof (value);
        RegQueryValueEx (regKey, name.c_str (), NULL, NULL, (BYTE *)(&value),
                         &valueSize);
        RegCloseKey (regKey);
    }

    return value;
}

std::string
Win32Options::getStringValue (const std::string &section,
                              const std::string &name,
                              const std::string &defaultValue)
{
    std::string value = defaultValue;

    std::string regPath (k_RegistrySubkey);
    regPath += section;

    HKEY regKey;
    if ( ERROR_SUCCESS == RegOpenKeyEx (HKEY_CURRENT_USER, regPath.c_str (),
                                        0, KEY_READ, &regKey) )
    {
        char registryValue[512];
        DWORD valueSize = sizeof (registryValue);
        if ( ERROR_SUCCESS == RegQueryValueEx (regKey, name.c_str (), NULL,
                                               NULL, (BYTE *)(&registryValue),
                                               &valueSize) )
        {
            registryValue[valueSize - 1] = '\0';
            value = std::string (registryValue);
        }
        RegCloseKey (regKey);
    }

    return value;
}

void
Win32Options::setBooleanValue (const std::string &section,
                               const std::string &name,
                               bool value)
{
    setIntegerValue (section, name, value ? 1 : 0);
}

void
Win32Options::setIntegerValue (const std::string &section,
                               const std::string &name,
                               int value)
{
    std::string regPath (k_RegistrySubkey);
    regPath += section;

    HKEY regKey;
    if ( ERROR_SUCCESS == RegCreateKeyEx (HKEY_CURRENT_USER, regPath.c_str (),
                                          0, NULL, REG_OPTION_NON_VOLATILE,
                                          KEY_WRITE, NULL, &regKey, NULL) )
    {
        RegSetValueEx (regKey, name.c_str (), 0, REG_DWORD,
                       (BYTE *)(&value), sizeof (value));
        RegCloseKey (regKey);
    }
}

void
Win32Options::setStringValue (const std::string &section,
                              const std::string &name,
                              const std::string &value)
{
    std::string regPath (k_RegistrySubkey);
    regPath += section;

    HKEY regKey;
    if ( ERROR_SUCCESS == RegCreateKeyEx (HKEY_CURRENT_USER, regPath.c_str (),
                                          0, NULL, REG_OPTION_NON_VOLATILE,
                                          KEY_WRITE, NULL, &regKey, NULL) )
    {
        RegSetValueEx (regKey, name.c_str (), 0, REG_SZ,
                       (BYTE *)(value.c_str ()), value.size ());
        RegCloseKey (regKey);
    }
}
