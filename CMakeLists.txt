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

# packaging
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_PACKAGE_VENDOR "Emma's Software")
set(CPACK_PACKAGE_DIRECTORY distribution)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_MONOLITHIC_INSTALL TRUE)
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(CPACK_GENERATOR TGZ Bundle)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(CPACK_GENERATOR TGZ)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(CPACK_GENERATOR ZIP WIX)
else()
	set(CPACK_GENERATOR TGZ)
endif()
include(CPack)
