
add_library(cute_png STATIC cute_png.h cute_png.cpp)
target_include_directories(cute_png PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(NOT SDL2_mixer_FOUND)
	# STB vorbis is used by default, so no external deps and submodules
	set(SDL2MIXER_CMD OFF CACHE INTERNAL "No external")
	set(SDL2MIXER_FLAC OFF CACHE INTERNAL "No Flac")
	set(SDL2MIXER_MOD OFF CACHE INTERNAL "No Mod")
	set(SDL2MIXER_MP3 OFF CACHE INTERNAL "No Mp3")
	set(SDL2MIXER_MIDI OFF CACHE INTERNAL "No MIDI")
	set(SDL2MIXER_OPUS OFF CACHE INTERNAL "No Opus")
	set(SDL2MIXER_WAVPACK OFF CACHE INTERNAL "No WavPack")

	include(FetchContent)
	FetchContent_Declare(SDL2_MIXER
		GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
		GIT_TAG release-2.8.1
		GIT_SHALLOW TRUE GIT_SUBMODULES "" GIT_PROGRESS TRUE)
	FetchContent_MakeAvailable(SDL2_MIXER)
endif()
