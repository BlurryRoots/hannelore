#ifndef __MESH__
#define __MESH__

struct Vertex {
	GLfloat position[3];
	GLfloat uv[2];
};

struct Color {
	GLfloat rgba[4];
};

class Mesh {

private:
	GLuint vertexbuffer;
	std::vector<Vertex> vertices;

	GLuint colorbuffer;
	std::vector<Color> colors;

	GLuint indexbuffer;
	std::vector<GLuint> indices;

	void
	upload_buffer_data (
		GLuint buffer,
		GLenum type,
		const GLvoid * data,
		GLsizeiptr size
	) {
		glBindBuffer (type, buffer);
		glBufferData (type, size, data, GL_STATIC_DRAW);
		glBindBuffer (type, 0);
	}

public:

	float rotation_z;

	Mesh (
		std::vector<Vertex> vertices,
		std::vector<Color> colors,
		std::vector<GLuint> indices
	)
	: 	vertexbuffer (0), vertices (vertices),
		colorbuffer (0), colors (colors),
		indexbuffer (0), indices (indices),
		rotation_z (0) {
			glGenBuffers (1, & vertexbuffer);
			glGenBuffers (1, & colorbuffer);
			glGenBuffers (1, & indexbuffer);
	}

	virtual ~Mesh () {
	}

	void
	dispose () {
		glDeleteBuffers (1, & vertexbuffer);
		glDeleteBuffers (1, & colorbuffer);
		glDeleteBuffers (1, & indexbuffer);
	}

	void
	upload () {
		upload_buffer_data (
			vertexbuffer,
			GL_ARRAY_BUFFER,
			(const GLvoid *)vertices.data (),
			sizeof (vertices[0]) * vertices.size ()
		);

		upload_buffer_data (
			colorbuffer,
			GL_ARRAY_BUFFER,
			(const GLvoid *)colors.data (),
			sizeof (colors[0]) * colors.size ()
		);

		upload_buffer_data (
			indexbuffer,
			GL_ELEMENT_ARRAY_BUFFER,
			(const GLvoid *)indices.data (),
			sizeof (indices[0]) * indices.size ()
		);
	}

	void
	bind () {
		// first attribute is the vertex position x,y,z
		glEnableVertexAttribArray(0);
		glBindBuffer (GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
		   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   sizeof (struct Vertex),    // stride
		   (GLvoid*)0            // array buffer offset
		);
		/* // use this later for uv stuff
		glVertexAttribPointer(
		   0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   2,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   sizeof (struct Vertex),    // stride
		   (GLvoid*) offsetof (struct Vertex, uv)     // array buffer offset
		);
		*/

		glEnableVertexAttribArray (1);
		glBindBuffer (GL_ARRAY_BUFFER, colorbuffer);
		glVertexAttribPointer (
		   1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		   3,                  // size
		   GL_FLOAT,           // type
		   GL_FALSE,           // normalized?
		   0,                  // stride
		   (void*)0            // array buffer offset
		);

		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	}

	void
	draw () {
		int size;
		glGetBufferParameteriv (
			GL_ELEMENT_ARRAY_BUFFER,
			GL_BUFFER_SIZE,
			& size
		);
		if (0 == size) {
			std::cout << "WTF?" << std::endl;
			exit (1);
		}

		int trinagle_count = size / sizeof (GLushort);
		glDrawElements (GL_TRIANGLES, trinagle_count, GL_UNSIGNED_INT, NULL);
	}

	void
	unbind () {
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray (1);
		glDisableVertexAttribArray (0);
	}
};

#endif
