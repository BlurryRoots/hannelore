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

// Calculate lighting value via lampert reflectance
float
calculate_lighting_value (vec3 light_direction, vec3 normal) {
	return dot (-1.0f * light_direction, normal);
}

void
main (void) {
	vec4 sample_color = texture2D (texture_sampler, fragment_uv);

	float attinuation = LIGHT0_intensity / length (fragment_pointing_to_light0);
	attinuation = clamp (attinuation, 0.0f, 1.0f);

	float shade = calculate_lighting_value (
		normalize (fragment_pointing_to_light0),
		normalize (fragment_normal)
	);
	shade = clamp (shade, 0.0f, 1.0f);

	float brightness = attinuation * shade;
	//float brightness = shade * LIGHT0_intensity;
	brightness = clamp (brightness, 0.0f, 1.0f);

	if (0.2f > brightness) {
		brightness = 0.0f;
	}

	vec3 diffuse_color = sample_color.rgb * brightness;

	gl_FragColor = vec4 (diffuse_color, sample_color.a);
}
