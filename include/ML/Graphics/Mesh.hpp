#ifndef _ML_MESH_HPP_
#define _ML_MESH_HPP_

#include <ML/Core/IReadable.hpp>
#include <ML/Core/IDisposable.hpp>
#include <ML/Graphics/VertexList.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Mesh final
		: public ITrackable
		, public IDisposable
		, public IReadable
	{
	public:
		Mesh();
		Mesh(const VertexList & vertices);
		Mesh(const VertexList & vertices, const List<uint32_t> & indices);
		Mesh(const Mesh & copy);
		~Mesh();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromMemory(const List<float> & vertices);
		bool loadFromMemory(const VertexList & vertices);
		bool loadFromMemory(const VertexList & vertices, const List<uint32_t> & indices);

		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	public:
		inline const VertexList		&	vertices	() const { return m_vertices;	}
		inline const List<uint32_t> &	indices		() const { return m_indices;	}
		inline const List<float>	&	contiguous	() const { return m_contiguous; }

	private:
		VertexList		m_vertices;
		List<uint32_t>	m_indices;
		List<float>		m_contiguous;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_MESH_HPP_