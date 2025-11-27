
if(UNIX)
	target_sources(amoebax PRIVATE UnixOptions.cxx UnixOptions.h)
	set(DATADIR "${CMAKE_INSTALL_FULL_DATADIR}/amoebax" CACHE PATH "Path to gamedata")
	target_compile_definitions(amoebax PRIVATE DATADIR="${DATADIR}")
endif()
