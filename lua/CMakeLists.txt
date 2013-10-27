option(
	BUILD_LUA
	"Whether or not to build the Lua plugin loader"
	"Off"
)

if(BUILD_LUA)
	set(GPLUGIN_LUA_SOURCES
		gplugin-lua-core.c
	)

	set(GPLUGIN_LUA_HEADERS
	)

	find_package(Lua50)
	if(LUA50_FOUND)
		add_library(gplugin-lua50 MODULE
			${GPLUGIN_LUA_SOURCES}
			${GPLUGIN_LUA_HEADERS}
		)

		set_target_properties(gplugin-lua50 PROPERTIES PREFIX "")

		include_directories(${LUA_INCLUDE_DIR})
		target_link_libraries(gplugin-lua50
			${LUA_LIBRARIES}
			gplugin
		)

		target_compile_definitions(gplugin-lua50 PUBLIC -DLUA_VERSION="50")

		install(TARGETS gplugin-lua50 DESTINATION lib/gplugin)
	else(LUA50_FOUND)
		message(WARNING "Failed to find the lua50 library")
	endif(LUA50_FOUND)

	find_package(Lua51)
	if(LUA51_FOUND)
		add_library(gplugin-lua51 MODULE
			${GPLUGIN_LUA_SOURCES}
			${GPLUGIN_LUA_HEADERS}
		)

		set_target_properties(gplugin-lua51 PROPERTIES PREFIX "")

		include_directories(${LUA_INCLUDE_DIR})
		target_link_libraries(gplugin-lua50
			${LUA_LIBRARIES}
			gplugin
		)

		target_compile_definitions(gplugin-lua51 PUBLIC -DLUA_VERSION="51")

		install(TARGETS gplugin-lua51 DESTINATION lib/gplugin)
	else(LUA51_FOUND)
		message(WARNING "Failed to find the lua51 library")
	endif(LUA51_FOUND)
endif(BUILD_LUA)

