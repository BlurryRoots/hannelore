cmake_minimum_required (VERSION 2.8)
project (shit)

# put the executable into the bin directory
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# c++11 activation
list (APPEND CMAKE_CXX_FLAGS "-std=c++11")

# compilation flags
add_definitions (-Wall)

# find every source file
file (GLOB_RECURSE source_files ${PROJECT_SOURCE_DIR}/src/*)

# find every source file
file (GLOB_RECURSE test_files ${PROJECT_SOURCE_DIR}/test/*)

# app specific includes
include_directories (inc)
include_directories (lib)

# build test executable
add_executable (test-all ${test_files})

# 'make run' execute main into the bin directory
add_custom_target (test
	COMMAND ${EXECUTABLE_OUTPUT_PATH}/test-all -s
		DEPENDS test-all
		WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)