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
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <SDL.h>
#include <stdexcept>
#include "MainMenuState.h"
#include "Options.h"
#include "System.h"

using namespace Amoebax;

static void parseCommandLine (int argc, char **argv);
static void showUsage (void);
static void showVersion (void);

int
main (int argc, char **argv)
{
    try
    {
        parseCommandLine (argc, argv);
        System::getInstance ().init ();
        System::getInstance ().setActiveState (new MainMenuState (),
                                               System::FadeIn);
        System::getInstance ().run ();
    }
    catch (std::exception &e)
    {
        System::showFatalError (e.what ());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

///
/// \brief Parses the command line for options.
///
/// \param argc The number of arguments from the command line.
/// \param argv The array of command line parameters.
///
void
parseCommandLine (int argc, char **argv)
{
    for ( int currentArgument = 1 ; currentArgument < argc ; ++currentArgument )
    {
        std::string argument (argv[currentArgument]);

        // Fullscreen.
        if ( argument == "-f" || argument == "--fullscreen" )
        {
            Options::getInstance ().setFullScreen (true);
        }

        // Enable sound.
        else if ( argument == "--enable-sound" )
        {
            Options::getInstance ().setSoundEnabled (true);
        }

        // Disable sound
        else if ( argument == "--disable-sound" )
        {
            Options::getInstance ().setSoundEnabled (false);
        }

        // Help.
        else if ( argument == "-h" || argument == "--help" )
        {
            showUsage ();
            exit (EXIT_SUCCESS);
        }

        // Version.
        else if ( argument == "-V" || argument == "--version" )
        {
            showVersion ();
            exit (EXIT_SUCCESS);
        }

        // Windowed mode.
        else if ( argument == "-w" || argument == "--windowed" )
        {
            Options::getInstance ().setFullScreen (false);
        }

        // Unknown parameter.
        else
        {
            std::cerr << "Unknown option: " << argument << std::endl;
            showUsage ();
            throw std::runtime_error ("Unknown command line option.");
        }
    }
}

///
/// \brief Shows the application's usage information.
///
void
showUsage (void)
{
    using namespace std;
    const int optionWidth = 25;

    cout << "Usage: amoebax [OPTION]" << endl;
    cout << endl;
    cout << "Free Puyo-Puyo clone game." << endl;
    cout << endl;

    cout << left << setw (optionWidth) << "  -f, --fullscreen";
    cout << right << "start in full screen mode" << endl;

    cout << left << setw (optionWidth) << "      --enable-sound";
    cout << right << "enables the sound" << endl;

    cout << left << setw (optionWidth) << "      --disable-sound";
    cout << right << "disables the sound" << endl;

    cout << left << setw (optionWidth) << "  -h, --help";
    cout << right << "display this help message and exit" << endl;

    cout << left << setw (optionWidth) << "  -V, --version";
    cout << right << "print version information" << endl;

    cout << left << setw (optionWidth) << "  -w, --windowed";
    cout << right << "start in windowed mode" << endl;

    cout << endl;
    cout << "Report bugs to <" << PACKAGE_BUGREPORT << ">" << endl;
}

///
/// \brief Shows the application's version information.
///
void
showVersion (void)
{
    using namespace std;

    cout << PACKAGE_STRING << endl;
    cout << endl;
    cout << "Copyright (c) 2006, 2007 Emma's Software." << endl;
    cout << "This is free software; see the source for copying ";
    cout << "conditions.  There is NO" << endl;
    cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR ";
    cout << "A PARTICULAR PURPOSE." << endl;
}
