#pragma once

#ifndef blurryroots_Util_h
#define blurryroots_Util_h

#include <vector>
#include <sstream>
#include <stdexcept>
#include <string>

#include <stdio.h>

// allows for super class referencing without
// having to care about the type name.
#define DECLARE_SUPER_CLASS(super_class) \
	private: typedef super_class __super;

// for convenient and verbose output of throw_if
// exeption messages.
#ifndef YOLO
#define THROW_IF_SOURCE_LOCATION \
	__FILE__, "@", std::to_string (__LINE__), ": "
#define THROW_IF(premise, ...) \
	blurryroots::util::throw_if (premise, THROW_IF_SOURCE_LOCATION, ##__VA_ARGS__)
#else
#define TROW_IF(premise, ...) \
	if (false) {}
#endif

// for logging debug and warning messages
#ifdef DEBUG
#define LOG_SOURCE_LOCATION \
	std::string (__FILE__) + "@" + std::to_string (__LINE__)

#define DEBUG_LOG_PREFIX "DEBUG_LOG: "
#define DEBUG_LOG(msg) \
	if (blurryroots::util::log_level & blurryroots::util::DebugLogLevel::Log) { \
		blurryroots::util::log (DEBUG_LOG_PREFIX, LOG_SOURCE_LOCATION, "%s", msg); }
#define DEBUG_LOG(msg, ...) \
	if (blurryroots::util::log_level & blurryroots::util::DebugLogLevel::Log) { \
		blurryroots::util::log (DEBUG_LOG_PREFIX, LOG_SOURCE_LOCATION, msg, ##__VA_ARGS__); }

#define DEBUG_WARN_PREFIX "DEBUG_WARN: "
#define DEBUG_WARN(msg) \
	if (blurryroots::util::log_level & blurryroots::util::DebugLogLevel::Warn) { \
		blurryroots::util::log (DEBUG_LOG_PREFIX, LOG_SOURCE_LOCATION, "%s", msg); }
#define DEBUG_WARN(msg, ...) \
	if (blurryroots::util::log_level & blurryroots::util::DebugLogLevel::Warn) { \
		blurryroots::util::log (DEBUG_WARN_PREFIX, LOG_SOURCE_LOCATION, msg, ##__VA_ARGS__); }

#define DEBUG_ERROR_PREFIX "ERROR: "
#define DEBUG_ERROR(msg) \
	if (blurryroots::util::log_level & blurryroots::util::DebugLogLevel::Error) { \
		blurryroots::util::log (DEBUG_ERROR_PREFIX, LOG_SOURCE_LOCATION, "%s", msg); }
#define DEBUG_ERROR(msg, ...) \
	if (blurryroots::util::log_level & blurryroots::util::DebugLogLevel::Error) { \
		blurryroots::util::log (DEBUG_ERROR_PREFIX, LOG_SOURCE_LOCATION, msg, ##__VA_ARGS__); }
#else
// redifine macros so they will be omitted by the compiler
#define DEBUG_LOG(msg) \
	if (false) {}
#define DEBUG_LOG(msg, ...) \
	if (false) {}

#define DEBUG_WARN(msg) \
	if (false) {}
#define DEBUG_WARN(msg, ...) \
	if (false) {}

#define DEBUG_ERROR(msg) \
	if (false) {}
#define DEBUG_ERROR(msg, ...) \
	if (false) {}
#endif

namespace blurryroots { namespace util {

// enum defining possible log levels
enum DebugLogLevel {

	Error = 1,
	Warn = 2,
	Log = 4

};


// redefine logical or for flag shifting
inline DebugLogLevel
operator| (DebugLogLevel a, DebugLogLevel b) {
	return static_cast<DebugLogLevel> (static_cast<int> (a) | static_cast<int> (b));
}

// mark enum variable as extern so it can be accessed by every translation unit
extern DebugLogLevel log_level;

// if given promise is true, throw a runtime_error
template<class... TArgs> static void
throw_if (bool premise, TArgs... args) {
	// if the error condition is not met, abort.
	if (false == premise) {
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

// logging
template<class... TArgs> static void
log (std::string prefix, std::string location, std::string msg, TArgs... args) {
	// collect all arguments into a stringstream.
	std::stringstream ss;
	ss << prefix << " (" << location << "): " << msg << std::endl;
	std::string format_string = ss.str ();

	// fetch raw cstring
	const char *format_cstring = format_string.c_str ();
	printf (format_cstring, args...);
}

bool
has_only_spaces(const std::string& in);

}}

#endif
