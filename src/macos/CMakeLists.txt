
if(MACOS)
	target_sources(amoebax PRIVATE OSXOptions.cxx OSXOptions.h)
	target_compile_definitions(amoebax PRIVATE IS_OSX_HOST)

	# Add SDL2 Framework
	add_custom_command(TARGET amoebax POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_FILE:SDL2::SDL2>
		$<TARGET_FILE_DIR:amoebax>/../Frameworks/$<TARGET_FILE_NAME:SDL2::SDL2>)
	if(NOT "${CMAKE_GENERATOR}" STREQUAL "Xcode")
		install(FILES $<TARGET_FILE:SDL2::SDL2>
			DESTINATION $<TARGET_FILE_DIR:amoebax>/../Frameworks/)
	endif()

	# Set runtime library path
	set_target_properties(amoebax PROPERTIES
		INSTALL_RPATH @executable_path/../Frameworks)
endif()
