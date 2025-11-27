Amoebax
=======

Description
-----------

Amoebax is a free multi-platform match-3 puzzle game where the objective is to
beat your opponent in a battle by filling their grid up to the top with garbage.

You can play as Kim or Tom against six cute creatures controlled by the amoebas
or you can play up to four players in a tournament to check out who is the
amoebas' master.

ChangeLog
---------

See [NEWS.md](NEWS.md).

Requirements
------------

In order to build Amoebax from sources you will need to following libraries:

- SDL2, version 2.0.5 or higher.
- SDL_image, version 2.0.0 or higher with support for PNG.
- SDL_mixer, version 2.0.0 or higher with support for Ogg Vorbis.

All these libraries can be found at: https://libsdl.org/
For the build process CMake is needed, available at https://cmake.org/

Installation
------------

```console
cmake -B builddir [-DCMAKE_INSTALL_DIR=/usr]
cmake --build builddir [-j4]
cmake --install builddir [--prefix DESTDIR]
```

License
-------

This software is licensed under the GNU General Public License (GPL).
The sound effects, music, and graphics are licensed under the Free Art
License (LAL).
For more details about the licenses, please read [COPYING.GPL](COPYING.GPL) and
[COPYING.LAL](COPYING.LAL)
files.

Contact
-------

Amoebax's home page, where the most recent stable version is released, is
located at: https://emma-soft.com/games/amoebax/

If you are a developer and want access to the latest source code, please take a
look at our development page at: https://github.com/carstene1ns/amoebax-sdl2

If you want to reach us by e-mail, you can use the addresses found at the
[AUTHORS.md](AUTHORS.md) file.

Thanks
------

We would like to give thanks to the following people for their support and
contributions:

- Albert Benzal, for testing out the game play.
- Carles Matabosch, for testing out the game play.
- Christian Mata, the help us visualizing the initial idea.
- Joan Ramos, for testing out and some comments.
- Natàlia Brugat, for your time spent trying out the gameplay and help us
  finding bugs :-).
- Pascal Bleser <pascal.bleser@opensuse.org>, for his patch to compile
  amoebax using GCC 4.3.
- Peter Wintle, for your comments.
- Xevir Roig, for helping us testing the gameplay.

If you feel that you should be in this list and I didn't remember to add you in
it, please send us an e-mail/open an issue!
