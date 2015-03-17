#version 120

attribute vec3 vertex_position;

void
main (void) {
    gl_Position = vec4 (vertex_position, 1.0f);
}
