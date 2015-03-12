#ifndef __MESHDATA_H__
#define __MESHDATA_H__

#include <yanecos/Data.h>

struct MeshData : public Yanecos::Data<MeshData> {
	std::string key;

	MeshData (const std::string &key)
	: key (key) {}

	virtual
	~MeshData () {}
};

#endif
