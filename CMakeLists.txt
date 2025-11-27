cmake_minimum_required(VERSION 3.18...4.0)

project(Amoebax VERSION 0.2.2
	DESCRIPTION "Cute and addictive action-puzzle game"
	HOMEPAGE_URL https://emma-soft.com/games/amoebax
	LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)

option(DEBUG "Turn on debugging (default is OFF)" OFF)
if(DEBUG)
	add_compile_definitions(DEBUG)
	add_compile_options(-Wall)
else()
	add_compile_definitions(NDEBUG)
endif()

add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(doc)

# packaging
set(DATA_DIR ${CMAKE_SOURCE_DIR}/data)

set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_PACKAGE_VENDOR "Emma's Software")
set(CPACK_PACKAGE_DIRECTORY distribution)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}")
set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})
set(CPACK_MONOLITHIC_INSTALL TRUE)
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	#set(CPACK_DMG_BACKGROUND_IMAGE)
	set(CPACK_GENERATOR TGZ DragNDrop)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(CPACK_GENERATOR TGZ)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(CPACK_WIX_TEMPLATE ${DATA_DIR}/amoebax.wxs)
	set(CPACK_WIX_PRODUCT_GUID "55e7c65a-c44d-11db-8314-0800200c9a66")
	set(CPACK_WIX_UPGRADE_GUID "f8184fb6-0011-44e6-b764-4b2505cc2e1a")
	set(CPACK_WIX_LICENSE_RTF ${DATA_DIR}/License.rtf)
	set(CPACK_WIX_PRODUCT_ICON ${DATA_DIR}/amoebax.ico)
	set(CPACK_WIX_UI_BANNER ${DATA_DIR}/bannrbmp.bmp)
	set(CPACK_WIX_UI_DIALOG ${DATA_DIR}/dlgbmp.bmp)
	set(CPACK_WIX_CULTURES en-us)
	set(CPACK_WIX_CANDLE_EXTRA_FLAGS
		-dBuildDir=${CMAKE_CURRENT_BINARY_DIR} -dSourceDir=${DATA_DIR})
	set(CPACK_WIX_LIGHT_EXTENSIONS WixUtilExtension)
	set(CPACK_GENERATOR ZIP WIX)
else()
	set(CPACK_GENERATOR TGZ)
endif()
include(CPack)
