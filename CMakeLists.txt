cmake_minimum_required(VERSION 3.18...4.0)

project(Amoebax VERSION 0.2.2
	HOMEPAGE_URL https://emma-soft.com/games/amoebax
	LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)

option(DEBUG "Turn on debugging (default is OFF)" OFF)
if(DEBUG)
	add_compile_definitions(-DDEBUG)
else()
	add_compile_definitions(-DNDEBUG)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(amoebax
	src/AdvancedAIPlayer.cxx src/AdvancedAIPlayer.h
	src/AIPlayer.cxx src/AIPlayer.h
	src/AIPlayerFactory.cxx src/AIPlayerFactory.h
	src/Amoeba.cxx src/Amoeba.h
	src/AnticipatoryAIPlayer.cxx src/AnticipatoryAIPlayer.h
	src/ChainLabel.cxx src/ChainLabel.h
	src/CongratulationsState.cxx src/CongratulationsState.h
	src/ControlSetupState.cxx src/ControlSetupState.h
	src/CreditsState.cxx src/CreditsState.h
	src/DemoState.cxx src/DemoState.h
	src/DrawAmoeba.h src/DrawChainLabel.h
	src/DumbAIPlayer.cxx src/DumbAIPlayer.h
	src/FadeInState.cxx src/FadeInState.h
	src/FadeOutState.cxx src/FadeOutState.h
	src/File.cxx src/File.h
	src/Font.cxx src/Font.h
	src/FrameManager.cxx src/FrameManager.h
	src/Grid.cxx src/Grid.h
	src/GridStatus.cxx src/GridStatus.h
	src/HighScoreState.cxx src/HighScoreState.h
	src/HumanPlayer.cxx src/HumanPlayer.h
	src/IMatchObserver.h
	src/IPlayer.h
	src/IState.h
	src/Joystick.cxx src/Joystick.h
	src/main.cxx
	src/MainMenuState.cxx src/MainMenuState.h
	src/Music.cxx src/Music.h
	src/NewHighScoreState.cxx src/NewHighScoreState.h
	src/NormalSetupState.cxx src/NormalSetupState.h
	src/NormalState.cxx src/NormalState.h
	src/Options.cxx src/Options.h
	src/OptionsMenuState.cxx src/OptionsMenuState.h
	src/PairGenerator.cxx src/PairGenerator.h
	src/PauseState.cxx src/PauseState.h
	src/SimpleAIPlayer.cxx src/SimpleAIPlayer.h
	src/Sound.cxx src/Sound.h
	src/Surface.cxx src/Surface.h
	src/System.cxx src/System.h
	src/TournamentMenuState.cxx src/TournamentMenuState.h
	src/TournamentSetupState.cxx src/TournamentSetupState.h
	src/TournamentState.cxx src/TournamentState.h
	src/TrainingState.cxx src/TrainingState.h
	src/TryAgainState.cxx src/TryAgainState.h
	src/TwoComputerPlayersState.cxx src/TwoComputerPlayersState.h
	src/TwoPlayersState.cxx src/TwoPlayersState.h
	src/VersusState.cxx src/VersusState.h
	src/VideoErrorState.cxx src/VideoErrorState.h)

if(WIN32)
	target_sources(amoebax PRIVATE
		src/win32res.rc	src/Win32Options.cxx src/Win32Options.h)
	target_compile_definitions(amoebax PRIVATE WIN32_LEAN_AND_MEAN IS_WIN32_HOST)
elseif(MACOS)
	target_sources(amoebax PRIVATE
		src/OSXOptions.cxx src/OSXOptions.h)
	target_compile_definitions(amoebax PRIVATE IS_OSX_HOST)
elseif(UNIX)
	target_sources(amoebax PRIVATE
		src/UnixOptions.cxx src/UnixOptions.h)
	target_compile_definitions(amoebax PRIVATE DATADIR="${CMAKE_FULL_DATADIR}")
endif()

# TODO
target_compile_definitions(amoebax PRIVATE 
	PACKAGE_NAME="amoebax" PACKAGE_STRING="amoebax-0.2.2" PACKAGE_BUGREPORT="n/a")

target_link_libraries(amoebax PRIVATE
	SDL2::SDL2 SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer)
