#ifndef blurryroots_MaterialData
#define blurryroots_MaterialData

#include <yanecos/Data.h>

#include <glm/vec4.hpp>

#include <string>

struct MaterialData : public blurryroots::yanecos::Data<MaterialData> {

	std::string texture_name;
	glm::vec4 color;

	unsigned long shader_reference;

};

#endif
