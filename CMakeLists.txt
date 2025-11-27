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

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(doc)
