#include <Hashing.h>

namespace blurryroots {
namespace hashing {

namespace djb {

unsigned int hash (std::string s) {
	const std::size_t length = s.length ();
	unsigned int hash = base_hash;

	for (std::size_t i_string = 0; i_string < length; ++i_string) {
		hash = ((hash << hash_shift) + hash) + static_cast<std::size_t> (s[i_string]);
	}

	return hash;
}

}

}}
