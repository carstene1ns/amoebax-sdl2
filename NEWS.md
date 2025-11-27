Amoebax 0.2.2
=============

- Dropped GP2X and WinCE support.
- Ported to SDL2/C++11
- Small bugfixes.

Amoebax 0.2.1
=============

- Fixed the problem of players not being centered on tournament's grid in
  GP2X.
- Fixed the problem of the tournament's players selection was a bit off
  in GP2X.
- Fixed the problem of amoebas going down the grid in GP2X's two players mode.
- Fixed the problem with the amoebas in the queue being updated incorrectly
  in GP2X's two players mode. 

Amoebax 0.2.0
=============

- Added demo mode.
- Fade effect between screens.
- The game does not start right away.  It shows a "Ready?" followed by a
  "Go!!" for a short period of time.
- The normal mode can be played either as Kim or as Tom.
- The main amoeba of the falling pair (the amoeba that does not rotate) is now
  visibly different by a glowing silhouette.
- Smooth rotating animations for the falling pair of amoebas and
  animation for the queued amoebas.
- Smooth movement for the pairs waiting on the left/right side of the grid.
- The game now is over when the 3rd column is filled, not when the other
  columns are filled.
- The groups near the top of the grid makes chains now before checking if the
  game is over.
- Fixed the way the score and opponent's ghost amoebas are computed to give
  more importance to the steps rather the number of removed amoebas.
- Implemented the Options menu.
- The keyboard controls are configurable.
- When there is a fatal error the game now shows a platform-specific
  message/alert box instead of dying silently.  On GNU/Linux and GP2X it just
  shows the error at the console.
- Added joystick support. Both players can use a joystick to play.
- Added new levels (5) in the Training Mode.
- Balanced the opponents' difficulty.
- Added new musics, one for each opponent plus the previous for the training
  and menu.
- Added sound effects for winning and losing a game.

Amoebax 0.1.2
=============

- Added the tournament mode.
- Added the two human players mode for GP2X.
- Fixed the problem of red and orange amoebas being too similar.
- Fixed the problem while playing with the last opponent.
- Fixed the problem with amoebas not properly join together.
- High scores are now also stored for normal and tournament modes.

Amoebax 0.1.1
=============

- Added normal mode with six A.I. players.
- Substituted placeholder graphics with sweet new graphics.
- Fixed some minor bugs.

Amoebax 0.1.0
=============

- Initial release.
