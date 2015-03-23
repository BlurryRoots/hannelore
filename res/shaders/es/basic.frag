//#version 120

// first define version of opengl es 1.0 and update to opengl es 2.0 (WTF?)
//#version 100
//#define GLES2

#version 300 es

#extension GL_ARB_gpu_shader5 : enable

//out highp vec4 fragment_position;

in highp vec2 fragment_uv;
in highp vec3 fragment_normal;
in highp vec3 point_light_directions[4];
in highp vec4 points[4];

// make transformations available
uniform highp mat4 m, v, p;

// flag to color regions in need of debugging
uniform int colorized_debug;

uniform sampler2D texture_sampler;
uniform highp vec4 point_light;

//
uniform highp vec3 ambient_light;

//
out highp vec4 pixel_color;

// Calculate lighting value via lampert reflectance
highp float
calculate_lighting_value (highp vec3 light_direction, highp vec3 normal) {
	return dot (-1.0 * light_direction, normal);
}

highp float
calculate_brightness (highp vec4 light, highp vec3 distance) {
	highp float d = clamp (length (distance), 0.0, 1.0);
	highp float attinuation = light.w * (1.0 - d);
	attinuation = clamp (attinuation, 0.0, 1.0);

	highp float shade = calculate_lighting_value (
		distance,
		normalize (fragment_normal)
	);
	shade = clamp (shade, 0.0, 1.0);

	highp float brightness = attinuation * shade;
	brightness = clamp (brightness, 0.0, 1.0);

	return brightness;
}

void
main (void) {
	highp vec4 sample_color = texture (texture_sampler, fragment_uv);

	highp float brightness = 0.0;
	for (int i = 0; i < 4; ++i) {
		brightness+= clamp (calculate_brightness (
			points[i], point_light_directions[i]
		), 0.0, 1.0);
	}
	brightness = clamp (brightness, 0.0, 1.0);

	highp vec3 ambient_color = sample_color.rgb * ambient_light;
	highp vec3 diffuse_color = sample_color.rgb * brightness;

	pixel_color = vec4 (diffuse_color + ambient_color, 1.0);
}
