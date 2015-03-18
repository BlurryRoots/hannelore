#version 120

varying vec2 fragment_uv;
varying vec3 fragment_normal;

uniform sampler2D texture_sampler;

vec3 LIGHT0 = vec3 (0, 0, 1);

// Calculate lighting value via lampert reflectance
float
calculate_lighting_value (vec3 light_direction, vec3 normal) {
	return clamp (dot (-light_direction, normal), 0.0f, 1.0f);
}

void
main (void) {
	vec4 sample_color = texture2D (texture_sampler, fragment_uv);
	float shade = calculate_lighting_value (LIGHT0, fragment_normal);
	//float shade = 0.4f;

	if (0.01f > shade) {
		gl_FragColor = vec4 (1, 0, 0, 1);
	}
	else {
		gl_FragColor = sample_color * shade;
	}
}
