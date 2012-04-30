###############################################################################
# Library
###############################################################################
add_definitions(
	-DGPLUGIN_COMPILATION
	-DG_LOG_DOMAIN="GPlugin"
)

set(GPLUGIN_HEADERS
	gplugin-core.h
	gplugin-plugin.h
	gplugin-plugin-loader.h
	gplugin-plugin-manager.h
)

set(GPLUGIN_SOURCES
	gplugin-core.c
	gplugin-plugin.c
	gplugin-plugin-loader.c
	gplugin-plugin-manager.c
)

set(GPLUGIN_PUBLIC_BUILT_HEADERS
	gplugin-enums.h
	gplugin-version.h
)

set(GPLUGIN_PRIVATE_BUILT_HEADERS
	gplugin-marshallers.h
)

set(GPLUGIN_PUBLIC_BUILT_SOURCES
	gplugin-enums.c
)

set(GPLUGIN_PRIVATE_BUILT_SOURCES
	gplugin-marshallers.c
)

set(GPLUGIN_NATIVE_HEADERS
	gplugin-native-plugin.h
	gplugin-native-plugin-loader.h
)

set(GPLUGIN_NATIVE_SOURCES
	gplugin-native-plugin.c
	gplugin-native-plugin-loader.c
)

##############################################################################
# Helper Variables
###############################################################################
list(APPEND PUBLIC_HEADERS
	${GPLUGIN_HEADERS} ${GPLUGIN_PUBLIC_BUILT_HEADERS} gplugin.h gplugin-native.h
)
list(SORT PUBLIC_HEADERS)

list(APPEND PRIVATE_HEADERS ${GPLUGIN_PRIVATE_BUILT_HEADERS} gplugin-private.h)

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

# build gplugin-native.h
set(GPLUGIN_NATIVE_H_INCLUDES "")
foreach(HEADER ${GPLUGIN_NATIVE_HEADERS})
	set(GPLUGIN_NATIVE_H_INCLUDES "${GPLUGIN_NATIVE_H_INCLUDES}\n#include <gplugin/${HEADER}>")
endforeach(HEADER)

configure_file(gplugin-native.h.in gplugin-native.h @ONLY)

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
	${GPLUGIN_NATIVE_SOURCES}
	${GPLUGIN_PUBLIC_BUILT_SOURCES}
	${GPLUGIN_PRIVATE_BUILT_SOURCES}
	${GPLUGIN_NATIVE_HEADERS}
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

	gobject_introspection(
		FILENAME GPlugin-0.1.gir
		PACKAGES gobject-2.0
		LIBRARY gplugin
		QUIET
		SCANNER_ARGS --add-include-path=${CMAKE_CURRENT_SOURCE_DIR} --warn-all
		COMPILER_ARGS --includedir=${CMAKE_CURRENT_SOURCE_DIR}
		SYMBOL_PREFIXES gplugin
		SOURCES ${GPLUGIN_SOURCES} ${GPLUGIN_HEADERS}
		BUILT_SOURCES ${GPLUGIN_PUBLIC_BUILT_SOURCES}
		              ${GPLUGIN_PUBLIC_BUILT_HEADERS}
	)
endif(BUILD_GIR)

###############################################################################
# Install Stuff
###############################################################################
# install the main library
install(TARGETS gplugin LIBRARY DESTINATION lib)

# install the single includes into the main directory
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/gplugin.h
	      ${CMAKE_CURRENT_BINARY_DIR}/gplugin-native.h
	DESTINATION include/gplugin-1.0
)

# install the normal includes into the gplugin subdirectory
foreach(HEADER ${GPLUGIN_HEADERS} ${GPLUGIN_NATIVE_HEADERS})
	install(
		FILES ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER}
		DESTINATION include/gplugin-1.0/gplugin
	)
endforeach(HEADER)

# install the built include into the gplugin subdirectory
foreach(HEADER ${GPLUGIN_PUBLIC_BUILT_HEADERS})
	install(
		FILES ${CMAKE_CURRENT_BINARY_DIR}/${HEADER}
		DESTINATION include/gplugin-1.0/gplugin
	)
endforeach(HEADER)

