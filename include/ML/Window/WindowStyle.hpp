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

		enum Flag : uint32_t
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

		constexpr WindowStyle(const BitMask_8 & value)
			: m_bits{ value }
		{
		}

		constexpr WindowStyle(
			bool resizable, bool visible, bool decorated, bool focused, bool autoIconify, bool floating, bool maximized
		) : m_bits{ {
			resizable, visible, decorated, focused, autoIconify, floating, maximized, 0
		} }
		{
		}

		constexpr WindowStyle(const WindowStyle & copy)
			: WindowStyle{ copy.m_bits }
		{
		}

		constexpr WindowStyle()
			: WindowStyle{ true, true, true, true, true, false, false }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr bool resizable()		const { return m_bits[0]; }
		constexpr bool visible()		const { return m_bits[1]; }
		constexpr bool decorated()		const { return m_bits[2]; }
		constexpr bool focused()		const { return m_bits[3]; }
		constexpr bool autoIconify()	const { return m_bits[4]; }
		constexpr bool floating()		const { return m_bits[5]; }
		constexpr bool maximized()		const { return m_bits[6]; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		BitMask_8 m_bits;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_WINDOW_STYLE_HPP_
