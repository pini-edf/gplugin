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

set(GPLUGIN_GTK_LD_CURRENT 0)
set(GPLUGIN_GTK_LD_REVISION 1)
set(GPLUGIN_GTK_LD_AGE 0)
set(GPLUGIN_GTK_LD_VERSION ${GPLUGIN_GTK_LD_CURRENT}.${GPLUGIN_GTK_LD_REVISION}.${GPLUGIN_GTK_LD_AGE})

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

include_directories(
	${CHAKE_CURRENT_SOURCE_DIR}
)

set(GPLUGIN_GTK_SOURCES
	gplugin-gtk-plugin-info.c
	gplugin-gtk-store.c
	gplugin-gtk-view.c
)

set(GPLUGIN_GTK_HEADERS
	gplugin-gtk-plugin-info.h
	gplugin-gtk-store.h
	gplugin-gtk-view.h
)

set(GPLUGIN_GTK_PRIVATE_SOURCES
)

set(GPLUGIN_GTK_PRIVATE_HEADERS
)

set(GPLUGIN_GTK_BUILT_SOURCES
)

set(GPLUGIN_GTK_BUILT_HEADERS
)

###############################################################################
# Configure Files
###############################################################################
set(GPLUGIN_GTK_H_INCLUDES "")

foreach(HEADER ${GPLUGIN_GTK_HEADERS} ${GPLUGIN_GTK_BUILT_HEADERS})
	set(GPLUGIN_GTK_H_INCLUDES "${GPLUGIN_GTK_H_INCLUDES}\n#include <gplugin-gtk/${HEADER}>")
endforeach(HEADER)

configure_file(gplugin-gtk.h.in gplugin-gtk.h @ONLY)

configure_file(gplugin-gtk.pc.in gplugin-gtk.pc @ONLY)
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/gplugin-gtk.pc
	DESTINATION lib/pkgconfig
)

###############################################################################
# Library Target
###############################################################################
add_library(gplugin-gtk SHARED
	${GPLUGIN_GTK_SOURCES}
	${GPLUGIN_GTK_HEADERS}
	${GPLUGIN_GTK_PRIVATE_SOURCES}
	${GPLUGIN_GTK_PRIVATE_HEADERS}
	${GPLUGIN_GTK_BUILT_SOURCES}
	${GPLUGIN_GTK_BUILT_HEADERS}
)

set_target_properties(gplugin-gtk PROPERTIES
	SOVERSION ${GPLUGIN_GTK_LD_VERSION}
	VERSION ${GPLUGIN_MAJOR_VERSION}.${GPLUGIN_MINOR_VERSION}
)

target_link_libraries(gplugin-gtk
	${GTK3_LIBRARIES}
	gplugin
)

install(TARGETS gplugin-gtk DESTINATION lib)

# install the single include into the main directory
install(
	FILES ${CMAKE_CURRENT_BINARY_DIR}/gplugin-gtk.h
	DESTINATION include/gplugin-1.0
)

# install the normal includes into the gplugin-gtk subdirectory
foreach(HEADER ${GPLUGIN_GTK_HEADERS})
	install(
		FILES ${CMAKE_CURRENT_SOURCE_DIR}/${HEADER}
		DESTINATION include/gplugin-1.0/gplugin-gtk
	)
endforeach(HEADER)

# install the GtkBuilder files
install(
	FILES gplugin-gtk-plugin-info.ui
	DESTINATION share/gplugin/gplugin-gtk/
)

###############################################################################
# Application Target
###############################################################################
add_executable(gplugin-gtk-viewer
	gplugin-gtk-viewer.c
)

target_link_libraries(gplugin-gtk-viewer
	gplugin-gtk
	gplugin
)

install(TARGETS gplugin-gtk-viewer RUNTIME DESTINATION bin)

endif(BUILD_GTK3)
