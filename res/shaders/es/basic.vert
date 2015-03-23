#version 120

attribute vec3 vertex_position;
attribute vec2 vertex_uv;
attribute vec3 vertex_normal;

varying vec2 fragment_uv;
varying vec3 fragment_normal;
//varying vec3 fragment_pointing_to_lights[4];
varying vec3 point_light_direction;

uniform mat4 m, v, p;
//uniform vec4 point_lights[4];
uniform vec4 point_light;

void
main (void) {
	mat4 vp = p * v;
	vec4 world_position = m * vec4 (vertex_position, 1.0f);
	vec4 world_normal = m * vec4 (vertex_normal, 1.0f);

	gl_Position = vp * world_position;

	//for (int i = 0; i < 4; ++i) {
	//	fragment_pointing_to_lights[i] = world_position.xyz - point_lights[i].xyz;
	//}
	point_light_direction = world_position.xyz - point_light.xyz;

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
