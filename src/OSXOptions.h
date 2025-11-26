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
#if !defined (AMOEBAX_OSX_OPTIONS_H)
#define AMOEBAX_OSX_OPTIONS_H

#include "Options.h"

namespace Amoebax
{
    ///
    /// \class OSXOptions
    /// \brief OS X system specific options functions.
    ///
    class OSXOptions: public Options
    {
        public:
            OSXOptions (void);

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
            // Prevent copies of OSXOptions classes.
            ///
            /// \brief Copy constructor.
            ///
            /// \note This constructor is left unimplemented because
            /// we don't want copied of OSXOptions object. Don't use it.
            ///
            /// \param object The source object to copy from.
            ///
            OSXOptions (const OSXOptions &object);

            ///
            /// \brief Assigment operator.
            ///
            /// \note This operator is left unimplemented because we
            /// don't want copied of OSXOptions objects. Don't use it.
            ///
            /// \param object The source object to copy from.
            ///
            OSXOptions &operator= (const OSXOptions &object);
    };
}

#endif // !AMOBEAX_OSX_OPTIONS_H
