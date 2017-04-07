#pragma once

#ifndef blurryroots_PathUtil_h
#define blurryroots_PathUtil_h

#include <string>

namespace blurryroots {namespace util {

std::string get_executable_path ();

std::string normalize_file_path (std::string raw_path);

}}

#endif