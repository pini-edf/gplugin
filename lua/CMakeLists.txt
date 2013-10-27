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

	find_package(Lua51)
	if(LUA51_FOUND)
		add_library(gplugin-lua MODULE
			${GPLUGIN_LUA_SOURCES}
			${GPLUGIN_LUA_HEADERS}
		)

		set_target_properties(gplugin-lua PROPERTIES PREFIX "")

		include_directories(${LUA_INCLUDE_DIR})
		target_link_libraries(gplugin-lua
			${LUA_LIBRARIES}
			gplugin
		)

		install(TARGETS gplugin-lua DESTINATION lib/gplugin)
	else(LUA51_FOUND)
		message(FATAL_ERROR "Failed to find the lua51 library")
	endif(LUA51_FOUND)
endif(BUILD_LUA)

