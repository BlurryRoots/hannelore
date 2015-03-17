#version 120

attribute vec3 vertex_position;

uniform vec4 vertex_color;

varying vec4 fragment_color;

void
main (void) {
    gl_Position = vec4 (vertex_position, 1.0f);

    fragment_color = vertex_color;
}
