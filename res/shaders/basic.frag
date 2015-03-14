#version 330 core

// Constants
vec4 LIGHT = vec4 (0, 0, -1, 0);

// Values coming in from vertex shader
in vec2 fragment_uv_position;
in vec4 fragment_color;
in vec4 fragment_normal;

// Values that stay constant for the whole mesh.
uniform sampler2D texture_sampler;

//
out vec4 pixel_color;

// Calculate lighting value via lampert reflectance
float
calculate_lighting_value (vec4 light_direction, vec4 normal) {
	return clamp (dot (-light_direction, fragment_normal), 0.0f, 1.0f);
}

void main () {
    // Sample the texture with the given uv position and extract the color
    vec4 sample_color = texture (texture_sampler, fragment_uv_position).rgba;


   	// Calculate the actual color value from the fragments color, sampled
   	// via the texture of the surface and its light value
    pixel_color = sample_color
    	* calculate_lighting_value (LIGHT, fragment_normal)
    	;
}
