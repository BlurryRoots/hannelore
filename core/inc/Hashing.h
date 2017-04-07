#pragma once

#ifndef blurryroots_Hashing_h
#define blurryroots_Hashing_h

#include <string>

namespace blurryroots {
namespace hashing {

namespace djb {

constexpr unsigned int base_hash = 5381;
constexpr unsigned int hash_shift = 5;

unsigned int hash (std::string s);

}

}}

#endif
