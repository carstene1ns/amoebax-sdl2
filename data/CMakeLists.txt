
# data files
file(GLOB FONTS fonts/*.png)
file(GLOB GRAPHICS graphics/*.png)
file(GLOB MUSIC music/*.ogg)
file(GLOB SFX sfx/*.wav)
set(SHARED_ASSETS ${FONTS} ${GRAPHICS} ${MUSIC} ${SFX})

if(WIN32)

	target_sources(amoebax PUBLIC
		${SHARED_ASSETS} win32res.rc amoebax.ico amoebax.manifest)

	install(DIRECTORY fonts graphics music sfx DESTINATION ".")

elseif(APPLE)

	target_sources(amoebax PUBLIC ${SHARED_ASSETS} amoebax.icns)
	# macOS Bundle
	set_target_properties(amoebax PROPERTIES
		RESOURCE "amoebax.icns"
		MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist"
		MACOSX_BUNDLE_ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/Amoebax.icns")
	set_source_files_properties(${FONTS} TARGET_DIRECTORY amoebax
		PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/fonts")
	set_source_files_properties(${GRAPHICS} TARGET_DIRECTORY amoebax
		PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/graphics")
	set_source_files_properties(${MUSIC} TARGET_DIRECTORY amoebax
		PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/music")
	set_source_files_properties(${SFX} TARGET_DIRECTORY amoebax
		PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/sfx")

elseif(UNIX)

	# TODO: only Linux tested
	install(DIRECTORY fonts graphics music sfx DESTINATION ${CMAKE_INSTALL_DATADIR}/amoebax)
	install(FILES amoebax.6 DESTINATION ${CMAKE_INSTALL_MANDIR}/man6)
	install(FILES amoebax.desktop DESTINATION ${CMAKE_INSTALL_DATADIR}/applications)
	install(FILES amoebax.png DESTINATION ${CMAKE_INSTALL_DATADIR}/pixmaps) # legacy
	install(FILES amoebax.png DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/32x32/apps)
	install(FILES amoebax.svg DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps)

endif()
