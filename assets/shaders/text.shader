// text.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Vert.shader"

uniform mat4 u_proj;
uniform mat4 u_view;
uniform mat4 u_model;

void main()
{
	V.Position	= a_Position;
	V.Normal		= a_Normal;
	V.Texcoord	= a_Texcoord;
	gl_Position		= u_proj * vec4(V.Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Frag.shader"

uniform vec4 u_mainColor;
uniform sampler2D u_mainTexture;

void main()
{
	gl_Color = u_mainColor * vec4(1, 1, 1, texture(u_mainTexture, V.Texcoord).r);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */