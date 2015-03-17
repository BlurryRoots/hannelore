#version 120

varying vec2 fragment_uv;
varying vec4 fragment_color;

uniform sampler2D texture_sampler;

void
main (void) {
	vec4 sample_color = texture2D (texture_sampler, fragment_uv).rgba;
	gl_FragColor = fragment_color * sample_color;
	gl_FragColor.a = 1.0f;
}
