#version 300 es
//#version 100

// Uniform data: Arbitrary data send from c++ side
uniform mediump float angle;

uniform mat4 model_matrix;

uniform mediump mat4 camera_matrix;
uniform mediump float aspect;
uniform mediump float fov;

// Layout data: Buffer data specified via vertex attribute function
in mediump vec3 vertex_position;
in mediump vec3 vertex_color;
in mediump vec2 vertex_uv_position;
in mediump vec3 vertex_normal;

// Output data ; will be interpolated for each fragment.
out mediump vec4 fragment_color;
out mediump vec2 fragment_uv_position;
out mediump vec3 fragment_normal;

//
mediump mat4 view_frustum (
    mediump float angle_of_view,
    mediump float aspect_ratio,
    mediump float z_near,
    mediump float z_far
) {
    mediump float a = 1.0 / tan (angle_of_view);
    mediump float b = aspect_ratio / tan (angle_of_view);
    mediump float c = (z_far + z_near) / (z_far - z_near);
    mediump float d = -2.0 * z_far * z_near / (z_far - z_near);

    return mat4 (
        vec4 (  a, 0.0, 0.0, 0.0),
        vec4 (0.0,   b, 0.0, 0.0),
        vec4 (0.0, 0.0,   c, 1.0),
        vec4 (0.0, 0.0,   d, 0.0)
    );
}

mediump mat4
scale (mediump float x, mediump float y, mediump float z) {
    return mat4 (
        vec4 (x,   0.0, 0.0, 0.0),
        vec4 (0.0, y,   0.0, 0.0),
        vec4 (0.0, 0.0, z,   0.0),
        vec4 (0.0, 0.0, 0.0, 1.0)
    );
}

mediump mat4
translate (mediump float x, mediump float y, mediump float z) {
    return mat4 (
        vec4 (1.0, 0.0, 0.0, 0.0),
        vec4 (0.0, 1.0, 0.0, 0.0),
        vec4 (0.0, 0.0, 1.0, 0.0),
        vec4 (x,   y,   z,   1.0)
    );
}

mediump mat4
rotate_x (mediump float theta) {
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
    mediump mat4 view_projection = mat4 (1)
        * view_frustum (radians (fov), aspect, 0.05, 100.0)
        * camera_matrix
        ;

    // Combine model space and view projection
    mediump mat4 model_view_projection = mat4 (1)
        * view_projection
        * model_matrix
        ;

    // Apply transformation to vectex position
    gl_Position = mat4 (1)
        * model_view_projection
        //* view_projection * model_matrix
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
        //* view_projection * model_matrix
        // set the vector to be a direction
        * vec4 (vertex_normal, 0.0f)).xyz
        ;
}
