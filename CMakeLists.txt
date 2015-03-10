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

# glfw
set (GLFW_BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
set (GLFW_BUILD_TESTS OFF CACHE STRING "" FORCE)
set (GLFW_INSTALL OFF CACHE STRING "" FORCE)
add_subdirectory (lib/glfw)
include_directories (lib/glfw/include)

# glew
add_definitions (-DGLEW_STATIC)
option (glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory (lib/glew)
include_directories (lib/glew/include lib/glew/src)

# glm
include_directories (lib/glm)

# yanecos
include_directories (lib/yanecos/inc)

# app specific includes
include_directories (inc)

# guid library
set (GUID_LIB "")
# check platform
if (${APPLE})
	## if on mac
	add_definitions (-DGUID_CFUUID)
	list (APPEND CMAKE_CXX_FLAGS "-framework CoreFoundation")
elseif (${WIN32})
	## if on windows
	add_definitions (-DGUID_WINDOWS)
elseif (${UNIX})
	## needs uuid-dev package if on ubuntu linux
	add_definitions (-DGUID_LIBUUID)
	set (GUID_LIB "uuid")
else ()
	message (FATAL_ERROR "Platform not supported!")
endif ()

# executables
set (shader_path res/shaders)
#file (COPY ${shader_path} DESTINATION '${EXECUTABLE_OUTPUT_PATH}')
add_executable (main ${source_files} ${shader_files})
target_link_libraries (main glfw ${GLFW_LIBRARIES} libglew_static ${GUID_LIB} pthread)

# command to copy resources when using default target
add_custom_command (TARGET main PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/res"
		$<TARGET_FILE_DIR:main>
)

# 'make run' execute main into the bin directory
add_custom_target (run
    COMMAND ${EXECUTABLE_OUTPUT_PATH}/main
    	DEPENDS main
    	WORKING_DIRECTORY ${EXECUTABLE_OUTPUT_PATH}
)
# command to copy resources when using run command
add_custom_command (TARGET run PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/res"
		$<TARGET_FILE_DIR:main>
)
