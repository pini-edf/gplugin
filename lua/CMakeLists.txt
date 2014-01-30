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

	include_directories(${LUA51_INCLUDE_DIRS})

	message(STATUS "checking for lua module 'lgi'")

	# compile our lua-lgi test program
	try_compile(LUA_LGI_COMPILE
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/gplugin-lua-test-lgi.c
		CMAKE_FLAGS
			-DINCLUDE_DIRECTORIES:STRING=${LUA51_INCLUDE_DIRS}
			-DLINK_LIBRARIES:STRING=${LUA51_LIBRARIES}
		OUTPUT_VARIABLE OUTPUT
		COPY_FILE ${CMAKE_CURRENT_BINARY_DIR}/gplugin-lua-test-lgi
	)
	if(NOT ${LUA_LGI_COMPILE})
		message(STATUS ${OUTPUT})
		message(FATAL_ERROR "Failed to compile the lua-lgi test")
	endif(NOT ${LUA_LGI_COMPILE})

	# run our lua-lgi test program
	execute_process(
		COMMAND ${CMAKE_CURRENT_BINARY_DIR}/gplugin-lua-test-lgi
		RESULT_VARIABLE LUA_LGI_FOUND
	)

	if(${LUA_LGI_FOUND} EQUAL 0)
		message(STATUS "  found lgi")
	else(${LUA_LGI_FOUND} EQUAL 0)
		message(FATAL_ERROR "  failed to find the 'lgi' lua module")
	endif(${LUA_LGI_FOUND} EQUAL 0)


	# now add the library
	add_library(gplugin-lua MODULE
		${GPLUGIN_LUA_SOURCES}
		${GPLUGIN_LUA_HEADERS}
	)

	set_target_properties(gplugin-lua PROPERTIES PREFIX "")

	target_link_libraries(gplugin-lua
		${LUA51_LIBRARIES}
		gplugin
	)

	install(TARGETS gplugin-lua DESTINATION lib/gplugin)
endif(BUILD_LUA)

add_subdirectory(tests)

