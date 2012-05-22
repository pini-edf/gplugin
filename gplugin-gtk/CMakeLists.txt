if(BUILD_GTK3)
	pkg_check_modules(GTK3 REQUIRED gtk+-3.0>=3.0.0)

	add_definitions(-DGPLUGIN_GTK_COMPILATION)

	set(GPLUGIN_GTK_SOURCES
		gplugin-gtk-manager.c
	)

	set(GPLUGIN_GTK_HEADERS
		gplugin-gtk-manager.h
	)

	link_directories(
		${GTK3_LIBRARY_DIRS}
	)

	include_directories(
		${GTK3_INCLUDE_DIRS}
	)

	add_library(gplugin-gtk SHARED
		${GPLUGIN_GTK_SOURCES}
		${GPLUGIN_GTK_HEADERS}
	)

	add_dependencies(gplugin-gtk gplugin)

	target_link_libraries(gplugin-gtk
		${GTK3_LIBRARIES}
		gplugin
	)
endif(BUILD_GTK3)
