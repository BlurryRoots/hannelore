#ifndef __MESHRENDERER_H__
#define __MESHRENDERER_H__

#include <MeshData.h>

#include <memory>
#include <cassert>

class MeshRenderer {

public:
	MeshRenderer (void) {}

	virtual
	~MeshRenderer (void) {}

	void
	render (MeshData *mesh) {
		// indices
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, mesh->indexbuffer);

		// draw
		int size;
		glGetBufferParameteriv (
			GL_ELEMENT_ARRAY_BUFFER,
			GL_BUFFER_SIZE,
			&size
		);
		assert (0 < size);

		int trinagle_count = size / sizeof (GLushort);
		glDrawElements (GL_TRIANGLES, trinagle_count, GL_UNSIGNED_INT, NULL);

		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);

		// TODO: Implement instancing, mesh -> model ??
		//glDrawElements (mode, count, type, indices);
	}

};

#endif
