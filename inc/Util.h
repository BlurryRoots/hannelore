#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <sstream>
#include <stdexcept>
#include <string>

#define SOURCE_LOCATION \
	__FILE__, "@", std::to_string (__LINE__), ": "
#define THROW_IF(premise, ...) \
	blurryroots::util::throw_if (premise, SOURCE_LOCATION, ##__VA_ARGS__)

namespace blurryroots { namespace util {

template<class... TArgs> static void
throw_if (bool premise, TArgs... args) {
	if (! premise) {
		return;
	}

	std::vector<std::string> arguments {
		args...
	};

	std::stringstream ss;

	for (auto &a : arguments) {
		ss << a;
	}
	ss << std::endl;

	throw std::runtime_error (ss.str ());
}

}}

#endif
