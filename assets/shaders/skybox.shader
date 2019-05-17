// skybox.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Vert.MVP.shader>
#shader vertex

void main()
{
	ml_Update_Vertex();

	gl_Position = (Vert.proj * Vert.view) * vec4(Out.Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Frag.Draw.shader>
#shader fragment

//struct Frag_Uniforms
//{
//	samplerCube	mainTex;
//	vec4 mainCol;
//};
//uniform Frag_Uniforms Frag;

void main()
{
	gl_Color = Frag.mainCol * texture(Frag.mainTex, In.Texcoord);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */