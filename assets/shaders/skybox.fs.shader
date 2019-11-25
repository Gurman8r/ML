#shader fragment
#version 460 core

in Vertex { vec3 position; vec3 normal; vec2 texcoord; } V;

out vec4 gl_Color;

uniform vec4 u_color;
uniform samplerCube u_texture0;

void main()
{
    gl_Color = u_color * texture(u_texture0, V.position);
}