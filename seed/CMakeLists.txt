option(
	BUILD_SEED
	"Whether or not to build the seed JavaScript plugin loader"
	"On"
)

if(BUILD_SEED)
	set(GPLUGIN_SEED_SOURCES
		gplugin-seed-core.c
		gplugin-seed-loader.c
		gplugin-seed-plugin.c
	)

	set(GPLUGIN_SEED_HEADERS
		gplugin-seed-loader.h
		gplugin-seed-plugin.h
	)

	pkg_check_modules(SEED REQUIRED seed>=3.0.0)
	add_library(gplugin-seed MODULE
		${GPLUGIN_SEED_SOURCES}
		${GPLUGIN_SEED_HEADERS}
	)
	set_target_properties(gplugin-seed PROPERTIES PREFIX "")

	include_directories(${SEED_INCLUDE_DIRS})
	target_link_libraries(gplugin-seed
		${SEED_LIBRARIES}
		gplugin
	)

	install(TARGETS gplugin-seed DESTINATION lib/gplugin)
endif(BUILD_SEED)

#add_subdirectory(tests)

