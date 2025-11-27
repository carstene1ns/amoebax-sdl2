
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG QUIET)
if(NOT SDL2_mixer_FOUND)
	include(FindPkgConfig)
	pkg_search_module(SDL2_mixer SDL2_mixer QUIET IMPORTED_TARGET)
	if(SDL2_mixer_FOUND)
		add_library(SDL2_mixer::SDL2_mixer ALIAS PkgConfig::SDL2_mixer)
	endif()
endif()

add_subdirectory(ext)

add_executable(amoebax
	AdvancedAIPlayer.cxx AdvancedAIPlayer.h
	AIPlayer.cxx AIPlayer.h
	AIPlayerFactory.cxx AIPlayerFactory.h
	Amoeba.cxx Amoeba.h
	AnticipatoryAIPlayer.cxx AnticipatoryAIPlayer.h
	ChainLabel.cxx ChainLabel.h
	CongratulationsState.cxx CongratulationsState.h
	ControlSetupState.cxx ControlSetupState.h
	CreditsState.cxx CreditsState.h
	DemoState.cxx DemoState.h
	DrawAmoeba.h DrawChainLabel.h
	DumbAIPlayer.cxx DumbAIPlayer.h
	FadeInState.cxx FadeInState.h
	FadeOutState.cxx FadeOutState.h
	File.cxx File.h
	Font.cxx Font.h
	FrameManager.cxx FrameManager.h
	Grid.cxx Grid.h
	GridStatus.cxx GridStatus.h
	HighScoreState.cxx HighScoreState.h
	HumanPlayer.cxx HumanPlayer.h
	IMatchObserver.h
	IPlayer.h
	IState.h
	Joystick.cxx Joystick.h
	main.cxx
	MainMenuState.cxx MainMenuState.h
	Music.cxx Music.h
	NewHighScoreState.cxx NewHighScoreState.h
	NormalSetupState.cxx NormalSetupState.h
	NormalState.cxx NormalState.h
	Options.cxx Options.h
	OptionsMenuState.cxx OptionsMenuState.h
	PairGenerator.cxx PairGenerator.h
	PauseState.cxx PauseState.h
	SimpleAIPlayer.cxx SimpleAIPlayer.h
	Sound.cxx Sound.h
	Surface.cxx Surface.h
	System.cxx System.h
	TournamentMenuState.cxx TournamentMenuState.h
	TournamentSetupState.cxx TournamentSetupState.h
	TournamentState.cxx TournamentState.h
	TrainingState.cxx TrainingState.h
	TryAgainState.cxx TryAgainState.h
	TwoComputerPlayersState.cxx TwoComputerPlayersState.h
	TwoPlayersState.cxx TwoPlayersState.h
	VersusState.cxx VersusState.h
	VideoErrorState.cxx VideoErrorState.h)
target_include_directories(amoebax PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
	include(win32/win32.cmake)
elseif(APPLE)
	include(macos/macos.cmake)
elseif(UNIX)
	include(unix/unix.cmake)
endif()

target_compile_definitions(amoebax PRIVATE 
	PACKAGE_NAME="${CMAKE_PROJECT_NAME}"
	PACKAGE_STRING="${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}"
	PACKAGE_BUGREPORT="https://github.com/carstene1ns/amoebax-sdl2/issues")

target_link_libraries(amoebax PRIVATE
	SDL2::SDL2 SDL2_mixer::SDL2_mixer cute_png)

# installation

install(TARGETS amoebax
	BUNDLE DESTINATION .
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
