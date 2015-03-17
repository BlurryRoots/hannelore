#version 120

#ifdef FRAGMENT_PRECISION_HIGH
//precision highp float;
//precision highp vec3;
//precision highp vec4;
//precision highp mat4;
#else
//precision mediump float;
//precision mediump vec3;
//precision mediump vec4;
//precision mediump mat4;
#endif

varying vec2 fragment_uv;
varying vec4 fragment_color;

uniform sampler2D texture_sampler;

void
main (void) {
	vec4 sample_color = texture2D (texture_sampler, fragment_uv).rgba;
	gl_FragColor = vec4 (
		clamp (fragment_color.r + sample_color.r, 0.0f, 1.0f),
		clamp (fragment_color.g + sample_color.g, 0.0f, 1.0f),
		clamp (fragment_color.b + sample_color.b, 0.0f, 1.0f),
		fragment_color.r
	);
}
