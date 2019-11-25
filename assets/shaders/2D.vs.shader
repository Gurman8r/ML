#shader vertex
#version 460 core

// Input
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texcoord;

// Output
out Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

// Uniforms
uniform mat4 u_proj;  // Projection Matrix
uniform mat4 u_view;  // View Matrix
uniform mat4 u_model; // Model Matrix

// Main
void main()
{
    V.position  = a_position;
    V.normal    = a_normal;
    V.texcoord  = a_texcoord;
    gl_Position = (u_proj * u_view * u_model) * vec4(V.position, 1.0);
}