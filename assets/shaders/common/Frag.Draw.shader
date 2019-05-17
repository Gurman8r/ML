// Frag.Draw.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Frag.shader>
#shader fragment

/* * * * * * * * * * * * * * * * * * * * */

struct Frag_Uniforms
{
	vec4 mainCol;
	sampler2D mainTex;
};
uniform Frag_Uniforms Frag;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */