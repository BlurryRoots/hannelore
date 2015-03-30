#version 300 es

#extension GL_ARB_gpu_shader5 : enable

//out highp vec4 fragment_position;

in highp vec2 fragment_uv;
in highp vec3 fragment_normal;
in highp vec3 point_light_direction;

// make transformations available
uniform highp mat4 m, v, p;

// flag to color regions in need of debugging
uniform int colorized_debug;

uniform sampler2D texture_sampler;
uniform highp vec4 point_light; // position + radius
uniform highp vec4 point_light_color; // color + intensity

//
uniform highp vec3 ambient_light;

//
out highp vec4 pixel_color;

highp float
calculate_attenuation (highp float intensity, highp float radius, highp float distance, highp float cutoff) {
	//highp float linear = (2.0 / radius) * distance;
	//highp float quadratic = (1.0 / (radius * radius)) * (distance * distance);
	//highp float attenuation = intensity / (1.0 + linear + quadratic);

	highp float coefficient = ((distance / radius) + 1);
	highp float attenuation = intensity / (coefficient * coefficient);

	// scale and bias attenuation such that:
    //   attenuation == 0 at extent of max influence
    //   attenuation == 1 when d == 0
    attenuation = (attenuation - cutoff) / (1.0 - cutoff);
    // (0.06 - 0.05 / 1 - 0.05) ~ 0.01 (att left)
    attenuation = max (attenuation, 0.0);

	return attenuation;
}

highp float
calculate_brightness (highp vec4 light, highp vec3 distance_vector) {
	highp vec3 light_direction = normalize (distance_vector);
	highp float angle_of_incidence = dot (
		fragment_normal, light_direction
	);

	return angle_of_incidence;
}

highp float EPSI = 0.000001;

void
main (void) {
	highp vec4 sample_color = texture (texture_sampler, fragment_uv);

	highp float brightness = calculate_brightness (
		point_light, point_light_direction
	);

	highp float attenuation = calculate_attenuation (
		point_light_color.a, // intensity
		point_light.w, // radius
		length (point_light_direction),
		0.08 // cutoff
	);

	if (EPSI > attenuation) {
		brightness = 0.0;
	}

	highp vec3 ambient_color = sample_color.rgb * ambient_light;
	highp vec3 diffuse_color =
		brightness * attenuation * point_light_color.rgb * sample_color.rgb;

	pixel_color = vec4 (diffuse_color + ambient_color, 1.0);
}
