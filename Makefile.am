# Process this file with automake to produce a Makefile.in file
SUBDIRS = autopackage data doc m4 src

ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST =	\
	COPYING.GPL	\
	COPYING.LAL	\
	README.GP2X

if IS_WIN32_HOST

if build_win32_msi
msi:
	"$(CANDLE)" -nologo -out amoebax.wixobj -dBuildDir="$(top_builddir)" -dSourceDir="$(top_srcdir)" $(top_srcdir)/data/amoebax.wxs
	"$(LIGHT)" -nologo -out amoebax-@VERSION@.msi -ext WixUtilExtension -ext WixUIExtension \
		 -cultures:en-us amoebax.wixobj
	rm amoebax.wixobj
endif

endif

if IS_OSX_HOST

bundle_contents = @PACKAGE_NAME@.app/Contents
bundle: $(top_builddir)/src/amoebax
	mkdir -p $(bundle_contents)
	mkdir -p $(bundle_contents)/Frameworks
	mkdir -p $(bundle_contents)/MacOS
	mkdir -p $(bundle_contents)/Resources
	mkdir -p $(bundle_contents)/Resources/fonts
	mkdir -p $(bundle_contents)/Resources/graphics
	mkdir -p $(bundle_contents)/Resources/music
	mkdir -p $(bundle_contents)/Resources/sfx
	mkdir -p $(bundle_contents)/Resources/English.lproj
	mkdir -p $(bundle_contents)/Resources/English.lproj/AmoebaxMain.nib
	echo "APPL????" > $(bundle_contents)/PkgInfo
	$(INSTALL_STRIP_PROGRAM) $< $(bundle_contents)/MacOS
	$(INSTALL_PROGRAM) $(top_builddir)/data/Info.plist $(bundle_contents)
	$(INSTALL_PROGRAM) $(top_srcdir)/data/Amoebax.icns $(bundle_contents)/Resources
	$(INSTALL_PROGRAM) $(top_builddir)/data/InfoPlist.strings $(bundle_contents)/Resources/English.lproj
	$(INSTALL_PROGRAM) $(top_srcdir)/data/AmoebaxMain.nib/classes.nib $(bundle_contents)/Resources/English.lproj/AmoebaxMain.nib
	$(INSTALL_PROGRAM) $(top_srcdir)/data/AmoebaxMain.nib/info.nib $(bundle_contents)/Resources/English.lproj/AmoebaxMain.nib
	$(INSTALL_PROGRAM) $(top_srcdir)/data/AmoebaxMain.nib/objects.nib $(bundle_contents)/Resources/English.lproj/AmoebaxMain.nib
	$(INSTALL_PROGRAM) $(top_srcdir)/data/fonts/*.png $(bundle_contents)/Resources/fonts
	$(INSTALL_PROGRAM) $(top_srcdir)/data/graphics/*.png $(bundle_contents)/Resources/graphics
	$(INSTALL_PROGRAM) $(top_srcdir)/data/music/*.ogg $(bundle_contents)/Resources/music
	$(INSTALL_PROGRAM) $(top_srcdir)/data/sfx/*.wav $(bundle_contents)/Resources/sfx

endif
