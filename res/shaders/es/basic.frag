//#version 120

// first define version of opengl es 1.0 and update to opengl es 2.0 (WTF?)
#version 100
#define GLES2

//#version 300

#extension GL_ARB_gpu_shader5 : enable

#ifndef mediump
	#define mediump lowp
#endif

varying highp vec2 fragment_uv;
varying highp vec3 fragment_normal;
//varying vec3 point_light_direction;
varying highp vec3 point_light_directions[4];
varying highp vec4 points[4];

// make transformations available
uniform highp mat4 m, v, p;

// flag to color regions in need of debugging
uniform int colorized_debug;

uniform sampler2D texture_sampler;
uniform highp vec4 point_light;

//
uniform highp vec3 ambient_light;

// Calculate lighting value via lampert reflectance
highp float
calculate_lighting_value (highp vec3 light_direction, highp vec3 normal) {
	return dot (-1.0 * light_direction, normal);
}

highp float
calculate_brightness (highp vec4 light, highp vec3 distance) {
	highp float d = clamp (length (distance), 0.0, 1.0);
	highp float attinuation = light.w * (1.0 - d);
	//attinuation = clamp (attinuation, 0.0, 1.0);

	highp float shade = calculate_lighting_value (
		distance,
		normalize (fragment_normal)
	);
	//shade = clamp (shade, 0.0, 1.0);

	highp float brightness = attinuation * shade;
	//float brightness = shade * LIGHT0_intensity;
	//brightness = clamp (brightness, 0.0, 1.0);

	//if (0.01 > brightness) {
	//	brightness = 0.0;
	//}

	return brightness;
}

void
main (void) {
	//highp vec4 sample_color = texture2D (texture_sampler, fragment_uv);
	highp vec4 sample_color = texture2D (texture_sampler, fragment_uv);

	highp float brightness = 0;
	for (int i = 0; i < 4; ++i) {
		brightness+= clamp (calculate_brightness (
			points[i], point_light_directions[i]
		), 0.0, 1.0);
	}

	//brightness = clamp (brightness, 0.0, 1.0);

	//if (0.01f > brightness) {
	//	brightness = 0.0f;
	//}

	highp vec3 ambient_color = sample_color.rgb * ambient_light;
	highp vec3 diffuse_color = sample_color.rgb * brightness;

	highp float l = length (sample_color);
	if (0.1 < l) {
		diffuse_color.r = 1;
	}

	//diffuse_color.r += sample_color.r;
	//diffuse_color.g += sample_color.g;
	//diffuse_color.b += sample_color.b;
	ambient_color *= 0.001;

	gl_FragColor = vec4 (diffuse_color + ambient_color, 1.0);
}
