#shader fragment
#version 460 core

// Input
in Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

// Output
out vec4 gl_Color;

// Uniforms
uniform vec4 u_color; // Main Color
uniform sampler2D u_texture0; // Main Texture

// Main
void main()
{
    gl_Color = u_color * texture(u_texture0, V.texcoord);
}