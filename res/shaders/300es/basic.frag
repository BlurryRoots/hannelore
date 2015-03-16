#version 300 es
//#version 100

//highp - 16-bit, floating point range: -2^62 to 2^62, integer range: -2^16 to 2^16
//mediump - 10 bit, floating point range: -2^14 to 2^14, integer range: -2^10 to 2^10
//lowp - 8 bit, floating point range: -2 to 2, integer range: -2^8 to 2^8

// Ambient lights
//in vec3 fragment_ambient_light;

// Values coming in from vertex shader
in mediump vec2 fragment_uv_position;
in mediump vec4 fragment_color;
in mediump vec3 fragment_normal;

// Values that stay constant for the whole mesh.
uniform sampler2D texture_sampler;

mediump vec3 LIGHT = vec3 (0, 0, -1);

//
out mediump vec4 pixel_color;

// Calculate lighting value via lampert reflectance
mediump float
calculate_lighting_value (mediump vec3 light_direction, mediump vec3 normal) {
	return clamp (dot (-light_direction, fragment_normal), 0.0f, 1.0f);
}

void main () {
    // Sample the texture with the given uv position and extract the color
    mediump vec4 sample_color = texture2D (texture_sampler, fragment_uv_position).rgba;

   	// Calculate the actual color value from the fragments color, sampled
   	// via the texture of the surface and its light value
    pixel_color = sample_color * fragment_color
    	* calculate_lighting_value (LIGHT, fragment_normal)
    	;
}
