
target_sources(amoebax PRIVATE
	win32/Win32Options.cxx win32/Win32Options.h)
target_compile_definitions(amoebax PRIVATE WIN32_LEAN_AND_MEAN IS_WIN32_HOST)
target_link_libraries(amoebax PRIVATE SDL2::SDL2main)

set_target_properties(amoebax PROPERTIES WIN32_EXECUTABLE TRUE)

set(CMAKE_INSTALL_BINDIR ".")
# Copy .dll files to the target folder.
add_custom_command(TARGET amoebax POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_RUNTIME_DLLS:amoebax> $<TARGET_FILE_DIR:amoebax>
	COMMAND_EXPAND_LISTS
)
if(${CMAKE_VERSION} VERSION_GREATER_EQUAL 3.21)
	install(FILES $<TARGET_RUNTIME_DLLS:amoebax> TYPE BIN)
else()
	message(WARNING "You need at least CMake 3.21 to ship the needed DLLs.")
endif()
