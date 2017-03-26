#ifndef TEXTUREDATA_H
#define TEXTUREDATA_H

#include <yanecos/Data.h>

struct TextureData : public blurryroots::yanecos::Data<TextureData> {
	std::string key;

	unsigned int width;
	unsigned int height;
};

#endif
