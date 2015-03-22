#ifndef MESH_H
#define MESH_H

#include <vector>
#include <tiny_obj_loader.h>

namespace blurryroots { namespace model {

struct Mesh {

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint uv_buffer;
	GLuint normal_buffer;
	GLuint color_buffer;

	Mesh ()
	: shapes ()
	, materials ()
	, vertex_buffer (0)
	, index_buffer (0)
	, uv_buffer (0)
	, normal_buffer (0)
	, color_buffer (0) {}

	Mesh (
		std::vector<tinyobj::shape_t> shapes,
		std::vector<tinyobj::material_t> materials
	)
	: shapes (shapes)
	, materials (materials)
	, vertex_buffer (0)
	, index_buffer (0)
	, uv_buffer (0)
	, normal_buffer (0)
	, color_buffer (0) {}

	Mesh (const Mesh &other)
	: shapes (other.shapes)
	, materials (other.materials)
	, vertex_buffer (other.vertex_buffer)
	, index_buffer (other.index_buffer)
	, uv_buffer (other.uv_buffer)
	, normal_buffer (other.normal_buffer)
	, color_buffer (other.color_buffer) {}

	virtual
	~Mesh (void) {}

};

}}

#endif
