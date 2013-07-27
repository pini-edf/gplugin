include(FindPkgConfig)

macro(pkg_config_variable MODULE VARIABLE OUTPUT_VARIABLE)
	execute_process(
		COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=${VARIABLE} ${MODULE}
		OUTPUT_VARIABLE ${OUTPUT_VARIABLE}
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endmacro(pkg_config_variable)

