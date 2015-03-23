//#version 120

// first define version of opengl es 1.0 and update to opengl es 2.0 (WTF?)
#version 100
#define GLES2

//#version 300

attribute highp vec3 vertex_position;
attribute highp vec2 vertex_uv;
attribute highp vec3 vertex_normal;

varying highp vec2 fragment_uv;
varying highp vec3 fragment_normal;
varying highp vec3 point_light_directions[4];
varying highp vec4 points[4];

uniform mat4 m, v, p;
uniform highp vec4 point_light;

void
main (void) {
	mat4 vp = p * v;
	highp vec4 world_position = m * vec4 (vertex_position, 1.0);
	highp vec4 world_normal = m * vec4 (vertex_normal, 1.0);

	points[0] = point_light;
	points[1] = vec4 ( 2.5,   0,   0, 1);
	points[2] = vec4 (   0,   0, 2.5, 1);
	points[3] = vec4 (-2.5,   0,   0, 1);

	for (int i = 0; i < 4; ++i) {
		point_light_directions[i] = world_position.xyz - points[i].xyz;
	}

	fragment_uv = vertex_uv;
	fragment_normal = world_normal.xyz;

	gl_Position = vp * world_position;
}
