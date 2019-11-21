#ifndef _ML_WINDOW_STYLE_HPP_
#define _ML_WINDOW_STYLE_HPP_

#include <ML/Window/Export.hpp>
#include <ML/Core/BitMask.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct WindowStyle final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename mask8_t;
		using self_type = typename WindowStyle;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Flag : size_t
		{
			Resizable,
			Visible,
			Decorated,
			Focused,
			AutoIconify,
			Floating,
			Maximized,
		};

		static constexpr Flag Flag_values[] = {
			Flag::Resizable,
			Flag::Visible,
			Flag::Decorated,
			Flag::Focused,
			Flag::AutoIconify,
			Flag::Floating,
			Flag::Maximized
		};

		static constexpr C_String Flag_names[] = {
			"Resizable",
			"Visible",
			"Decorated",
			"Focused",
			"AutoIconify",
			"Floating",
			"Maximized"
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr WindowStyle()
			: self_type{ base_type{ 0 } }
		{
		}

		constexpr WindowStyle(
			bool resizable,
			bool visible,
			bool decorated,
			bool focused,
			bool autoIconify,
			bool floating,
			bool maximized
		) : self_type{ base_type { {
			resizable, visible, decorated, focused, autoIconify, floating, maximized, false
		} } }
		{
		}

		constexpr WindowStyle(WindowStyle const & copy)
			: self_type{ copy.m_data }
		{
		}

		constexpr WindowStyle(base_type const & data)
			: m_data{ data }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr auto data() const -> base_type const & { return m_data; }
		
		constexpr auto data() -> base_type & { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bool resizable()		const { return m_data[Resizable]; }
		constexpr bool visible()		const { return m_data[Visible]; }
		constexpr bool decorated()		const { return m_data[Decorated]; }
		constexpr bool focused()		const { return m_data[Focused]; }
		constexpr bool autoIconify()	const { return m_data[AutoIconify]; }
		constexpr bool floating()		const { return m_data[Floating]; }
		constexpr bool maximized()		const { return m_data[Maximized]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: base_type m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_STYLE_HPP_