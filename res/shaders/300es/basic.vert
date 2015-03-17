#version 330 core

// Uniform data: Arbitrary data send from c++ side
uniform float angle;

uniform mat4 model_matrix;

uniform mat4 camera_matrix;
uniform float aspect;
uniform float fov;

uniform vec3 ambient_light_color;

// Layout data: Buffer data specified via vertex attribute function
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_uv_position;
layout (location = 3) in vec3 vertex_normal;

// Output data ; will be interpolated for each fragment.
out vec4 fragment_color;
out vec2 fragment_uv_position;
out vec3 fragment_normal;

//
mat4 view_frustum (
    float angle_of_view,
    float aspect_ratio,
    float z_near,
    float z_far
) {
    float a = 1.0 / tan (angle_of_view);
    float b = aspect_ratio / tan (angle_of_view);
    float c = (z_far + z_near) / (z_far - z_near);
    float d = -2.0 * z_far * z_near / (z_far - z_near);

    return mat4 (
        vec4 (  a, 0.0, 0.0, 0.0),
        vec4 (0.0,   b, 0.0, 0.0),
        vec4 (0.0, 0.0,   c, 1.0),
        vec4 (0.0, 0.0,   d, 0.0)
    );
}

mat4
scale (float x, float y, float z) {
    return mat4 (
        vec4 (x,   0.0, 0.0, 0.0),
        vec4 (0.0, y,   0.0, 0.0),
        vec4 (0.0, 0.0, z,   0.0),
        vec4 (0.0, 0.0, 0.0, 1.0)
    );
}

mat4
translate (float x, float y, float z) {
    return mat4 (
        vec4 (1.0, 0.0, 0.0, 0.0),
        vec4 (0.0, 1.0, 0.0, 0.0),
        vec4 (0.0, 0.0, 1.0, 0.0),
        vec4 (x,   y,   z,   1.0)
    );
}

mat4
rotate_x (float theta) {
    return mat4 (
        vec4 (1.0,          0.0,          0.0, 0.0),
        vec4 (0.0,  cos (theta),  sin (theta), 0.0),
        vec4 (0.0, -sin (theta),  cos (theta), 0.0),
        vec4 (0.0,          0.0,          0.0, 1.0)
    );
}

// Shader entry point
void main () {
    // Calculate the view projection
    mat4 view_projection = mat4 (1)
        * view_frustum (radians (fov), aspect, 0.05, 100.0)
        * camera_matrix
        ;

    // Combine model space and view projection
    mat4 model_view_projection = mat4 (1)
        * view_projection
        * model_matrix
        ;

    // Apply transformation to vectex position
    gl_Position = mat4 (1)
        * model_view_projection
        * vec4 (vertex_position, 1.0f)
        ;

    // Export the vertex color
    fragment_color = vec4 (vertex_color, 1.0f);

    // Export the vertex uv position
    fragment_uv_position = vertex_uv_position;

    // Calculate the vertex normal and export it
    fragment_normal = (mat4 (1)
        // transform the normal according to mvp
        * model_view_projection
        // set the vector to be a direction
        * vec4 (vertex_normal, 0.0f)).xyz
        ;
}
