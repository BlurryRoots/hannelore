#version 120

varying vec2 fragment_uv;
varying vec3 fragment_normal;
varying vec4 fragment_color;

uniform sampler2D texture_sampler;

// Calculate lighting value via lampert reflectance
float
calculate_lighting_value (vec3 light_direction, vec3 normal) {
	return clamp (dot (-light_direction, fragment_normal), 0.0f, 1.0f);
}

void
main (void) {
	vec4 sample_color = texture2D (texture_sampler, fragment_uv);

	gl_FragColor = sample_color;
}
