#ifndef MESH_H
#define MESH_H

#include <tiny_obj_loader.h>

// GLM
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3

#include <GLHelper.h>

#include <vector>

namespace blurryroots { namespace model {

struct Mesh {

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::vector<glm::vec3> dimensions;

	GLuint vertex_array_object;

	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint uv_buffer;
	GLuint normal_buffer;
	GLuint color_buffer;

	Mesh ();

	Mesh (
		std::vector<tinyobj::shape_t> shapes,
		std::vector<tinyobj::material_t> materials
	);

	Mesh (const Mesh &other);

	virtual
	~Mesh (void);

};

}}

#endif
