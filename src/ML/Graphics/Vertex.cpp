#include <ML/Graphics/Vertex.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Vertex::Vertex()
	{
		this->position(vec3::Zero);
		this->color(Color::White);
		this->texcoords(vec2::Zero);
	}

	Vertex::Vertex(const vec3 & position)
	{
		this->position(position);
		this->color(Color::White);
		this->texcoords(vec2::Zero);
	}

	Vertex::Vertex(const vec3 & position, const vec4 & color)
	{
		this->position(position);
		this->color(color);
		this->texcoords(vec2::Zero);
	}

	Vertex::Vertex(const vec3 & position, const vec2 & texcoords)
	{
		this->position(position);
		this->color(Color::White);
		this->texcoords(texcoords);
	}

	Vertex::Vertex(const vec3 & position, const vec4 & color, const vec2 & texcoords)
	{
		this->position(position);
		this->color(color);
		this->texcoords(texcoords);
	}

	Vertex::Vertex(const Vertex & copy)
	{
		this->position(copy.position());
		this->color(copy.color());
		this->texcoords(copy.texcoords());
	}

	Vertex::~Vertex()
	{
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
	
	const vec3 & Vertex::position() const
	{
		static vec3 temp;
		temp[0] = (*this)[0];
		temp[1] = (*this)[1];
		temp[2] = (*this)[2];
		return temp;
	}
	
	const vec4 & Vertex::color() const
	{
		static vec4 temp;
		temp[0] = (*this)[3];
		temp[1] = (*this)[4];
		temp[2] = (*this)[5];
		temp[3] = (*this)[6];
		return temp;
	}
	
	const vec2 & Vertex::texcoords() const
	{
		static vec2 temp;
		temp[0] = (*this)[7];
		temp[1] = (*this)[8];
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Vertex & Vertex::position(const vec3 & value)
	{
		return position(
			value[0],
			value[1], 
			value[2]);
	}
	
	Vertex & Vertex::color(const vec4 & value)
	{
		return color(
			value[0],
			value[1], 
			value[2], 
			value[3]);
	}
	
	Vertex & Vertex::texcoords(const vec2 & value)
	{
		return texcoords(
			value[0],
			value[1]);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Vertex & Vertex::position(const float x, const float y, const float z)
	{
		(*this)[0] = x;
		(*this)[1] = y;
		(*this)[2] = z;
		return (*this);
	}

	Vertex & Vertex::color(const float r, const float g, const float b, const float a)
	{
		(*this)[3] = r;
		(*this)[4] = g;
		(*this)[5] = b;
		(*this)[6] = a;
		return (*this);
	}

	Vertex & Vertex::texcoords(const float x, const float y)
	{
		(*this)[7] = x;
		(*this)[8] = y;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}