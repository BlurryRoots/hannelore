cmake_minimum_required (VERSION 2.8)
project (hannelore)

## basic setup

# put the executable into the bin directory
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set (ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

# find every source file
file (GLOB_RECURSE source_files ${PROJECT_SOURCE_DIR}/src/*)

# find every test file
file (GLOB_RECURSE test_files ${PROJECT_SOURCE_DIR}/test/*)

## dependencies

# glfw
set (GLFW_BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
set (GLFW_BUILD_TESTS OFF CACHE STRING "" FORCE)
set (GLFW_INSTALL OFF CACHE STRING "" FORCE)
add_subdirectory (lib/glfw)
include_directories (lib/glfw/include)
set (GLFW glfw)
list(APPEND GLFW ${GLFW_LIBRARIES})

# glew
add_definitions (-DGLEW_STATIC)
option (glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory (lib/glew)
include_directories (lib/glew/include lib/glew/src)
set (GLEW libglew_static)

# glm
include_directories (lib/glm)

# yanecos
include_directories (lib/yanecos/inc)

# json
include_directories (lib/json/src)

# obj loader
include_directories (lib/tinyobjloader)
add_library (tinyobjloader STATIC
	lib/tinyobjloader/tiny_obj_loader.h
	lib/tinyobjloader/tiny_obj_loader.cc
)
set (TINYOBJ_LIB tinyobjloader)

# catch test framework (single include file)
include_directories (lib/catch/single_include)

# app specific includes
include_directories (inc)

## building

# c++11 activation
list (APPEND CMAKE_CXX_FLAGS "-std=c++11")
# compilation flags
add_definitions (-Wall)

# system libraries
set (SYSTEM_LIBS)
list (APPEND SYSTEM_LIBS z)
list (APPEND SYSTEM_LIBS pthread)

## executables

# build main executable
add_executable (main ${source_files})
target_link_libraries (main ${GLFW} ${GLEW} ${TINYOBJ_LIB} ${SYSTEM_LIBS})

# build test executable
add_executable (test-runner ${test_files})

## custom commands and targets

# command to copy resources when using default target
add_custom_target (assets
	COMMAND cp -r "${PROJECT_SOURCE_DIR}/res/*" $<TARGET_FILE_DIR:main>
	DEPENDS main
)

# 'make run' execute main into the bin directory
add_custom_target (run
	COMMAND ${EXECUTABLE_OUTPUT_PATH}/main
		DEPENDS assets
		WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

# 'make test'
add_custom_target (tests
	COMMAND ${EXECUTABLE_OUTPUT_PATH}/test-runner -s
		DEPENDS test-runner
		WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)
