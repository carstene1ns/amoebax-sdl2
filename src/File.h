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
#if !defined (AMOEBAX_FILE_H)
#define AMOEBAX_FILE_H

#include <string>

namespace Amoebax
{
    ///
    /// \class File
    /// \brief Filesystem related functions.
    ///
    class File
    {
        public:
            static bool ChangeWorkDirectory (const std::string &directory);
            static std::string getFontFilePath (const std::string &fileName);
            static std::string getGraphicsFilePath (const std::string &fileName);
            static std::string getMusicFilePath (const std::string &fileName);
            static std::string getSoundFilePath (const std::string &fileName);

        private:
            // Since this is an “static only” class, we don't let
            // make new object from this class by making the default and
            // copy constructors private members. The same with the destructor
            // and copy operator.
            ///
            /// \brief Default constructor.
            ///
            File (void);
            ///
            /// \brief Copy constructor.
            ///
            /// \note This constructor is left unimplemented because we don't
            /// want to make copies from a File object. Don't use it.
            ///
            /// \param file The source object to make a copy from.
            ///
            File (const File &file);
            ///
            /// \brief Destructor.
            ///
            ~File (void);
            ///
            /// \brief Assignment operator.
            ///
            /// \note This operator is left unimplemented because we don't
            /// want to copy Font objects. Don't use it.
            ///
            /// \param file The source object to assign to this object.
            ///
            File &operator= (const File &file);

            static std::string getDataFilePath (const std::string &subdirectory,
                                                const std::string &fileName);
    };
}

#endif // !AMOEBAX_FILE_H
