#version 120

varying vec2 fragment_uv;
varying vec4 fragment_color;

uniform sampler2D texture_sampler;

void
main (void) {
	vec2 flipped_uv = vec2 (fragment_uv.x, 1.0 - fragment_uv.y);
	vec4 sample_color = texture2D (texture_sampler, flipped_uv).rgba;

	gl_FragColor = fragment_color * sample_color;
	gl_FragColor.a = 1.0f;
}
