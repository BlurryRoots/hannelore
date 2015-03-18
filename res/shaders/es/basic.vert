#version 120

attribute vec3 vertex_position;
attribute vec2 vertex_uv;
attribute vec3 vertex_normal;

varying vec2 fragment_uv;
varying vec3 fragment_normal;

uniform mat4 m, v, p;

void
main (void) {
	mat4 mvp = p * v * m;

	gl_Position = mvp * vec4 (vertex_position, 1.0f);

	fragment_uv = vertex_uv;

	 // Calculate the vertex normal and export it
	fragment_normal = (mat4 (1)
		// transform the normal according to mvp
		* p * v * m
		// set the vector to be a direction
		* vec4 (vertex_normal, 0.0f)).xyz
		;
}
