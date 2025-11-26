// Cross-platform free Puyo-Puyo clone.
// Copyright (C) 2006, 2007 Emma's Software
// Based on the work of Darrel Walisser <dwaliss1@purdue.edu> and
// Max Horn <max@quendi.de>.
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
#import <sys/param.h> // for MAXPATHLEN. */
#import <unistd.h>
#import <SDL.h>
#import "AmoebaxMain.h"

#if defined (main)
#undef main
#endif // main

// Global variables where the command line arguments gets stored,
// so we can pass them to SDL_main later.
static int g_argc;
static char **g_argv;

@interface Amoebax: NSApplication
@end

@implementation Amoebax
///
/// \brief Posts a SQL_QUIT event.
///
/// This is called from the `Quit' menu item.
///
- (void)terminate: (id)sender
{
    SDL_Event quitEvent;

    quitEvent.type = SDL_QUIT;
    SDL_PushEvent (&quitEvent);
}

///
/// \brief Ignores keystrokes which doesn't use the the Command key.
///
-(void)sendEvent: (NSEvent *)event
{
    if ( NSKeyDown == [event type] || NSKeyUp == [event type] )
    {
        if ( NSCommandKeyMask == ([event modifierFlags] & NSCommandKeyMask) )
        {
            [super sendEvent: event];
        }
    }
    else
    {
        [super sendEvent: event];
    }
}
@end

@implementation AmoebaxMain
///
/// \brief Sets the working directory to the bundle's resource directory.
///
- (void)setupWorkingDirectory: (BOOL)shouldChdir
{
    if ( shouldChdir )
    {
        char resourcesDirectory[MAXPATHLEN];
        CFURLRef resourcesUrl = CFBundleCopyResourcesDirectoryURL (CFBundleGetMainBundle ());
        if ( CFURLGetFileSystemRepresentation (resourcesUrl, true, (UInt8 *)resourcesDirectory, MAXPATHLEN) )
        {
            assert ( chdir (resourcesDirectory) == 0 );
        }
        CFRelease (resourcesUrl);
    }
}

///
/// \brief Gets called when the internal event loop has just started running.
///
- (void)applicationDidFinishLaunching: (NSNotification *)note
{
    [self setupWorkingDirectory:true];
    int status = SDL_main (g_argc, g_argv);
    // Done!
    exit (status);
}
@end

///
/// \brief Shows an alert.
///
/// \param title The alert's title.
/// \param error The error message.
///
void
OSXAlert (const char *title, const char *error)
{
    NSRunAlertPanel ([NSString stringWithCString: title],
                     [NSString stringWithCString: error],
                     [NSString stringWithCString: "OK"], nil, nil);
}

///
/// \brief Main entry point of executable. Should *not* be SDL_main!
///
int
main (int argc, char **argv)
{
    // This is passed if we are launched by double-clicking. */
    if ( 2 <= argc && strncmp (argv[1], "-psn", 4) == 0 )
    {
        g_argc = 1;
        g_argv = (char **)SDL_malloc (sizeof (char *) * 2);
        g_argv[0] = argv[0];
        g_argv[1] = NULL;
    }
    else
    {
        int currentArgument = 0;

        g_argc = argc;
        g_argv = (char **)SDL_malloc (sizeof (char *) * (argc + 1));
        for ( currentArgument = 0 ; currentArgument <= argc ; ++currentArgument )
        {
            g_argv[currentArgument] = argv[currentArgument];
        }
    }
    [Amoebax poseAsClass:[NSApplication class]];
    NSApplicationMain (argc, (const char **)argv);

    return 0;
}
