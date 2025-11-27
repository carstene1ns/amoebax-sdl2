
if(WIN32)
	target_sources(amoebax PRIVATE Win32Options.cxx Win32Options.h)
	target_compile_definitions(amoebax PRIVATE WIN32_LEAN_AND_MEAN IS_WIN32_HOST)

	# Copy .dll files to the target build folder.
	add_custom_command(TARGET amoebax POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			$<TARGET_FILE:SDL2::SDL2>
			$<TARGET_FILE_DIR:amoebax>)
	install(FILES $<TARGET_FILE:SDL2::SDL2>
		DESTINATION ${CMAKE_INSTALL_BINDIR})
endif()
