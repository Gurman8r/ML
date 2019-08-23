#ifndef _ML_BUFFER_LAYOUT_HPP_
#define _ML_BUFFER_LAYOUT_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Core/I_Newable.hpp>
//#include <ML/Core/TypeInfo.hpp>
//#include <mapbox/eternal.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API BufferLayout final : public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		//static constexpr const auto test = mapbox::eternal::map<hash_t, size_t>({
		//{ ml_type_id<vec2>().hash_code(), sizeof(float_t) * 2 * 1 },
		//{ ml_type_id<vec3>().hash_code(), sizeof(float_t) * 3 * 1 },
		//{ ml_type_id<vec4>().hash_code(), sizeof(float_t) * 4 * 1 },
		//{ ml_type_id<mat3>().hash_code(), sizeof(float_t) * 3 * 3 },
		//{ ml_type_id<mat4>().hash_code(), sizeof(float_t) * 4 * 4 },
		//});
		//
		//template <class T>
		//static constexpr size_t size_of()
		//{
		//	return test.find(ml_type_id<T>().hash_code())->second;
		//}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Element final
		{
			uint32_t	index;
			uint32_t	size;
			GL::Type	type;
			bool		normalized;
			uint32_t	stride;
			uint32_t	offset;
			uint32_t	width;

			const Element & operator()() const;
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		BufferLayout();
		BufferLayout(const List<Element> & elements);
		BufferLayout(const Initializer<Element> & elements);
		BufferLayout(const BufferLayout & copy);
		~BufferLayout();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static const BufferLayout & get_default();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void bind() const;

		BufferLayout & push_back(const Element & value);
		
		inline const List<Element> & elements() const { return m_elements; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: List<Element> m_elements;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUFFER_LAYOUT_HPP_