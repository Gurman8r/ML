#shader vertex
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 460 core

// In Attributes
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_normal;
layout(location = 2) in vec2 a_texcoord;

// Out Vertex
out Vertex { 
	vec3 position; vec4 normal; vec2 texcoord; 
} V;

// Uniforms
uniform mat4 u_proj; // Projection Matrix
uniform mat4 u_view; // View Matrix
uniform mat4 u_model; // Model Matrix

// Main
void main()
{
	V.position	= a_position;
	V.normal	= a_normal;
	V.texcoord	= a_texcoord;
	gl_Position = (u_proj * u_view * u_model) * vec4(V.position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#shader fragment
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#version 460 core

// In Vertex
in Vertex { 
	vec3 position; vec4 normal; vec2 texcoord; 
} V;

// Out Color
out vec4 gl_Color;

// Uniforms
uniform	vec4 u_color; // Main Color
uniform sampler2D u_texture0; // Main Texture

// Main
void main()
{
	gl_Color = u_color * texture(u_texture0, V.texcoord);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */