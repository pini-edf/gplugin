macro(add_test_plugin plugin)
	add_library(${plugin} MODULE ${plugin}.c)
	set_target_properties(${plugin} PROPERTIES PREFIX "")
	target_link_libraries(${plugin} ${GLIB_LIBRARIES} gplugin)
endmacro(add_test_plugin)

add_subdirectory(plugins)
add_subdirectory(bad-plugins)

macro(add_gtest target)
	add_executable(${target} ${target}.c)
	target_link_libraries(${target} ${GLIB_LIBRARIES} gplugin)

	get_target_property(_output_name ${target} RUNTIME_OUTPUT_NAME)
	if(${_output_name})
	else(${_output_name})
		get_target_property(_output_name ${target} LOCATION)
	endif(${_output_name})

	list(APPEND TESTS ${_output_name})
endmacro(add_gtest)

add_gtest(test-plugin-manager-paths)
add_gtest(test-plugin-info)

add_gtest(test-native-loader)
add_dependencies(test-native-loader
	basic-plugin
	dependent-plugin
	broken-dependent-plugin
	query-error
	load-error
	unload-error
)

add_definitions(-DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}/plugins/")
add_definitions(-DTEST_BAD_DIR="${CMAKE_CURRENT_BINARY_DIR}/bad-plugins/")

set(GTESTER_TESTS "${TESTS}")
set(GTESTER_LOG "testgplugin.log")

add_custom_target(tests ALL
	COMMAND ${GTESTER} -k --verbose -o ${GTESTER_LOG} ${GTESTER_TESTS}
	DEPENDS ${GTESTER_TESTS}
)

