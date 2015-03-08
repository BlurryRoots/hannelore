#ifndef __IDATA_H__
#define __IDATA_H__

#include <string>

namespace Yanecos {

struct IData {

	virtual
	~IData (void) {};

	virtual const std::string&
	get_type (void) const = 0;

};

}

#endif
