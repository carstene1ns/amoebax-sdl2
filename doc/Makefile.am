# Process this file with automake to produce a Makefile.in file
EXTRA_DIST = manual.pdf manual.sla
if build_reference
reference:
	$(DOXYGEN) Doxyfile
else
reference:

endif

manualdir = $(datarootdir)/doc/amoebax
manual_DATA = manual.pdf

clean-local:
	rm -fr html

all: reference
