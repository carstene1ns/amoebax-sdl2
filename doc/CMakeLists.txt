
find_package(Doxygen)

configure_file(Doxyfile.in Doxyfile @ONLY)

add_custom_target(build_reference
	${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
	COMMENT "Generating documentation with Doxygen" VERBATIM)
