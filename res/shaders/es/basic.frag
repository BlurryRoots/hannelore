#version 300 es

#extension GL_ARB_gpu_shader5 : enable

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
uniform int complex_attenuation;

//
out highp vec4 pixel_color;

highp float
cut_value_if_below (highp float threshold, highp float value) {
	// scale and bias value such that:
    //   value == 0 at extent of max influence
    //   value == 1 when d == 0
    value = (value - threshold) / (1.0 - threshold);
    // (0.06 - 0.05) / (1 - 0.05) yields ~ 0.01 (value left)
    value = max (value, 0.0);

    return value;
}

highp float
calculate_attenuation_simple (
	highp float intensity,
	highp float radius,
	highp float distance,
	highp float cutoff
) {
	highp float coefficient = ((distance / radius) + 1.0);
	highp float attenuation = intensity / (coefficient * coefficient);

	return cut_value_if_below (cutoff, attenuation);
}

highp float
calculate_attenuation (
	highp float intensity,
	highp float radius,
	highp float distance,
	highp float cutoff
) {
	highp float linear = (2.0 / radius) * distance;
	highp float quadratic = (1.0 / (radius * radius)) * (distance * distance);
	highp float attenuation = intensity / (1.0 + linear + quadratic);

	return cut_value_if_below (cutoff, attenuation);
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
highp float CUTOFF = 0.05;

void
main (void) {
	// sample the color of the current texture on the given uv coordinate
	highp vec4 sample_color = texture (texture_sampler, fragment_uv);

	// calculate the brightness of the current pixel dependend on distance
	// and angle of incidence to the light
	highp float brightness = calculate_brightness (
		point_light, point_light_direction
	);

	// calculate the attenuation (falloff) of the light
	highp float attenuation;
	if (0 < complex_attenuation) {
		attenuation = calculate_attenuation (
			point_light_color.a, // intensity
			point_light.w, // radius
			length (point_light_direction),
			CUTOFF // cutoff
		);
	}
	else {
		attenuation = calculate_attenuation_simple (
			point_light_color.a, // intensity
			point_light.w, // radius
			length (point_light_direction),
			CUTOFF // cutoff
		);
	}

	highp vec3 ambient_color = sample_color.rgb * ambient_light;
	highp vec3 diffuse_color = vec3 (1)
		* brightness * attenuation
		* point_light_color.rgb
		* sample_color.rgb
		;
	highp vec3 resulting_color = diffuse_color + ambient_color;
	resulting_color.r = clamp (resulting_color.r, 0.0, 1.0);
	resulting_color.g = clamp (resulting_color.g, 0.0, 1.0);
	resulting_color.b = clamp (resulting_color.b, 0.0, 1.0);

	pixel_color = vec4 (resulting_color, 1.0);
}
