#ifndef yanecos_IData_h
#define yanecos_IData_h

#include <string>

namespace blurryroots { namespace yanecos {

struct IData {

	virtual
	~IData (void) {};

	virtual const std::string&
	get_type (void) const = 0;

};

}}

#endif
