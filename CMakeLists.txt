###############################################################################
# CMake Initialization
###############################################################################
set(CMAKE_LEGACY_CYGWIN_WIN32 0)

cmake_minimum_required(VERSION 2.8.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

if(${CMAKE_VERSION} VERSION_LESS "2.8.3")
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Compatibility)
endif(${CMAKE_VERSION} VERSION_LESS "2.8.3")

###############################################################################
# Project Info
###############################################################################
project(gplugin C)

set(GPLUGIN_MAJOR_VERSION 0)
set(GPLUGIN_MINOR_VERSION 0)
set(GPLUGIN_MICRO_VERSION 14)
set(GPLUGIN_EXTRA_VERSION dev)

set(GPLUGIN_VERSION ${GPLUGIN_MAJOR_VERSION}.${GPLUGIN_MINOR_VERSION}.${GPLUGIN_MICRO_VERSION}${GPLUGIN_EXTRA_VERSION})

set(LOCALE_DIR ${CMAKE_INSTALL_PREFIX}/locale)
add_definitions(-DLOCALEDIR="${LOCALE_DIR}")

###############################################################################
# Options
###############################################################################
option(
	BUILD_GIR
	"Whether or not to build a GObject Introspection type library"
	"On"
)

option(
	NLS
	"Install translation files"
	"On"
)

###############################################################################
# Dependencies
###############################################################################
if(APPLE)
	# homebrew stuff
	set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/usr/local/lib/pkgconfig:/opt/X11/lib/pkgconfig")

	# look for homebrew, if it's installed theres so more mucking we need to do
	find_program(BREW
		NAMES brew
		DOC "brew executable"
	)
	if(BREW)
		execute_process(
			COMMAND ${BREW} --prefix libffi
			OUTPUT_VARIABLE FFI_PREFIX
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
		if(FFI_PREFIX)
			set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${FFI_PREFIX}/lib/pkgconfig")
		endif(FFI_PREFIX)

		execute_process(
			COMMAND ${BREW} --prefix cairo
			OUTPUT_VARIABLE CAIRO_PREFIX
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)

		if(CAIRO_PREFIX)
			set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:${CAIRO_PREFIX}/lib/pkgconfig")
		endif(CAIRO_PREFIX)
	endif(BREW)

	# fink and macports
	set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH}:/sw/lib/pkgconfig:/opt/local/lib/pkgconfig")
endif(APPLE)

include(FindPkgConfig)
include(PkgConfigVariable)

set(REQ_GLIB_VER 2.20.0)

pkg_check_modules(GLIB REQUIRED
	glib-2.0>=${REQ_GLIB_VER}
	gobject-2.0>=${REQ_GLIB_VER}
)

# we separate gmodule out so our test aren't linked to it
pkg_check_modules(GMODULE REQUIRED
	gmodule-2.0>=${REQ_GLIB_VER}
)

pkg_config_variable(glib-2.0 glib_genmarshal GLIB_GENMARSHAL)

if(${GLIB_gobject-2.0_VERSION} LESS 2.32.0)
	set(GLIB_MKENUMS ${CMAKE_SOURCE_DIR}/tools/glib-mkenums
		CACHE FILEPATH "glib-mkenums executable"
	)
else(${GLIB_gobject-2.0_VERSION} LESS 2.32.0)
	pkg_config_variable(glib-2.0 glib_mkenums GLIB_MKENUMS)
endif(${GLIB_gobject-2.0_VERSION} LESS 2.32.0)

find_program(GTESTER
	NAMES gtester
	DOC "gtester executable"
)
mark_as_advanced(GTESTER)

find_program(XSLTPROC
	NAMES xsltproc
	DOC "xsltproc executable"
)
mark_as_advanced(XSLTPROC)

###############################################################################
# NLS
###############################################################################
set(GETTEXT_PACKAGE gplugin)

add_definitions(-DGETTEXT_PACKAGE="${GETTEXT_PACKAGE}")

if(NLS)
	include(Gettextize)
endif(NLS)

###############################################################################
# Build Info
###############################################################################
add_definitions(
	-g -g3 -Wall
	-DPREFIX="${CMAKE_INSTALL_PREFIX}"
	-DGPLUGIN_WEBSITE="http://bitbucket.org/rw_grim/gplugin/"
)

include_directories(
	${CMAKE_SOURCE_DIR}
	${CMAKE_BINARY_DIR}                # for built headers/sources
	${CMAKE_BINARY_DIR}/gplugin        # for gplugin.h
	${GLIB_INCLUDE_DIRS}
	${GMODULE_INCLUDE_DIRS}
)

link_directories(
	${GLIB_LIBRARY_DIRS}
	${GMODULE_LIBRARY_DIRS}
)

###############################################################################
# Subdirectories
###############################################################################
add_subdirectory(gplugin)
add_subdirectory(gplugin-gtk)
add_subdirectory(plugins)
add_subdirectory(po)

add_subdirectory(lua)
add_subdirectory(perl)
add_subdirectory(python)

###############################################################################
# Install stuff
###############################################################################
# documentation
install(FILES ChangeLog INSTALL README HACKING DESTINATION share/doc/gplugin)

