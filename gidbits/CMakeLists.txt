###############################################################################
# Library
###############################################################################
add_definitions(
	-DGIDBITS_COMPILATION
)

set(GIDBITS_HEADERS
	gidbits-core.h
	gidbits-plugin.h
	gidbits-plugin-loader.h
	gidbits-plugin-manager.h
)

set(GIDBITS_SOURCES
	gidbits-core.c
	gidbits-plugin.c
	gidbits-plugin-loader.c
	gidbits-plugin-manager.c
)

set(GIDBITS_BUILT_HEADERS
	gidbits-enums.h
	gidbits-version.h
)

set(GIDBITS_BUILT_SOURCES
	gidbits-enums.c
)

###############################################################################
# Configure Files
###############################################################################
configure_file(gidbits-version.h.in gidbits-version.h @ONLY)

# build gidbits.h
set(GIDBITS_H_INCLUDES "")

foreach(HEADER ${GIDBITS_HEADERS} ${GIDBITS_BUILT_HEADERS})
	set(GIDBITS_H_INCLUDES "${GIDBITS_H_INCLUDES}\n#include <gidbits/${HEADER}>")
endforeach(HEADER)

configure_file(gidbits.h.in gidbits.h @ONLY)

###############################################################################
# gidbits-enum.[ch] generation
###############################################################################
set(ENUM_HEADERS
	gidbits-plugin.h
)

add_custom_command(
	OUTPUT gidbits-enums.h
	DEPENDS ${ENUM_HEADERS} gidbits-enums.h.template
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND ${GLIB_MKENUMS} --template gidbits-enums.h.template ${ENUM_HEADERS} >
		${CMAKE_CURRENT_BINARY_DIR}/gidbits-enums.h
)

add_custom_command(
	OUTPUT gidbits-enums.c
	DEPENDS ${ENUM_HEADERS} gidbits-enums.c.template gidbits-enums.h
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	COMMAND ${GLIB_MKENUMS} --template gidbits-enums.c.template ${ENUM_HEADERS} >
		${CMAKE_CURRENT_BINARY_DIR}/gidbits-enums.c
)

###############################################################################
# Library target
###############################################################################
add_library(gidbits SHARED
	${GIDBITS_SOURCES}
	${GIDBITS_BUILT_SOURCES}
	${GIDBITS_HEADERS}
	${GIDBITS_BUILT_HEADERS}
	gidbits.h
	gidbits-private.h
)

target_link_libraries(gidbits
	${GLIB_LIBRARIES}
	${GMODULE_LIBRARIES}
)

