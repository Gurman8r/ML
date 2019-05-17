// Vert.MVP.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <common/Vert.shader>
#shader vertex

/* * * * * * * * * * * * * * * * * * * * */

struct Vert_Uniforms
{
	mat4 proj;
	mat4 view;
	mat4 model;
};
uniform Vert_Uniforms Vert;

/* * * * * * * * * * * * * * * * * * * * */

mat4 ml_MVP()
{
	return (Vert.proj * Vert.view * Vert.model);
}

vec4 ml_MVP_Position()
{
	ml_Update_Vertex();

	return ml_MVP() * vec4(Out.Position, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * */