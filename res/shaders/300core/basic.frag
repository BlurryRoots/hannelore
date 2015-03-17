#version 330 core

// Ambient lights
//in vec3 fragment_ambient_light;

// Values coming in from vertex shader
in vec2 fragment_uv_position;
in vec4 fragment_color;
in vec3 fragment_normal;

// Values that stay constant for the whole mesh.
uniform sampler2D texture_sampler;

vec3 LIGHT = vec3 (0, 0, -1);

//
out vec4 pixel_color;

// Calculate lighting value via lampert reflectance
float
calculate_lighting_value (vec3 light_direction, vec3 normal) {
	return clamp (dot (-light_direction, fragment_normal), 0.0f, 1.0f);
}

void main () {
    // Sample the texture with the given uv position and extract the color
    vec4 sample_color = texture (texture_sampler, fragment_uv_position).rgba;

   	// Calculate the actual color value from the fragments color, sampled
   	// via the texture of the surface and its light value
    pixel_color = sample_color * fragment_color
    	* calculate_lighting_value (LIGHT, fragment_normal)
    	;
}
