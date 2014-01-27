option(
	BUILD_GJS
	"Whether or not to build the GJS JavaScript plugin loader"
	"On"
)

if(BUILD_GJS)
	set(GPLUGIN_GJS_SOURCES
		gplugin-gjs-core.c
		gplugin-gjs-loader.c
		gplugin-gjs-plugin.c
	)

	set(GPLUGIN_GJS_HEADERS
		gplugin-gjs-loader.h
		gplugin-gjs-plugin.h
	)

	pkg_check_modules(GJS REQUIRED gjs-1.0>=1.0.0)
	add_library(gplugin-gjs MODULE
		${GPLUGIN_GJS_SOURCES}
		${GPLUGIN_GJS_HEADERS}
	)
	set_target_properties(gplugin-gjs PROPERTIES PREFIX "")

	include_directories(${GJS_INCLUDE_DIRS})
	target_link_libraries(gplugin-gjs
		${GJS_LIBRARIES}
		gplugin
	)

	install(TARGETS gplugin-gjs DESTINATION lib/gplugin)
endif(BUILD_GJS)

add_subdirectory(tests)

