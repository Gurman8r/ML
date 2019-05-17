#ifndef _ML_CANVAS_HPP_
#define _ML_CANVAS_HPP_

#include <ML/Graphics/VertexArray.hpp>
#include <ML/Graphics/VertexBuffer.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Used to store and create a VAO and VBO for 2D rendering
	class ML_GRAPHICS_API Canvas final
		: public ITrackable
		, public INonCopyable
	{
	public:
		Canvas();
		~Canvas();

	public:
		bool create();

	public:
		VAO & vao() { return m_vao; }
		VBO & vbo() { return m_vbo; }

	private:
		mutable VAO m_vao;
		mutable VBO m_vbo;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CANVAS_HPP_