add_library(core
	src/set.c
	src/map.c
	src/permute.c
)
target_include_directories(core PUBLIC inc)

foreach(test set map permute)
	add_executable(test_${test} tests/test_${test}.c)
	target_link_libraries(test_${test} core)
	add_test(test_${test} test_${test})
endforeach()
