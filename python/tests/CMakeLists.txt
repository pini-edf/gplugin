macro(add_python_gtest target)
	add_executable(${target} ${target}.c)
	target_link_libraries(${target} ${GLIB_LIBRARIES} gplugin)
	add_dependencies(${target} gplugin-python)

	get_target_property(_output_name ${target} RUNTIME_OUTPUT_NAME)
	if(NOT ${_output_name})
		get_target_property(_output_name ${target} LOCATION)
	endif(NOT ${_output_name})

	list(APPEND TESTS ${_OUTPUT_NAME})
endmacro(add_python_gtest)

add_definitions(
	-DPYTHON_LOADER_DIR="${CMAKE_BINARY_DIR}/python"
	-DPYTHON_PLUGIN_DIR="${CMAKE_CURRENT_SOURCE_DIR}/plugins"
)

add_python_gtest(test-python-loader)

