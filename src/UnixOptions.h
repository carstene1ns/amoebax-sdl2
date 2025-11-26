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
#if !defined (AMOEBAX_UNIX_OPTIONS_H)
#define AMOEBAX_UNIX_OPTIONS_H

#include <map>
#include "Options.h"

namespace Amoebax
{
    ///
    /// \class UnixOptions
    /// \brief Unix system specific options functions.
    ///
    class UnixOptions: public Options
    {
        public:
            UnixOptions (void);
            virtual ~UnixOptions (void);
#if defined (IS_GP2X_HOST)
            void flush (void);
#endif // IS_GP2X_HOST

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
            /// An option's section.
            typedef std::map<std::string, std::string> Section;

            // Prevent copies of UnixOption class.
            ///
            /// \brief Copy constructor.
            ///
            /// \note This constructor is left unimplemented because we
            /// don't want copies of UnixOptions objects. Don't use it.
            ///
            UnixOptions (const UnixOptions &);

            ///
            /// \brief Assigment operator.
            ///
            /// \note This operator is left unimplemented because we
            /// don't want copies of UnixOptions objects. Don't use it.
            ///
            UnixOptions &operator= (const UnixOptions &);

            std::string getOptionsDirectory (void);
            std::string getOptionsFileName (void);
            void makeDirectory (const std::string &directory);
            void readOptions (void);
            std::string trim (const std::string &source, const char *delims = " \t\r\n");
            void writeOptions (void);

            /// Tells if the options file was already loaded or not.
            bool m_OptionsAlreadyLoaded;
            /// The option's sections.
            std::map<std::string, Section> m_Sections;
    };
}

#endif // !AMOEBAX_UNIX_OPTIONS_H
