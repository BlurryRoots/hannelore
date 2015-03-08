#ifndef __DATA_H__
#define __DATA_H__

#include <string>
#include <typeinfo>

#include <yanecos/IData.h>

namespace Yanecos {

template<class TDataType>
class Data : public IData {

private:
	std::string type;

public:
	Data (void) {
		this->type = typeid (TDataType).name ();
	};

	virtual
	~Data (void) {}

	const std::string&
	get_type (void) const override {
		return this->type;
	}

};

}

#endif
