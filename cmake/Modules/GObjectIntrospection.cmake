include(FindPkgConfig)

pkg_check_modules(GOBJECT_INTROSPECTION REQUIRED gobject-introspection-1.0)

find_program(G_IR_SCANNER NAMES g-ir-scanner DOC "g-ir-scanner executable")
mark_as_advanced(G_IR_SCANNER)
find_program(G_IR_COMPILER NAMES g-ir-compiler DOC "g-ir-compiler executable")
mark_as_advanced(G_IR_COMPILER)
find_program(G_IR_GENERATE NAMES g-ir-generate DOC "g-ir-generate executable")
mark_as_advanced(G_IR_GENERATE)

macro(add_gir _target)
	# grab the source directory and the sources for the target
	get_target_property(_source_dir ${_target} SOURCE_DIR)
	get_target_property(_sources ${_target} SOURCES)

	get_target_property(_target_defines ${_target} COMPILE_FLAGS)
	get_directory_property(_directory_defines ${_target} COMPILE_FLAGS)

	message("target defines: " ${_target_defines})
	message("directory defines: " ${_directory_defines})

endmacro(add_gir)

