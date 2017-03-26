#pragma once

#ifndef hannelore_TextureData_h
#define hannelore_TextureData_h

#include <yanecos/Data.h>

struct TextureData : public blurryroots::yanecos::Data<TextureData> {
	std::string key;

	unsigned int width;
	unsigned int height;
};

#endif
