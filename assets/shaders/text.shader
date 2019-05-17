// text.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Vert.MVP.shader>
#shader vertex

void main()
{
	ml_Update_Vertex();

	gl_Position = Vert.proj * vec4(Out.Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Frag.Draw.shader>
#shader fragment

void main()
{
	gl_Color = Frag.mainCol * vec4(1, 1, 1, texture(Frag.mainTex, In.Texcoord).r);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */