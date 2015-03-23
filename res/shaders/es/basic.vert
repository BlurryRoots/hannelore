#version 120

attribute vec3 vertex_position;
attribute vec2 vertex_uv;
attribute vec3 vertex_normal;

varying vec2 fragment_uv;
varying vec3 fragment_normal;
varying vec3 fragment_pointing_to_light0;

uniform mat4 m, v, p;
uniform vec3 LIGHT0;

void
main (void) {
	mat4 vp = p * v;
	vec4 world_position = m * vec4 (vertex_position, 1.0f);
	vec4 world_normal = m * vec4 (vertex_normal, 1.0f);

	gl_Position = vp * world_position;
	fragment_pointing_to_light0 = world_position.xyz - LIGHT0;

	fragment_uv = vertex_uv;

	// Calculate the vertex normal and export it
	//fragment_normal = (mat4 (1)
	//	// transform the normal according to mvp
	//	* p * v * m
	//	// set the vector to be a direction
	//	* vec4 (vertex_normal, 1.0f)).xyz
	//	;
	fragment_normal = world_normal.xyz;
}
