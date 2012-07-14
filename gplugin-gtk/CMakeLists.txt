option(
    BUILD_GTK3
    "Whether or not to build the gtk3 library"
    "On"
)

if(BUILD_GTK3)

###############################################################################
# Build Info
###############################################################################
pkg_check_modules(GTK3 REQUIRED gtk+-3.0>=3.0.0)

add_definitions(
	-DGPLUGIN_GTK_COMPILATION
	-DG_LOG_DOMAIN="GPluginGtk"
)

link_directories(
	${GTK3_LIBRARY_DIRS}
)

include_directories(
	${GTK3_INCLUDE_DIRS}
)

set(GPLUGIN_GTK_SOURCES
	gplugin-gtk-view.c
	gplugin-gtk-store.c
)

set(GPLUGIN_GTK_HEADERS
	gplugin-gtk-view.h
	gplugin-gtk-store.h
)

set(GPLUGIN_GTK_PRIVATE_SOURCES
	gplugin-gtk-private.c
)

set(GPLUGIN_GTK_PRIVATE_HEADERS
	gplugin-gtk-private.h
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
	${GPLUGIN_GTK_PRIVATE_SOURCES}
	${GPLUGIN_GTK_PRIVATE_HEADERS}
)

target_link_libraries(gplugin-gtk
	${GTK3_LIBRARIES}
	gplugin
)

endif(BUILD_GTK3)
