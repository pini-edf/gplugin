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
	gidbits-version.h
)

add_library(gidbits SHARED
	${GIDBITS_SOURCES}
	${GIDBITS_HEADERS}
	${GIDBITS_BUILT_HEADERS}
	gidbits.h
	gidbits-private.h
)

target_link_libraries(gidbits
	${GLIB_LIBRARIES}
	${GMODULE_LIBRARIES}
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

