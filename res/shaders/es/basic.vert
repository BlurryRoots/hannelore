#version 120

struct lightSource {
	vec4 position;
	vec4 diffuse;
};
lightSource light0 = lightSource (
	vec4(-1.0, 1.0, -1.0, 0.0),
	vec4(1.0, 1.0, 1.0, 1.0)
);

struct material {
	vec4 diffuse;
};
material mymaterial = material (vec4 (1.0, 0.8, 0.8, 1.0));

attribute vec3 vertex_position;
attribute vec2 vertex_uv;
attribute vec3 vertex_normal;
attribute vec4 vertex_color;

varying vec2 fragment_uv;
varying vec3 fragment_normal;
varying vec4 fragment_color;

uniform mat4 mvp;

void
main (void) {
	gl_Position = mvp * vec4 (vertex_position, 1.0f);

	fragment_color = vertex_color;

	fragment_uv = vertex_uv;

	 // Calculate the vertex normal and export it
	fragment_normal = (mat4 (1)
		// transform the normal according to mvp
		* mvp
		// set the vector to be a direction
		* vec4 (vertex_normal, 0.0f)).xyz
		;
}
