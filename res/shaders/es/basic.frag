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

uniform sampler2D texture_sampler;

void
main (void) {
	gl_FragColor = texture2D (texture_sampler, fragment_uv);
}
