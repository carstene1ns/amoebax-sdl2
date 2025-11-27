
# data files
file(GLOB FONTS fonts/*.png)
file(GLOB GRAPHICS graphics/*.png)
file(GLOB MUSIC music/*.ogg)
file(GLOB SFX sfx/*.wav)

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
	# TODO
	# Check for WiX files in order to create Windows installer script.
	find_program(CANDLE candle)
	find_program(LIGHT light)
	#"$(CANDLE)" -nologo -out amoebax.wixobj -dBuildDir="$(top_builddir)" -dSourceDir="$(top_srcdir)" $(top_srcdir)/data/amoebax.wxs
	#"$(LIGHT)" -nologo -out amoebax-@VERSION@.msi -ext WixUtilExtension -ext WixUIExtension -cultures:en-us amoebax.wixobj
	#rm amoebax.wixobj
endif()

if(MACOS)
	# TODO: bundle
	#bundle_contents = @PACKAGE_NAME@.app/Contents
	#bundle: $(top_builddir)/src/amoebax
	#mkdir -p $(bundle_contents)
	#mkdir -p $(bundle_contents)/Frameworks
	#mkdir -p $(bundle_contents)/MacOS
	#mkdir -p $(bundle_contents)/Resources
	#mkdir -p $(bundle_contents)/Resources/fonts
	#mkdir -p $(bundle_contents)/Resources/graphics
	#mkdir -p $(bundle_contents)/Resources/music
	#mkdir -p $(bundle_contents)/Resources/sfx
	#echo "APPL????" > $(bundle_contents)/PkgInfo
	#$(INSTALL_STRIP_PROGRAM) $< $(bundle_contents)/MacOS
	#$(INSTALL_PROGRAM) $(top_builddir)/data/Info.plist $(bundle_contents)
	#$(INSTALL_PROGRAM) $(top_srcdir)/data/Amoebax.icns $(bundle_contents)/Resources
	#$(INSTALL_PROGRAM) $(top_builddir)/data/InfoPlist.strings $(bundle_contents)/Resources/English.lproj
	#$(INSTALL_PROGRAM) $(top_srcdir)/data/fonts/*.png $(bundle_contents)/Resources/fonts
	#$(INSTALL_PROGRAM) $(top_srcdir)/data/graphics/*.png $(bundle_contents)/Resources/graphics
	#$(INSTALL_PROGRAM) $(top_srcdir)/data/music/*.ogg $(bundle_contents)/Resources/music
	#$(INSTALL_PROGRAM) $(top_srcdir)/data/sfx/*.wav $(bundle_contents)/Resources/sfx
endif()
