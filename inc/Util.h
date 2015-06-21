#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <sstream>
#include <stdexcept>
#include <string>

// allows for super class referencing without
// having to care about the type name.
#define DECLARE_SUPER_CLASS(super_class) \
	private: typedef super_class __super;

// for convenient and verbose output of throw_if
// exeption messages.
#define SOURCE_LOCATION \
	__FILE__, "@", std::to_string (__LINE__), ": "
#define THROW_IF(premise, ...) \
	blurryroots::util::throw_if (premise, SOURCE_LOCATION, ##__VA_ARGS__)

namespace blurryroots { namespace util {

// if given promise is true, throw a runtime_error
template<class... TArgs> static void
throw_if (bool premise, TArgs... args) {
	// if the error condition is not met, abort.
	if (! premise) {
		return;
	}

	// unpack all further arguments.
	std::vector<std::string> arguments {
		args...
	};

	// collect all arguments into a stringstream.
	std::stringstream ss;
	for (auto &a : arguments) {
		ss << a;
	}
	ss << std::endl;

	// throw runtime error, with collected string as msg.
	throw std::runtime_error (ss.str ());
}

}}

#endif
