#version 330 core

in vec2 fragment_uv_position;
in vec4 fragment_color;

// Values that stay constant for the whole mesh.
uniform sampler2D texture_sampler;

//
out vec4 color;

void main () {
    color = fragment_color;
    //color = texture (texture_sampler, fragment_uv_position);
}
