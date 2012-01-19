include(CMakeParseArguments)
include(FindPkgConfig)

pkg_check_modules(GOBJECT_INTROSPECTION REQUIRED gobject-introspection-1.0)

find_program(GIR_SCANNER NAMES g-ir-scanner DOC "g-ir-scanner executable")
mark_as_advanced(GIR_SCANNER)
find_program(GIR_COMPILER NAMES g-ir-compiler DOC "g-ir-compiler executable")
mark_as_advanced(GIR_COMPILER)
find_program(GIR_GENERATE NAMES g-ir-generate DOC "g-ir-generate executable")
mark_as_advanced(GIR_GENERATE)

macro(_gir_list_prefix _newlist _list _prefix)
	set(${_newlist})
	foreach(_item IN LISTS ${_list})
		list(APPEND ${_newlist} ${_prefix}${_item})
	endforeach(_item)
endmacro(_gir_list_prefix)

function(add_gir_introspection _FIRST_ARG)
	set(oneValueArgs FILENAME PROGRAM PROGRAM_ARG)
	set(multiValueArgs
		SCANNER_ARGS
		COMPILER_ARGS
		SOURCES
		INCLUDES
		CFLAGS
		LIBS
		FILES
		IDENTIFIER_PREFIXES
		SYMBOL_PREFIXES
	)

	CMAKE_PARSE_ARGUMENTS(GIR "" "${oneValueArgs}" "${multiValueArgs}" ${_FIRST_ARG} ${ARGN})

	if(ADD_GIR_UNPARSED_ARGUMENTS)
		message(FATAL_ERROR "Unknown keys given to ADD_GIR_INTROSPECTION(): \"${ADD_GIR_UNPARSED_ARGUMENTS}\"")
	endif(ADD_GIR_UNPARSED_ARGUMENTS)

	# if program has been set, we prepend --program on to it
	if(GIR_PROGRAM)
		set(GIR_PROGRAM "--program=${GIR_PROGRAM}")
	endif(GIR_PROGRAM)

	# if program_arg has been set, we prepend --program-arg on to it
	if(GIR_PROGRAM_ARG)
		set(GIR_PROGRAM_ARG "--program-arg=${GIR_PROGRAM_ARG}")
	endif(GIR_PROGRAM_ARG)

	###########################################################################
	# Clean up any of the multivalue items that all need to be prefixed
	###########################################################################

	# if the user specified IDENTIFIER_PREFIXES we need to prefix each with --identifier-prefix
	if(GIR_IDENTIFIER_PREFIXES)
		_gir_list_prefix(GIR_REAL_IDENTIFIER_PREFIXES GIR_IDENTIFIER_PREFIXES "--identifier-prefix=")
	endif(GIR_IDENTIFIER_PREFIXES)

	# if the user specified SYMBOL_PREFIXES we need to prefix each with --symbol-prefix=
	if(GIR_SYMBOL_PREFIXES)
		_gir_list_prefix(GIR_REAL_SYMBOL_PREFIXES GIR_SYMBOL_PREFIXES "--symbol-prefix=")
	endif(GIR_SYMBOL_PREFIXES)

	###########################################################################
	# Add the custom commands
	###########################################################################
	add_custom_command(
		COMMAND ${GIR_SCANNER} ${GIR_SCANNER_ARGS}
			${GIR_REAL_IDENTIFIER_PREFIXES}
			${GIR_REAL_SYMBOL_PREFIXES}
			--no-libtool
			--output ${CMAKE_CURRENT_BINARY_DIR}/${GIR_FILENAME}
		OUTPUT ${GIR_FILENAME}
		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		VERBATIM
	)

	message("filename: ${GIR_FILENAME}")
	message("symbol prefixes: ${GIR_REAL_SYMBOL_PREFIXES}")
endfunction(add_gir_introspection)

