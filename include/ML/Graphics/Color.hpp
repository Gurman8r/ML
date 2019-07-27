#ifndef _ML_COLOR_HPP_
#define _ML_COLOR_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Matrix.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	struct Color final : public vec4f
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr vec4 clear		{ 0.0f, 0.0f, 0.0f, 0.0f };
		static constexpr vec4 white		{ 1.0f, 1.0f, 1.0f, 1.0f };
		static constexpr vec4 black		{ 0.0f, 0.0f, 0.0f, 1.0f };
		static constexpr vec4 gray		{ 0.2f, 0.2f, 0.2f, 1.0f };

		static constexpr vec4 red		{ 1.0f, 0.0f, 0.0f, 1.0f };
		static constexpr vec4 green		{ 0.0f, 1.0f, 0.0f, 1.0f };
		static constexpr vec4 blue		{ 0.0f, 0.0f, 1.0f, 1.0f };

		static constexpr vec4 cyan		{ 0.0f, 1.0f, 1.0f, 1.0f };
		static constexpr vec4 yellow	{ 1.0f, 1.0f, 0.0f, 1.0f };
		static constexpr vec4 magenta	{ 1.0f, 0.0f, 1.0f, 1.0f };

		static constexpr vec4 violet	{ 0.5f, 0.0f, 1.0f, 1.0f };
		static constexpr vec4 lime		{ 0.5f, 1.0f, 0.0f, 1.0f };
		static constexpr vec4 orange	{ 1.0f, 0.5f, 0.0f, 1.0f };

		static constexpr vec4 fuchsia	{ 1.0f, 0.0f, 0.5f, 1.0f };
		static constexpr vec4 aqua		{ 0.0f, 1.0f, 0.5f, 1.0f };
		static constexpr vec4 azure		{ 0.0f, 0.5f, 1.0f, 1.0f };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename float_t;
		using self_type			= typename Color;
		using base_type			= typename tvec4<value_type>;
		using complex_type		= typename tvec3<value_type>;
		using cast_type			= typename base_type::cast_type;
		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Color()
			: base_type { 1.0f, 1.0f, 1.0f, 1.0f }
		{
		}

		constexpr Color(value_type r, value_type g, value_type b, value_type a)
			: base_type { r, g, b, a }
		{
		}

		constexpr Color(value_type r, value_type g, value_type b)
			: base_type { r, g, b, 1.0f }
		{
		}

		constexpr Color(const self_type & copy)
			: base_type { copy }
		{
		}

		template <
			class U
		> constexpr Color(const tvec4<U> & copy)
			: base_type { copy }
		{
		}

		template <
			class U
		> constexpr operator tvec4<U>() const
		{
			using UT = static_value<U>;
			return { UT(r()), UT(g()), UT(b()), UT(a()) };
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto r() { return (*this)[0]; }
		constexpr auto g() { return (*this)[1]; }
		constexpr auto b() { return (*this)[2]; }
		constexpr auto a() { return (*this)[3]; }

		constexpr auto r() const { return (*this)[0]; }
		constexpr auto g() const { return (*this)[1]; }
		constexpr auto b() const { return (*this)[2]; }
		constexpr auto a() const { return (*this)[3]; }
		constexpr auto rgb() const -> vec3f { return { r(), g(), b() }; }

		constexpr auto r(value_type value) { (*this)[0] = value; return (*this); }
		constexpr auto g(value_type value) { (*this)[1] = value; return (*this); }
		constexpr auto b(value_type value) { (*this)[2] = value; return (*this); }
		constexpr auto a(value_type value) { (*this)[3] = value; return (*this); }
		constexpr auto rgb(const vec3f & value) { return r(value[0]).g(value[1]).b(value[2]); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COLOR_HPP_