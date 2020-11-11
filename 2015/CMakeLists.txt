foreach(day 01 02 03 04 05 06 07)
	add_executable(${day} ${day}.c)
	target_link_libraries(${day} core)
endforeach()
