if(BUILD_GTK3)

###############################################################################
# Build Info
###############################################################################
pkg_check_modules(GTK3 REQUIRED gtk+-3.0>=3.0.0)

add_definitions(-DGPLUGIN_GTK_COMPILATION)

link_directories(
	${GTK3_LIBRARY_DIRS}
)

include_directories(
	${GTK3_INCLUDE_DIRS}
)

set(GPLUGIN_GTK_SOURCES
	gplugin-gtk-manager.c
)

set(GPLUGIN_GTK_HEADERS
	gplugin-gtk-manager.h
)

###############################################################################
# Configure Files
###############################################################################
set(GPLUGIN_GTK_H_INCLUDES "")

foreach(HEADER ${GPLUGIN_GTK_HEADERS} ${GPLUGIN_GTK_BUILT_HEADERS})
	set(GPLUGIN_GTK_H_INCLUDES "${GPLUGIN_GTK_H_INCLUDES}\n#include <gplugin-gtk/${HEADER}>")
endforeach(HEADER)

configure_file(gplugin-gtk.h.in gplugin-gtk.h @ONLY)

###############################################################################
# Library Target
###############################################################################
add_library(gplugin-gtk SHARED
	${GPLUGIN_GTK_SOURCES}
	${GPLUGIN_GTK_HEADERS}
)

target_link_libraries(gplugin-gtk
	${GTK3_LIBRARIES}
	gplugin
)

endif(BUILD_GTK3)
