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
#if !defined (AMOEBAX_WIN32_OPTIONS_H)
#define AMOEBAX_WIN32_OPTIONS_H

#include "Options.h"

namespace Amoebax
{
    ///
    /// \class Win32Options
    /// \brief Windows system specific options functions.
    ///
    class Win32Options: public Options
    {
        public:
            Win32Options (void);

        protected:
            virtual bool getBooleanValue (const std::string &section,
                                          const std::string &name,
                                          bool defaultValue);
            virtual int getIntegerValue (const std::string &section,
                                         const std::string &name,
                                         int defaultValue);
            virtual std::string getStringValue (const std::string &section,
                                                const std::string &name,
                                                const std::string &defaultValue);
            virtual void setBooleanValue (const std::string &section,
                                          const std::string &name,
                                          bool value);
            virtual void setIntegerValue (const std::string &section,
                                          const std::string &name,
                                          int value);
            virtual void setStringValue (const std::string &section,
                                         const std::string &name,
                                         const std::string &value);

        private:
            // Prevent copies of Win32Options classes.
            ///
            /// \brief Copy constructor.
            ///
            /// \note This constructor is left unimplemented because we
            /// don't want copied of Win32Options objects. Don't use it.
            ///
            /// \param object The source object to copy from.
            ///
            Win32Options (const Win32Options &object);

            ///
            /// \brief Assigment operator.
            ///
            /// \note This operator is left unimplemented because we don't
            /// want copied of Win32Options objects. Don't use it.
            ///
            /// \param object The source object to copy from.
            ///
            Win32Options &operator= (const Win32Options &object);
    };
}

#endif // !AMOBEAX_WIN32_OPTIONS_H
