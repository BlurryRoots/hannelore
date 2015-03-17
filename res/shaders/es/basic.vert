#version 120

attribute vec3 vertex_position;
attribute vec2 vertex_uv;
attribute vec4 vertex_color;

varying vec2 fragment_uv;
varying vec4 fragment_color;

uniform mat4 mvp;

void
main (void) {
    gl_Position = mvp * vec4 (vertex_position, 1.0f);

    fragment_uv = vertex_uv;
    fragment_color = vertex_color;
}
