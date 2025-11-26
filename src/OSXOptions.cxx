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
#include <CoreFoundation/CFNumber.h>
#include <CoreFoundation/CFPreferences.h>
#include <CoreFoundation/CFString.h>
#include "OSXOptions.h"

using namespace Amoebax;

///
/// \brief OSXOptions default constructor.
///
OSXOptions::OSXOptions (void):
    Options ()
{
}

bool
OSXOptions::getBooleanValue (const std::string &section,
                             const std::string &name,
                             bool defaultValue)
{
    std::string fullName (section + "." + name);
    CFStringRef keyName = CFStringCreateWithCString (NULL, fullName.c_str (), kCFStringEncodingASCII);
    CFBooleanRef tempValue;
    bool value = defaultValue;

    tempValue = (CFBooleanRef)CFPreferencesCopyAppValue (keyName,
                                                        kCFPreferencesCurrentApplication);
    if ( tempValue )
    {
        value = CFBooleanGetValue (tempValue);
        CFRelease (tempValue);
    }
    CFRelease (keyName);

    return value;
}

int
OSXOptions::getIntegerValue (const std::string &section,
                             const std::string &name,
                             int defaultValue)
{
    std::string fullName (section + "." + name);
    CFStringRef keyName = CFStringCreateWithCString (NULL, fullName.c_str (), kCFStringEncodingASCII);
    CFNumberRef tempValue;
    int value = defaultValue;

    tempValue = (CFNumberRef)CFPreferencesCopyAppValue (keyName,
                                                        kCFPreferencesCurrentApplication);
    if ( tempValue )
    {
        CFNumberGetValue (tempValue, kCFNumberIntType, &value);
        CFRelease (tempValue);
    }
    CFRelease (keyName);

    return value;
}

std::string
OSXOptions::getStringValue (const std::string &section,
                            const std::string &name,
                            const std::string &defaultValue)
{
    std::string fullName (section + "." + name);
    CFStringRef keyName = CFStringCreateWithCString (NULL, fullName.c_str (),
                                                     kCFStringEncodingASCII);
    CFStringRef tempValue;
    std::string value = defaultValue;

    tempValue = (CFStringRef)CFPreferencesCopyAppValue (keyName,
                                                        kCFPreferencesCurrentApplication);
    if ( tempValue )
    {
        unsigned int valueLength =
            CFStringGetMaximumSizeForEncoding (CFStringGetLength (tempValue),
                                               kCFStringEncodingASCII) + 1;
        char *valueString = new char[valueLength];
        CFStringGetCString (tempValue, valueString, valueLength,
                            kCFStringEncodingASCII);
        value = std::string (valueString);
        delete[] valueString;
        CFRelease (tempValue);
    }
    CFRelease (keyName);

    return value;
}

void
OSXOptions::setBooleanValue (const std::string &section,
                             const std::string &name,
                             bool value)
{
    std::string fullName (section + "." + name);
    CFStringRef keyName = CFStringCreateWithCString (NULL, fullName.c_str (), kCFStringEncodingASCII);
    CFPreferencesSetAppValue (keyName, value ? kCFBooleanTrue : kCFBooleanFalse,
                              kCFPreferencesCurrentApplication);
    CFRelease (keyName);
    CFPreferencesAppSynchronize (kCFPreferencesCurrentApplication);
}

void
OSXOptions::setIntegerValue (const std::string &section,
                             const std::string &name,
                             int value)
{
    std::string fullName (section + "." + name);
    CFStringRef keyName = CFStringCreateWithCString (NULL, fullName.c_str (), kCFStringEncodingASCII);
    CFNumberRef tempValue = CFNumberCreate (NULL, kCFNumberIntType, &value);
    CFPreferencesSetAppValue (keyName, tempValue,
                              kCFPreferencesCurrentApplication);
    CFRelease (tempValue);
    CFRelease (keyName);
    CFPreferencesAppSynchronize (kCFPreferencesCurrentApplication);
}

void
OSXOptions::setStringValue (const std::string &section,
                            const std::string &name,
                            const std::string &value)
{
    std::string fullName (section + "." + name);
    CFStringRef keyName = CFStringCreateWithCString (NULL, fullName.c_str (),
                                                     kCFStringEncodingASCII);
    CFStringRef tempValue = CFStringCreateWithCString (NULL, value.c_str (),
                                                       kCFStringEncodingASCII);
    CFPreferencesSetAppValue (keyName, tempValue,
                              kCFPreferencesCurrentApplication);
    CFRelease (tempValue);
    CFRelease (keyName);
    CFPreferencesAppSynchronize (kCFPreferencesCurrentApplication);
}
