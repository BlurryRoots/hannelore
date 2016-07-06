#ifndef yanecos_Data_h
#define yanecos_Data_h

#include <string>
#include <typeinfo>

#include <yanecos/IData.h>

namespace blurryroots { namespace yanecos {

template<class TDataType>
class Data : public IData {

public:
	const std::string
	get_type (void) const override final {
		return this->type;
	}

	Data (void) {
		this->type = typeid (TDataType).name ();
	};

	virtual
	~Data (void) {}

private:
	std::string type;

};

}}

#endif
