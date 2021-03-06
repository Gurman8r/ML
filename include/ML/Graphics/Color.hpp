#ifndef _ML_COLOR_HPP_
#define _ML_COLOR_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		template <
			class To, class From
		> static constexpr tvec4<To> color_cast(const tvec4<From> & value)
		{
			return (tvec4<To>)value;
		}

		static constexpr vec4b color_cast(vec4f const & value)
		{
			return {
				static_cast<byte_t>(value[0] * 255.f),
				static_cast<byte_t>(value[1] * 255.f),
				static_cast<byte_t>(value[2] * 255.f),
				static_cast<byte_t>(value[3] * 255.f)
			};
		}

		static constexpr vec4f color_cast(vec4b const & value)
		{
			return {
				static_cast<float_t>(value[0]) / 255.f,
				static_cast<float_t>(value[1]) / 255.f,
				static_cast<float_t>(value[2]) / 255.f,
				static_cast<float_t>(value[3]) / 255.f
			};
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	template <class T> struct BasicColor final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type		= typename T;
		using rgb_t		= typename tvec3<type>;
		using rgba_t	= typename tvec4<type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr BasicColor(rgba_t const & value)
			: m_value { value }
		{
		}

		constexpr BasicColor(rgb_t const & rgb, type a)
			: m_value { rgb[0], rgb[1], rgb[2], a }
		{
		}

		constexpr BasicColor(type rgba)
			: m_value { rgba, rgba, rgba, rgba }
		{
		}

		constexpr BasicColor(type r, type g, type b)
			: m_value { r, g, b, 1.0f }
		{
		}

		constexpr BasicColor(type r, type g, type b, type a)
			: m_value { r, g, b, a }
		{
		}

		template <class U> constexpr BasicColor(const tvec4<U> & value)
			: m_value { detail::color_cast(value) }
		{
		}

		template <class U> constexpr BasicColor(const BasicColor<U> & copy)
			: m_value { detail::color_cast(copy.rgba()) }
		{
		}

		constexpr BasicColor()
			: m_value { 0 }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator rgba_t &() { return m_value; }

		constexpr operator rgba_t const &() const { return m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline type & operator[](size_t i) { return m_value[i]; }

		constexpr type const & operator[](size_t i) const { return m_value[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto r()		const -> type const &	{ return m_value[0]; }
		constexpr auto g()		const -> type const &	{ return m_value[1]; }
		constexpr auto b()		const -> type const &	{ return m_value[2]; }
		constexpr auto a()		const -> type const &	{ return m_value[3]; }
		constexpr auto rgb()	const -> const rgb_t	{ return (rgb_t)m_value; }
		constexpr auto rgba()	const -> rgba_t const & { return m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: rgba_t m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	ML_USING Color = BasicColor<float_t>;
	ML_USING Color32 = BasicColor<byte_t>;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> inline ML_SERIALIZE(std::ostream & out, const BasicColor<T> & value)
	{
		return out << value.rgba();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	namespace Colors
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Color clear		{ 0.0f, 0.0f, 0.0f, 0.0f };
		static constexpr Color white		{ 1.0f, 1.0f, 1.0f, 1.0f };
		static constexpr Color gray			{ 0.5f, 0.5f, 0.5f, 1.0f };
		static constexpr Color black		{ 0.0f, 0.0f, 0.0f, 1.0f };
		static constexpr Color red			{ 1.0f, 0.0f, 0.0f, 1.0f };
		static constexpr Color green		{ 0.0f, 1.0f, 0.0f, 1.0f };
		static constexpr Color blue			{ 0.0f, 0.0f, 1.0f, 1.0f };
		static constexpr Color cyan			{ 0.0f, 1.0f, 1.0f, 1.0f };
		static constexpr Color yellow		{ 1.0f, 1.0f, 0.0f, 1.0f };
		static constexpr Color magenta		{ 1.0f, 0.0f, 1.0f, 1.0f };
		static constexpr Color violet		{ 0.5f, 0.0f, 1.0f, 1.0f };
		static constexpr Color lime			{ 0.5f, 1.0f, 0.0f, 1.0f };
		static constexpr Color orange		{ 1.0f, 0.5f, 0.0f, 1.0f };
		static constexpr Color fuchsia		{ 1.0f, 0.0f, 0.5f, 1.0f };
		static constexpr Color aqua			{ 0.0f, 1.0f, 0.5f, 1.0f };
		static constexpr Color azure		{ 0.0f, 0.5f, 1.0f, 1.0f };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COLOR_HPP_