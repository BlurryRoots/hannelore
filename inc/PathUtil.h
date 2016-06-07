#ifndef PathUtil_H
#define PathUtil_H

#ifdef _WIN32
#define NOMINMAX 1
#include <windows.h>

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

TCHAR* GetThisPath (TCHAR* dest, size_t destSize) {
	if (!dest) return NULL;
	if (MAX_PATH > destSize) return NULL;

	DWORD length = GetModuleFileName (NULL, dest, destSize);
	PathRemoveFileSpec (dest);

	return dest;
}

std::string get_executable_path () {
	TCHAR buffer[MAX_PATH];

	TCHAR *path = GetThisPath (buffer, MAX_PATH);
	THROW_IF (NULL == path,
		"Could not get path from os!"
		);

	return std::string (buffer);
}
#else
#include <stdio.h>
#include <unistd.h>

std::string get_executable_path () {
	char buffer[256];
	readlink ("/proc/self/exe", buffer, sizeof (buffer));

	return std::string (buffer);
}
#endif

#endif