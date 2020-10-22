add_library(core src/set.c)
target_include_directories(core PUBLIC inc)

add_executable(test_set tests/test_set.c)
target_link_libraries(test_set core)
add_test(test_set test_set)
