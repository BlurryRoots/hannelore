#version 330 core

uniform float angle;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

out vec4 fragment_color;

void main () {
    gl_Position = vec4 (vertex_position, 1.0f);

    fragment_color = vec4 (vertex_color, 1.0f);
}