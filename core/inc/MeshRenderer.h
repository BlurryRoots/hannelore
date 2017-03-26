#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <MeshData.h>
#include <Util.h>
#include <Mesh.h>
#include <ShaderProgram.h>
#include <Transform.h>

#include <GLHelper.h>

#include <memory>
#include <stdexcept>

#define PRIMITIVE_COUNT (std::size_t)12

namespace blurryroots { namespace model {

static bool
is_valid_primitive_type (GLenum type);


class MeshRenderer {

private:
	Mesh *current_mesh;

	GLenum primitive;

public:
	MeshRenderer (void);

	virtual
	~MeshRenderer (void);

	void
	bind (const Mesh *mesh, ShaderProgram &program);

	// program has to be already activated
	void
	render (ShaderProgram &active_program, const Transform *transform);

	void
	unbind (ShaderProgram &program);

	void
	set_primitive (GLenum type);

	GLenum
	get_primitive (void);

};

}}

#endif
