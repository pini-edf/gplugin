option(
	BUILD_LUA
	"Whether or not to build the Lua plugin loader"
	"On"
)

if(BUILD_LUA)
	set(GPLUGIN_LUA_SOURCES
		gplugin-lua-core.c
		gplugin-lua-loader.c
		gplugin-lua-plugin.c
	)

	set(GPLUGIN_LUA_HEADERS
		gplugin-lua-loader.h
		gplugin-lua-plugin.h
	)

	pkg_check_modules(LUA51 REQUIRED lua5.1>=5.1.0)
	add_library(gplugin-lua MODULE
		${GPLUGIN_LUA_SOURCES}
		${GPLUGIN_LUA_HEADERS}
	)

	set_target_properties(gplugin-lua PROPERTIES PREFIX "")

	include_directories(${LUA51_INCLUDE_DIRS})
	target_link_libraries(gplugin-lua
		${LUA51_LIBRARIES}
		gplugin
	)

	install(TARGETS gplugin-lua DESTINATION lib/gplugin)
endif(BUILD_LUA)

add_subdirectory(tests)

