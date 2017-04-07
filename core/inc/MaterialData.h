#pragma once

#ifndef blurryroots_MaterialData_h
#define blurryroots_MaterialData_h

#include <yanecos/Data.h>

#include <glm/vec4.hpp>

#include <string>

struct MaterialData : public blurryroots::yanecos::Data<MaterialData> {

	std::string texture_name;
	glm::vec4 color;

	unsigned long shader_reference;

};

#endif
