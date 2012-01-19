###############################################################################
# Library
###############################################################################
add_definitions(
	-DGPLUGIN_COMPILATION
	-DG_LOG_DOMAIN="GPlugin"
)

set(GPLUGIN_HEADERS
	gplugin-core.h
	gplugin-native-plugin.h
	gplugin-native-plugin-loader.h
	gplugin-plugin.h
	gplugin-plugin-loader.h
	gplugin-plugin-manager.h
)

set(GPLUGIN_SOURCES
	gplugin-core.c
	gplugin-native-plugin.c
	gplugin-native-plugin-loader.c
	gplugin-plugin.c
	gplugin-plugin-loader.c
	gplugin-plugin-manager.c
)

set(GPLUGIN_BUILT_HEADERS
	gplugin-enums.h
	gplugin-marshallers.h
	gplugin-version.h
)

set(GPLUGIN_BUILT_SOURCES
	gplugin-enums.c
	gplugin-marshallers.c
)

###############################################################################
# Helper Variables
###############################################################################
list(APPEND PUBLIC_HEADERS ${GPLUGIN_HEADERS} ${GPLUGIN_BUILT_HEADERS} gplugin.h)
list(SORT PUBLIC_HEADERS)

list(APPEND PRIVATE_HEADERS gplugin-private.h)

###############################################################################
# Configure Files
###############################################################################
configure_file(gplugin-version.h.in gplugin-version.h @ONLY)

# build gplugin.h
set(GPLUGIN_H_INCLUDES "")

foreach(HEADER ${PUBLIC_HEADERS})
	if(${HEADER} STREQUAL gplugin.h)
	else(${HEADER} STREQUAL gplugin.h)
		set(GPLUGIN_H_INCLUDES "${GPLUGIN_H_INCLUDES}\n#include <gplugin/${HEADER}>")
	endif(${HEADER} STREQUAL gplugin.h)
endforeach(HEADER)

configure_file(gplugin.h.in gplugin.h @ONLY)

###############################################################################
# gplugin-enum.[ch] generation
###############################################################################
set(ENUM_HEADERS
	gplugin-plugin.h
)

add_custom_command(
	OUTPUT gplugin-enums.h
	DEPENDS ${ENUM_HEADERS} gplugin-enums.h.tmpl
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND ${GLIB_MKENUMS} --template gplugin-enums.h.tmpl ${ENUM_HEADERS} >
		${CMAKE_CURRENT_BINARY_DIR}/gplugin-enums.h
)

add_custom_command(
	OUTPUT gplugin-enums.c
	DEPENDS ${ENUM_HEADERS} gplugin-enums.c.tmpl gplugin-enums.h
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND ${GLIB_MKENUMS} --template gplugin-enums.c.tmpl ${ENUM_HEADERS} >
		${CMAKE_CURRENT_BINARY_DIR}/gplugin-enums.c
)

###############################################################################
# gplugin-marshallers.[ch] generation
###############################################################################
add_custom_command(
	OUTPUT gplugin-marshallers.h
	DEPENDS gplugin-marshallers.list
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND ${GLIB_GENMARSHAL} --header --prefix=gplugin_marshal gplugin-marshallers.list
		> ${CMAKE_CURRENT_BINARY_DIR}/gplugin-marshallers.h
)

add_custom_command(
	OUTPUT gplugin-marshallers.c
	DEPENDS gplugin-marshallers.list
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND ${GLIB_GENMARSHAL} --body --prefix=gplugin_marshal gplugin-marshallers.list
		> ${CMAKE_CURRENT_BINARY_DIR}/gplugin-marshallers.c
)

###############################################################################
# Library target
###############################################################################
add_library(gplugin SHARED
	${GPLUGIN_SOURCES}
	${GPLUGIN_BUILT_SOURCES}
	${PUBLIC_HEADERS}
	${PRIVATE_HEADERS}
)

set_target_properties(gplugin PROPERTIES
	PUBLIC_HEADER "${PUBLIC_HEADERS}"
	PRIVATE_HEADER "${PRIVATE_HEADERS}"
	SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(gplugin
	${GLIB_LIBRARIES}
	${GMODULE_LIBRARIES}
)

###############################################################################
# GObject Introspection
###############################################################################
if(BUILD_GIR)
	include(GObjectIntrospection)

	add_gir_introspection(
		FILENAME gplugin-0.1.gir
		INCLUDES GObject-2.0
		LIBS gplugin
		SCANNER_ARGS "--add-include-path=${CMAKE_CURRENT_SOURCE_DIR}"
		COMPILER_ARGS "--includedir=${CMAKE_CURRENT_SOURCE_DIR}"
		SYMBOL_PREFIXES gplugin
	)
endif(BUILD_GIR)

