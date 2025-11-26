# Process this file with automake to produce a Makefile.in file
EXTRA_DIST =	\
	OSXOptions.cxx	\
	OSXOptions.h	\
	UnixOptions.cxx	\
	UnixOptions.h	\
	Win32Options.cxx	\
	Win32Options.h	\
	win32res.rc

if IS_OSX_HOST
OSXSOURCES =	\
	OSXOptions.cxx	\
	OSXOptions.h
else # !IS_OSX_HOST
if IS_WIN32_HOST
WIN32CFLAGS = -DWIN32_LEAN_AND_MEAN

WIN32SOURCES=	\
	Win32Options.cxx	\
	Win32Options.h

if build_win32_resource
WIN32LDADD =	\
	win32res.o

win32res.o: win32res.rc
	$(WINDRES) -i $? -I rc --include-dir=$(top_srcdir)/data -o $@ -O coff
endif # build_win32_resource
else # !IS_WIN32_HOST
UNIXCFLAGS = -DDATADIR=\"$(pkgdatadir)\"

UNIXSOURCES =	\
	UnixOptions.cxx	\
	UnixOptions.h
endif # IS_WIN32_HOST
endif # IS_OSX_HOST

bin_PROGRAMS = amoebax
amoebax_SOURCES =	\
	$(OSXSOURCES)	\
	$(UNIXSOURCES)	\
	$(WIN32SOURCES)	\
	AdvancedAIPlayer.cxx	\
	AdvancedAIPlayer.h	\
	AIPlayer.cxx	\
	AIPlayer.h	\
	AIPlayerFactory.cxx \
	AIPlayerFactory.h   \
	Amoeba.cxx	\
	Amoeba.h	\
	AnticipatoryAIPlayer.cxx	\
	AnticipatoryAIPlayer.h	\
	ChainLabel.cxx	\
	ChainLabel.h	\
	CongratulationsState.cxx	\
	CongratulationsState.h	\
	ControlSetupState.cxx	\
	ControlSetupState.h	\
	CreditsState.cxx	\
	CreditsState.h	\
	DemoState.cxx	\
	DemoState.h	\
	DrawAmoeba.h	\
	DrawChainLabel.h	\
	DumbAIPlayer.cxx	\
	DumbAIPlayer.h	\
	FadeInState.cxx	\
	FadeInState.h	\
	FadeOutState.cxx	\
	FadeOutState.h	\
	FrameManager.cxx	\
	FrameManager.h	\
	File.cxx	\
	File.h	\
	Font.cxx	\
	Font.h	\
	Grid.cxx	\
	Grid.h	\
	GridStatus.cxx	\
	GridStatus.h	\
	gp2x.h	\
	HighScoreState.cxx	\
	HighScoreState.h	\
	HumanPlayer.cxx	\
	HumanPlayer.h	\
	IMatchObserver.h	\
	IPlayer.h	\
	IState.h	\
	Joystick.cxx	\
	Joystick.h	\
	main.cxx	\
	MainMenuState.cxx	\
	MainMenuState.h	\
	Music.cxx	\
	Music.h	\
	NewHighScoreState.cxx	\
	NewHighScoreState.h	\
	NormalSetupState.cxx	\
	NormalSetupState.h	\
	NormalState.cxx	\
	NormalState.h	\
	Options.cxx	\
	Options.h	\
	OptionsMenuState.cxx	\
	OptionsMenuState.h	\
	PairGenerator.cxx	\
	PairGenerator.h	\
	PauseState.cxx	\
	PauseState.h	\
	SimpleAIPlayer.cxx	\
	SimpleAIPlayer.h	\
	Sound.cxx	\
	Sound.h	\
	Surface.cxx	\
	Surface.h	\
	System.cxx	\
	System.h	\
	TournamentMenuState.cxx	\
	TournamentMenuState.h	\
	TournamentSetupState.cxx	\
	TournamentSetupState.h	\
	TournamentState.cxx	\
	TournamentState.h	\
	TrainingState.cxx	\
	TrainingState.h	\
	TryAgainState.cxx   \
	TryAgainState.h \
	TwoComputerPlayersState.cxx	\
	TwoComputerPlayersState.h   \
	TwoPlayersState.cxx	\
	TwoPlayersState.h   \
	VersusState.cxx \
	VersusState.h	\
	VideoErrorState.cxx	\
	VideoErrorState.h

amoebax_CXXFLAGS =	\
	$(UNIXCFLAGS)	\
	$(WIN32CFLAGS)	\
	$(SDL_CFLAGS)	\
	$(SDL_IMAGE_CFLAGS)	\
	$(SDL_MIXER_CFLAGS)

amoebax_OBJCFLAGS =	\
	$(SDL_CFLAGS)	\
	$(SDL_IMAGE_CFLAGS)	\
	$(SDL_MIXER_CFLAGS)

amoebax_LDADD =	\
	$(WIN32LDADD)	\
	$(SDL_LIBS)	\
	$(SDL_IMAGE_LIBS)	\
	$(SDL_MIXER_LIBS)
