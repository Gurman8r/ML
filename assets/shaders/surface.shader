// framebuffer.shader
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Vert.shader"

void main()
{
	V.Position	= a_Position;
	V.Normal	= a_Normal;
	V.Texcoord	= a_Texcoord;
	gl_Position	= vec4(V.Position.x, V.Position.y, 0.0, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "../../../assets/shaders/common/Frag.shader"

/* * * * * * * * * * * * * * * * * * * * */

uniform vec4		u_color;
uniform sampler2D	u_texture0;
uniform int			u_effectMode;
uniform mat3		u_kernel;

/* * * * * * * * * * * * * * * * * * * * */

#define MODE_DEFAULT		0
#define MODE_GRAYSCALE		1
#define MODE_BLUR			2
#define MODE_JUICY			3
#define MODE_INVERTED		4
#define MODE_CUSTOM_KERNEL	5

/* * * * * * * * * * * * * * * * * * * * */

vec4 drawKernel(in mat3 kernel)
{
	const float offset = 1.0 / 300.0;

	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),	// top-left
		vec2(0.0f, offset),		// top-center
		vec2(offset, offset),	// top-right
		vec2(-offset, 0.0f),	// center-left
		vec2(0.0f, 0.0f),		// center-center
		vec2(offset, 0.0f),		// center-right
		vec2(-offset, -offset),	// bottom-left
		vec2(0.0f, -offset),	// bottom-center
		vec2(offset, -offset)	// bottom-right    
	);

	vec3 samples[9];
	for (int i = 0; i < 9; i++)
	{
		samples[i] = vec3(texture(u_texture0, V.Texcoord.st + offsets[i]));
	}

	vec3 color = vec3(0.0);
	for (int i = 0; i < 9; i++)
	{
		color += samples[i] * kernel[i / 3][i % 3];
	}

	return vec4(color, 1.0);
}

/* * * * * * * * * * * * * * * * * * * * */

void main()
{
	switch (u_effectMode)
	{
	case MODE_GRAYSCALE:
		gl_Color = texture(u_texture0, V.Texcoord);
		float average = (gl_Color.r + gl_Color.g + gl_Color.b) / 3.0;
		gl_Color = vec4(average, average, average, 1.0);
		break;

	case MODE_JUICY:
		gl_Color = drawKernel(mat3(
			-1, -1, -1,
			-1, +9, -1,
			-1, -1, -1
		));
		break;

	case MODE_BLUR:
		gl_Color = drawKernel(mat3(
			1.0 / 16, 2.0 / 16, 1.0 / 16,
			2.0 / 16, 4.0 / 16, 2.0 / 16,
			1.0 / 16, 2.0 / 16, 1.0 / 16
		));
		break;

	case MODE_INVERTED:
		gl_Color = vec4(vec3(1.0 - texture(u_texture0, V.Texcoord)), 1.0);
		break;

	case MODE_CUSTOM_KERNEL:
		gl_Color = drawKernel(u_kernel);
		break;

	case MODE_DEFAULT:
	default:
		gl_Color = texture(u_texture0, V.Texcoord);
		break;
	};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */