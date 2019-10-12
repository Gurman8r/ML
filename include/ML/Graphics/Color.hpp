#ifndef _ML_COLOR_HPP_
#define _ML_COLOR_HPP_

#include <ML/Graphics/Export.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Core/I_NonNewable.hpp>

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

		static constexpr vec4b color_cast(const vec4f & value)
		{
			return {
				(byte_t)(value[0] * 255.f),
				(byte_t)(value[1] * 255.f),
				(byte_t)(value[2] * 255.f),
				(byte_t)(value[3] * 255.f)
			};
		}

		static constexpr vec4f color_cast(const vec4b & value)
		{
			return {
				(float_t)value[0] / 255.f,
				(float_t)value[1] / 255.f,
				(float_t)value[2] / 255.f,
				(float_t)value[3] / 255.f
			};
		}
	}

	/* * * * * * * * * * * * * * * * * * * * */
	
	template <class T> struct BasicColor final : public I_NonNewable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using type		= typename T;
		using rgb_t		= typename tvec3<type>;
		using rgba_t	= typename tvec4<type>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr BasicColor(const rgba_t & value)
			: m_value { value }
		{
		}

		constexpr BasicColor(const rgb_t & rgb, type a)
			: m_value { rgb[0], rgb[1], rgb[2], a }
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
			: m_value { uninit }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator rgba_t &() { return m_value; }

		constexpr operator const rgba_t &() const { return m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline type & operator[](size_t i) { return m_value[i]; }

		constexpr const type & operator[](size_t i) const { return m_value[i]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto r()		const -> const type &	{ return m_value[0]; }
		constexpr auto g()		const -> const type &	{ return m_value[1]; }
		constexpr auto b()		const -> const type &	{ return m_value[2]; }
		constexpr auto a()		const -> const type &	{ return m_value[3]; }
		constexpr auto rgb()	const -> const rgb_t	{ return (rgb_t)m_value; }
		constexpr auto rgba()	const -> const rgba_t & { return m_value; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: rgba_t m_value;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using Color		= BasicColor<float_t>;
	using Color32	= BasicColor<byte_t>;

	/* * * * * * * * * * * * * * * * * * * * */

	struct Colors final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Colors() = delete;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Color clear	{ 0.0f, 0.0f, 0.0f, 0.0f };
		static constexpr Color white	{ 1.0f, 1.0f, 1.0f, 1.0f };
		static constexpr Color black	{ 0.0f, 0.0f, 0.0f, 1.0f };
		static constexpr Color gray		{ 0.5f, 0.5f, 0.5f, 1.0f };

		static constexpr Color red		{ 1.0f, 0.0f, 0.0f, 1.0f };
		static constexpr Color green	{ 0.0f, 1.0f, 0.0f, 1.0f };
		static constexpr Color blue		{ 0.0f, 0.0f, 1.0f, 1.0f };

		static constexpr Color cyan		{ 0.0f, 1.0f, 1.0f, 1.0f };
		static constexpr Color yellow	{ 1.0f, 1.0f, 0.0f, 1.0f };
		static constexpr Color magenta	{ 1.0f, 0.0f, 1.0f, 1.0f };

		static constexpr Color violet	{ 0.5f, 0.0f, 1.0f, 1.0f };
		static constexpr Color lime		{ 0.5f, 1.0f, 0.0f, 1.0f };
		static constexpr Color orange	{ 1.0f, 0.5f, 0.0f, 1.0f };

		static constexpr Color fuchsia	{ 1.0f, 0.0f, 0.5f, 1.0f };
		static constexpr Color aqua		{ 0.0f, 1.0f, 0.5f, 1.0f };
		static constexpr Color azure	{ 0.0f, 0.5f, 1.0f, 1.0f };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_COLOR_HPP_