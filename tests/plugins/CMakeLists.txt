macro(add_plugin plugin)
	add_library(${plugin} MODULE ${plugin}.c)
	set_target_properties(${plugin} PROPERTIES PREFIX "")
endmacro(add_plugin)

add_plugin(basic-plugin)
add_plugin(dependent-plugin)
add_plugin(broken-dependent-plugin)

