#version 300 es

in highp vec3 vertex_position;
in highp vec2 vertex_uv;
in highp vec3 vertex_normal;

out highp vec2 fragment_uv;
out highp vec3 fragment_normal;
out highp vec3 point_light_direction;

uniform mat4 m, v, p;
uniform highp vec4 point_light;

void
main (void) {
	mat4 vp = p * v;
	highp vec4 world_position = m * vec4 (vertex_position, 1.0);

	point_light_direction = point_light.xyz - world_position.xyz;

	fragment_uv = vec2 (vertex_uv.x, 1.0 - vertex_uv.y);
	fragment_normal = (m * vec4 (vertex_normal, 0.0)).xyz;

	gl_Position = vp * world_position;
}
