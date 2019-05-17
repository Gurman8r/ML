#ifndef _ML_RENDER_VAR_HPP_
#define _ML_RENDER_VAR_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/ITrackable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	class ML_GRAPHICS_API RenderVar final
		: public ITrackable
	{
	public:
		using value_type		= typename int64_t;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		enum : uint32_t
		{
			Bool, Int
		};

	public:
		RenderVar();
		RenderVar(const uint32_t type, const_reference data);
		RenderVar(const RenderVar & copy);
		~RenderVar();

	public:
		inline const uint32_t & type() const	{ return m_type;  }
		inline const_reference	data() const	{ return m_data;  }
		inline reference &		data()			{ return m_data;  }
		inline const_pointer	ptr() const		{ return &m_data; }
		inline pointer 			ptr()			{ return &m_data; }

	private:
		uint32_t	m_type;
		value_type	m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_VAR_HPP_