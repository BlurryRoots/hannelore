/*
The MIT License (MIT)

Copyright (c) 2014 Graeme Hill (http://graemehill.ca)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <Guid.h>

#ifdef GUID_LIBUUID
#include <uuid/uuid.h>
#endif

#ifdef GUID_CFUUID
#include <CoreFoundation/CFUUID.h>
#endif

#ifdef GUID_WINDOWS
#include <objbase.h>
#endif

#include <stdexcept>

// overload << so that it's easy to convert to a string
std::ostream&
operator << (std::ostream &s, const Guid &guid) {
	std::ios init (NULL);
	init.copyfmt (s);

	s
		<< std::hex << std::setfill ('0')
		<< std::setw (2) << (int)guid._bytes[0]
		<< std::setw (2) << (int)guid._bytes[1]
		<< std::setw (2) << (int)guid._bytes[2]
		<< std::setw (2) << (int)guid._bytes[3]
		<< "-"
		<< std::setw (2) << (int)guid._bytes[4]
		<< std::setw (2) << (int)guid._bytes[5]
		<< "-"
		<< std::setw (2) << (int)guid._bytes[6]
		<< std::setw (2) << (int)guid._bytes[7]
		<< "-"
		<< std::setw (2) << (int)guid._bytes[8]
		<< std::setw (2) << (int)guid._bytes[9]
		<< "-"
		<< std::setw (2) << (int)guid._bytes[10]
		<< std::setw (2) << (int)guid._bytes[11]
		<< std::setw (2) << (int)guid._bytes[12]
		<< std::setw (2) << (int)guid._bytes[13]
		<< std::setw (2) << (int)guid._bytes[14]
		<< std::setw (2) << (int)guid._bytes[15];
	// restore default formatting
    s.copyfmt (init);

	return s;
}

// converts a single hex char to a number (0 - 15)
unsigned char
hexDigitToChar (char ch) {
	if (ch > 47 && ch < 58) {
		return ch - 48;
	}

	if (ch > 96 && ch < 103) {
		return ch - 87;
	}

	if (ch > 64 && ch < 71) {
		return ch - 55;
	}

	return 0;
}

// converts the two hexadecimal characters to an unsigned char (a byte)
unsigned char
hexPairToChar (char a, char b) {
	return hexDigitToChar (a) * 16 + hexDigitToChar (b);
}

// create empty guid
Guid::Guid (void)
: _bytes (16, 0) {
	//this->_bytes = std::vector<unsigned char> (16, 0);
#ifdef __GUID_DEBUG__
	std::cout << "Guid: Creating empty guid " << *this << std::endl;
#endif
}

// create a guid from vector of bytes
Guid::Guid (const std::vector<unsigned char> &bytes)
: _bytes (bytes) {
	//this->_bytes = bytes;
#ifdef __GUID_DEBUG__
	std::cout << "Guid: From vector" << std::endl;
#endif
}

// create a guid from array of bytes
Guid::Guid (const unsigned char *bytes)
: _bytes (16, 0) {
	this->_bytes.assign (bytes, bytes + 16);
#ifdef __GUID_DEBUG__
	std::cout << "Guid: From byte array" << std::endl;
#endif
}

// create a guid from string
Guid::Guid (const std::string &fromString)
: _bytes (16, 0) {
	//this->_bytes.clear();

	char charOne, charTwo;
	bool lookingForFirstChar = true;

	for (const char &ch : fromString) {
		if (ch == '-') {
			continue;
		}

		if (lookingForFirstChar) {
			charOne = ch;
			lookingForFirstChar = false;
		}
		else {
			charTwo = ch;
			auto byte = hexPairToChar (charOne, charTwo);
			this->_bytes.push_back (byte);
			lookingForFirstChar = true;
		}
	}
#ifdef __GUID_DEBUG__
	std::cout << "Guid: From string" << std::endl;
#endif
}

// copy constructor
Guid::Guid (const Guid &other)
: _bytes (other._bytes) {
	//this->_bytes = other._bytes;
#ifdef __GUID_DEBUG__
	std::cout << "Guid: Created copy of " << other << " into " << *this << std::endl;
#endif
}

Guid::~Guid (void) {
#ifdef __GUID_DEBUG__
	std::cout << "Guid: Destroying " << *this << std::endl;
#endif
}

// overload assignment operator
Guid&
Guid::operator = (const Guid &other) {
	this->_bytes = other._bytes;

	return *this;
}

// overload equality operator
bool
Guid::operator == (const Guid &other) const {
	return this->_bytes == other._bytes;
}

// overload inequality operator
bool
Guid::operator != (const Guid &other) const {
	return ! (this->_bytes == other._bytes);
}

bool
Guid::operator < (const Guid &other) const {
	static size_t n = sizeof (this->_bytes);
#ifdef __GUID_DEBUG__
	std::cout << "Guid: checking less for " << *this << " and " << other << std::endl;
#endif
	for (size_t i = 0; i < n; ++i) {
		if (this->_bytes[i] >= other._bytes[i]) {
			return false;
		}
	}

	return true;
}

bool
Guid::operator > (const Guid &other) const {
	static size_t n = sizeof (this->_bytes);
#ifdef __GUID_DEBUG__
	std::cout << "Guid: checking bigger for " << *this << " and " << other << std::endl;
#endif
	for (size_t i = 0; i < n; ++i) {
		if (this->_bytes[i] <= other._bytes[i]) {
			return false;
		}
	}

	return true;
}

// This is the linux friendly implementation, but it could work on other
// systems that have libuuid available
#ifdef GUID_LIBUUID
Guid
GuidGenerator::newGuid (void) {
	uuid_t id;
	uuid_generate (id);

	//if (-1 == uuid_generate_time_safe (id)) {
	//	throw std::runtime_error  ("Unsynchronized uuid geneartion!");
	//}

	return id;
}

// this is the mac and ios version
#elif GUID_CFUUID
Guid
GuidGenerator::newGuid (void) {
	auto newId = CFUUIDCreate (NULL);
	auto bytes = CFUUIDGetUUIDBytes (newId);
	CFRelease (newId);

	const unsigned char byteArray[16] = {
		bytes.byte0,
		bytes.byte1,
		bytes.byte2,
		bytes.byte3,
		bytes.byte4,
		bytes.byte5,
		bytes.byte6,
		bytes.byte7,
		bytes.byte8,
		bytes.byte9,
		bytes.byte10,
		bytes.byte11,
		bytes.byte12,
		bytes.byte13,
		bytes.byte14,
		bytes.byte15
	};

	return byteArray;
}

// obviously this is the windows version
#elif GUID_WINDOWS
Guid
GuidGenerator::newGuid (void) {
	GUID newId;
	CoCreateGuid (&newId);

	const unsigned char bytes[16] = {
		(newId.Data1 >> 24) & 0xFF,
		(newId.Data1 >> 16) & 0xFF,
		(newId.Data1 >> 8) & 0xFF,
		(newId.Data1) & 0xff,

		(newId.Data2 >> 8) & 0xFF,
		(newId.Data2) & 0xff,

		(newId.Data3 >> 8) & 0xFF,
		(newId.Data3) & 0xFF,

		newId.Data4[0],
		newId.Data4[1],
		newId.Data4[2],
		newId.Data4[3],
		newId.Data4[4],
		newId.Data4[5],
		newId.Data4[6],
		newId.Data4[7]
	};

	return bytes;
}

#else
	fail! "Please specifiy a platform to build Guid Util!"

#endif
