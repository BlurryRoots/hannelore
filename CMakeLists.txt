cmake_minimum_required (VERSION 2.8)
project (hannelore)

## basic setup

# put the executable into the bin directory
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set (ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

## building

# c++11 activation
list (APPEND CMAKE_CXX_FLAGS "-std=c++11")
# compilation flags
add_definitions (-Wall)

# system libraries
set (SYSTEM_LIBS)
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	list (APPEND SYSTEM_LIBS z)
	list (APPEND SYSTEM_LIBS pthread)
endif ()

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

# fontstash-es
include_directories (lib/fontstash-es)

# hannelore core
include_directories (core/inc)
file (GLOB_RECURSE core_files ${PROJECT_SOURCE_DIR}/core/*)
add_library (hannelore-core STATIC
	${core_files}
)
set (HANNELORE_CORE_LIB hannelore-core)


## executables

# core executable
include_directories (game/inc)
file (GLOB_RECURSE game_files ${PROJECT_SOURCE_DIR}/game/*)
add_executable (game ${game_files})
target_link_libraries (game ${HANNELORE_CORE_LIB} ${GLFW} ${GLEW} ${TINYOBJ_LIB} ${SYSTEM_LIBS})

# find every test file
file (GLOB_RECURSE test_files ${PROJECT_SOURCE_DIR}/test/*)
# build test executable
add_executable (test-runner ${test_files})
target_link_libraries (test-runner ${HANNELORE_CORE_LIB})

## custom commands and targets

if (UNIX)
	set (COPY_CMD cp)
	set (COPY_CMD_PARAMS -r)
elseif (WIN32)
	set (COPY_CMD xcopy)
	set (COPY_CMD_PARAMS)
else ()
	message (FATAL_ERROR "Operating system not supported!")
endif ()

# command to copy resources when using default target
add_custom_target (assets
	COMMAND {COPY_CMD} {COPY_CMD_PARAMS} "${PROJECT_SOURCE_DIR}/res/*" $<TARGET_FILE_DIR:game>
	DEPENDS game
)

# 'make run' execute game into the bin directory
add_custom_target (run
	COMMAND ${EXECUTABLE_OUTPUT_PATH}/game
		DEPENDS assets
		WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)

# 'make test'
add_custom_target (tests
	COMMAND ${EXECUTABLE_OUTPUT_PATH}/test-runner -s
		DEPENDS test-runner
		WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)
