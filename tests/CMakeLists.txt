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

add_subdirectory(plugins)

add_gtest(test-plugin-manager-paths)

add_gtest(test-native-loader)
add_dependencies(test-native-loader basic-plugin dependent-plugin broken-dependent-plugin)

add_definitions(-DTEST_DIR="${CMAKE_CURRENT_BINARY_DIR}/plugins/")

set(GTESTER_TESTS "${TESTS}")
set(GTESTER_LOG "testgplugin.log")

add_custom_target(tests ALL
	COMMAND ${GTESTER} -k --verbose -o ${GTESTER_LOG} ${GTESTER_TESTS}
	DEPENDS ${GTESTER_TESTS}
)

