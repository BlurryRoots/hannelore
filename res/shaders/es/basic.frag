#version 120

varying vec2 fragment_uv;

uniform sampler2D texture_sampler;

void
main (void) {
	vec4 sample_color = texture2D (texture_sampler, fragment_uv).rgba;

	gl_FragColor = sample_color;
}
