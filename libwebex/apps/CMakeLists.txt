# The core libwebex applications.

add_subdirectory(common)

macro(create_app target)
	add_executable("wbx_${target}" ${target}.cpp)
	target_link_libraries("wbx_${target}" webex common)
endmacro()

create_app(archive)
create_app(print)
create_app(search)