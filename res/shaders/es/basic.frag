#version 120
#extension GL_ARB_gpu_shader5 : enable

// make transformations available
uniform mat4 m, v, p;

// flag to color regions in need of debugging
uniform int colorized_debug;

uniform sampler2D texture_sampler;
uniform vec3 LIGHT0;
uniform float LIGHT0_intensity;

varying vec2 fragment_uv;
varying vec3 fragment_normal;
varying vec3 fragment_pointing_to_light0;

//vec3 LIGHT0 = vec3 (0, -0.5, 1);

// Calculate lighting value via lampert reflectance
float
calculate_lighting_value (vec3 light_direction, vec3 normal) {
	return clamp (dot (-light_direction, normal), 0.0f, 1.0f);
}

void
main (void) {
	float shade = calculate_lighting_value (LIGHT0, fragment_normal) * LIGHT0_intensity;
	vec4 sample_color = texture2D (texture_sampler, fragment_uv) * shade;
	gl_FragColor = vec4 (sample_color.rgb * shade, sample_color.a);
	//if (0.01f > shade && 1 == colorized_debug) {
	//	gl_FragColor = vec4 (1, 0, 0, 1);
	//}
	//else {
	//	gl_FragColor = vec4 (sample_color.rgb * shade, sample_color.a);
	//}
}
