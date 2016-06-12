#include <Util.h>

namespace blurryroots { namespace util {

DebugLogLevel blurryroots::util::log_level = DebugLogLevel::Error
	| DebugLogLevel::Warn
	| DebugLogLevel::Log
	;

}}
