
target_sources(amoebax PRIVATE
	macos/OSXOptions.cxx macos/OSXOptions.h)
target_compile_definitions(amoebax PRIVATE IS_OSX_HOST)
target_link_libraries(amoebax PRIVATE "-framework CoreFoundation")

# Add SDL2 Frameworks
add_custom_command(TARGET amoebax POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2::SDL2>
		$<TARGET_FILE_DIR:amoebax>/../Frameworks/$<TARGET_FILE_NAME:SDL2::SDL2>
	COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:SDL2_mixer::SDL2_mixer>
		$<TARGET_FILE_DIR:amoebax>/../Frameworks/$<TARGET_FILE_NAME:SDL2_mixer::SDL2_mixer>)
if(NOT "${CMAKE_GENERATOR}" STREQUAL "Xcode")
	install(FILES $<TARGET_FILE:SDL2::SDL2>
		DESTINATION $<TARGET_FILE_DIR:amoebax>/../Frameworks/)
	install(FILES $<TARGET_FILE:SDL2_mixer::SDL2_mixer>
		DESTINATION $<TARGET_FILE_DIR:amoebax>/../Frameworks/)
endif()

# Set runtime library path
set_target_properties(amoebax PROPERTIES
	MACOSX_BUNDLE TRUE
	INSTALL_RPATH @executable_path/../Frameworks
	MACOSX_BUNDLE_BUNDLE_VERSION "${BUILD_VERSION}"
	MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
	MACOSX_BUNDLE_GUI_IDENTIFIER "com.emma-soft.amoebax"
	XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.emma-soft.amoebax"
	MACOSX_BUNDLE_COPYRIGHT "(c) 2007 Emma's Software")
