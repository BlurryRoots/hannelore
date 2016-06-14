#include <PathUtil.h>
#include <Util.h>

#ifdef _WIN32
// tells windows.h to not define macros for min () and max ()
#define NOMINMAX 1
#include <windows.h>

#define MAX_FILE_PATH_SIZE MAX_PATH

// strange api which provides PathRemoveFileSpec
#include <Shlwapi.h>
#pragma comment (lib, "shlwapi.lib")

TCHAR* get_windows_executable_path (TCHAR* path, size_t path_size) {
	if (nullptr != path) {
		if (MAX_FILE_PATH_SIZE > path_size) {
			return nullptr;
		}

		DWORD length = GetModuleFileName (NULL, path, path_size);
		PathRemoveFileSpec (path);
	}

	return path;
}
#elif defined (linux)
#include <stdio.h>
#include <unistd.h>

#define MAX_FILE_PATH_SIZE 256

char* get_linux_executable_path (char* buffer, size_t buffer_size) {
	char buffer[MAX_FILE_PATH_SIZE];
	readlink ("/proc/self/exe", buffer, sizeof (buffer));

	return std::string (buffer);
}
#else
assert (false && "Unkonw OS!");
#endif

namespace blurryroots {namespace util {

std::string blurryroots::util::get_executable_path () {
	#if defined (_WIN32)
	TCHAR buffer[MAX_FILE_PATH_SIZE];
	TCHAR* path = get_windows_executable_path (buffer, MAX_FILE_PATH_SIZE);
	#elif defined (linux)
	char buffer[MAX_FILE_PATH_SIZE];
	char* path = get_linux_executable_path (buffer, MAX_FILE_PATH_SIZE);
	#endif

	THROW_IF (nullptr == path,
		"Could not get path from os layer!"
		);


	return std::string (buffer);
}

std::string normalize_file_path (std::string raw_path) {
	static const std::string double_backslash = "\\";
	static const std::string slash = "/";

	size_t start_pos = 0;
	while (std::string::npos != (start_pos = raw_path.find (double_backslash, start_pos))) {
		raw_path.replace (start_pos, double_backslash.length (), slash);
		// In case 'to' contains 'from', like replacing 'x' with 'yx'
		start_pos += slash.length ();
	}

	auto last_char = raw_path[raw_path.size () - 1];
	if ('/' != last_char) {
		raw_path += "/";
	}

	return raw_path;
}

}}
