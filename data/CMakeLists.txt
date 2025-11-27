
# data files
file(GLOB FONTS fonts/*.png)
file(GLOB GRAPHICS graphics/*.png)
file(GLOB MUSIC music/*.ogg)
file(GLOB SFX sfx/*.wav)
set(SHARED_STATIC_ASSETS ${FONTS} ${GRAPHICS} ${MUSIC} ${SFX})

if(UNIX)
	install(FILES ${FONTS} DESTINATION ${CMAKE_INSTALL_DATADIR}/amoebax/fonts)
	install(FILES ${GRAPHICS} DESTINATION ${CMAKE_INSTALL_DATADIR}/amoebax/graphics)
	install(FILES ${MUSIC} DESTINATION ${CMAKE_INSTALL_DATADIR}/amoebax/music)
	install(FILES ${SFX} DESTINATION ${CMAKE_INSTALL_DATADIR}/amoebax/sfx)
	install(FILES amoebax.6 DESTINATION ${CMAKE_INSTALL_MANDIR}/man6)
	install(FILES amoebax.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
	install(FILES amoebax.png DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps) # legacy
	install(FILES amoebax.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/32x32/apps)
	install(FILES amoebax.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)
endif()

if(WIN32)
	# TODO: Windows installer script
	set(CPACK_WIX_TEMPLATE amoebax.wxs)
	set(CPACK_WIX_CANDLE_EXTRA_FLAGS
		-dBuildDir="${CMAKE_CURRENT_BINARY_DIR}" -dSourceDir="${PROJECT_SOURCE_DIR}")
	set(CPACK_WIX_LIGHT_EXTENSIONS WixUtilExtension)
	set(CPACK_WIX_CULTURES en-us)
	target_sources(amoebax PUBLIC
		${SHARED_STATIC_ASSETS} win32res.rc amoebax.ico amoebax.manifest)
endif()

if(MACOS)
	# TODO: macOS Bundle
	set(CPACK_BUNDLE_NAME ${PROJECT_NAME}.app)
	set(CPACK_BUNDLE_PLIST Info.plist)
	set(CPACK_BUNDLE_ICON Amoebax.icns)

	set_source_files_properties(${SHARED_STATIC_ASSETS}
		PROPERTIES MACOSX_PACKAGE_LOCATION ${CMAKE_INSTALL_DATADIR})
	target_sources(amoebax PUBLIC ${SHARED_STATIC_ASSETS} amoebax.icns)
	set_target_properties(amoebax PROPERTIES
		RESOURCE "${SHARED_STATIC_ASSETS}"
		MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
		MACOSX_BUNDLE_BUNDLE_VERSION "${BUILD_VERSION}"
		MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
		MACOSX_BUNDLE_GUI_IDENTIFIER "com.emma-soft.amoebax"
		XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "com.emma-soft.amoebax"
		MACOSX_BUNDLE_COPYRIGHT "(c) 2006 Emma's Software")
endif()
