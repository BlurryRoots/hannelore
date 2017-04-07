#include <Util.h>

namespace blurryroots { namespace util {

DebugLogLevel blurryroots::util::log_level = DebugLogLevel::Error
	| DebugLogLevel::Warn
	| DebugLogLevel::Log
	;

bool
has_only_spaces (const std::string& in) {
	for (std::string::const_iterator it = in.begin (); it != in.end (); ++it) {
		// return early if there is something other than a space
		if (*it != ' ') {
			return false;
		}
	}

	return true;
}

}}
