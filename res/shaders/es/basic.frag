#version 120
#extension GL_ARB_gpu_shader5 : enable

varying vec2 fragment_uv;
varying vec3 fragment_normal;
//varying vec3 point_light_direction;
varying vec3 point_light_directions[4];
varying vec4 points[4];

// make transformations available
uniform mat4 m, v, p;

// flag to color regions in need of debugging
uniform int colorized_debug;

uniform sampler2D texture_sampler;
uniform vec4 point_light;

//
uniform vec3 ambient_light;

// Calculate lighting value via lampert reflectance
float
calculate_lighting_value (vec3 light_direction, vec3 normal) {
	return dot (-1.0f * light_direction, normal);
}

float
calculate_brightness (vec4 light, vec3 distance) {
	float d = clamp (length (distance), 0.0f, 1.0f);
	float attinuation = light.w * (1.0f - d);
	attinuation = clamp (attinuation, 0.0f, 1.0f);

	float shade = calculate_lighting_value (
		distance,
		normalize (fragment_normal)
	);
	shade = clamp (shade, 0.0f, 1.0f);

	float brightness = attinuation * shade;
	//float brightness = shade * LIGHT0_intensity;
	brightness = clamp (brightness, 0.0f, 1.0f);

	if (0.01f > brightness) {
		brightness = 0.0f;
	}

	return brightness;
}

void
main (void) {
	vec4 sample_color = texture2D (texture_sampler, fragment_uv);

	float brightness = 0;
	for (int i = 0; i < 4; ++i) {
		brightness+= clamp (calculate_brightness (
			points[i], point_light_directions[i]
		), 0.0f, 1.0f);
	}

	brightness = clamp (brightness, 0.0f, 1.0f);

	if (0.01f > brightness) {
		brightness = 0.0f;
	}

	vec3 diffuse_color = sample_color.rgb * brightness + sample_color.rgb * ambient_light;

	gl_FragColor = vec4 (diffuse_color, sample_color.a);
}
