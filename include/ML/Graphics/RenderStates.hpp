#ifndef _ML_RENDER_STATES_HPP_
#define _ML_RENDER_STATES_HPP_

#include <ML/Core/I_Newable.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API AlphaState final
	{
		bool enabled;
		GL::Predicate predicate;
		float_t coeff;

		explicit constexpr AlphaState(bool enabled, GL::Predicate predicate, float_t coeff)
			: enabled(enabled)
			, predicate(predicate)
			, coeff(coeff)
		{
		}

		constexpr AlphaState(const AlphaState & copy)
			: AlphaState(copy.enabled, copy.predicate, copy.coeff)
		{
		}

		constexpr AlphaState(bool enabled)
			: AlphaState(enabled, GL::Greater, 0.01f)
		{
		}

		constexpr AlphaState()
			: AlphaState(true)
		{
		}

		const AlphaState & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API BlendState final
	{
		bool enabled;
		GL::Factor srcRGB;
		GL::Factor srcAlpha;
		GL::Factor dstRGB;
		GL::Factor dstAlpha;

		constexpr explicit BlendState(bool enabled, GL::Factor srcRGB, GL::Factor srcAlpha, GL::Factor dstRGB, GL::Factor dstAlpha)
			: enabled	(enabled)
			, srcRGB	(srcRGB)
			, srcAlpha	(srcAlpha)
			, dstRGB	(dstRGB)
			, dstAlpha	(dstAlpha)
		{
		}

		constexpr BlendState(const BlendState & copy)
			: BlendState(copy.enabled, copy.srcRGB, copy.srcAlpha, copy.dstRGB, copy.dstAlpha)
		{
		}

		constexpr BlendState(bool enabled, GL::Factor rgb, GL::Factor alpha)
			: BlendState(enabled, rgb, alpha, rgb, alpha)
		{
		}

		constexpr BlendState(bool enabled)
			: BlendState(enabled, GL::SrcAlpha, GL::OneMinusSrcAlpha)
		{
		}

		constexpr BlendState()
			: BlendState(true)
		{
		}

		const BlendState & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API CullState final
	{
		bool enabled;
		GL::Face face;

		constexpr explicit CullState(bool enabled, GL::Face face)
			: enabled(enabled)
			, face(face)
		{
		}

		constexpr CullState(const CullState & copy)
			: CullState(copy.enabled, copy.face)
		{
		}

		constexpr CullState(bool enabled)
			: CullState(enabled, GL::Back)
		{
		}

		constexpr CullState()
			: CullState(true)
		{
		}

		const CullState & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API DepthState final
	{
		bool enabled;
		GL::Predicate predicate;
		bool mask;

		constexpr explicit DepthState(bool enabled, GL::Predicate predicate, bool mask)
			: enabled(enabled)
			, predicate(predicate)
			, mask(mask)
		{
		}

		constexpr DepthState(bool enabled, bool mask)
			: DepthState(enabled, GL::Less, mask)
		{
		}

		constexpr DepthState(const DepthState & copy)
			: DepthState(copy.enabled, copy.predicate, copy.mask)
		{
		}

		constexpr DepthState(bool enabled)
			: DepthState(enabled, GL::Less, true)
		{
		}

		constexpr DepthState()
			: DepthState(true)
		{
		}

		const DepthState & operator()() const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderStates final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr RenderStates()
			: m_alpha	{}
			, m_blend	{}
			, m_cull	{}
			, m_depth	{}
		{
		}

		constexpr RenderStates(const AlphaState & alpha, const BlendState & blend, const CullState & cull, const DepthState & depth)
			: m_alpha	{ alpha }
			, m_blend	{ blend }
			, m_cull	{ cull  }
			, m_depth	{ depth }
		{
		}

		constexpr RenderStates(const RenderStates & copy)
			: m_alpha	{ copy.m_alpha }
			, m_blend	{ copy.m_blend }
			, m_cull	{ copy.m_cull  }
			, m_depth	{ copy.m_depth }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const RenderStates & apply() const;

		inline auto operator()() const { return this->apply(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto alpha() -> AlphaState & { return m_alpha; }
		inline auto blend() -> BlendState & { return m_blend; }
		inline auto cull()	-> CullState  & { return m_cull;  }
		inline auto depth() -> DepthState & { return m_depth; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		AlphaState	m_alpha;
		BlendState	m_blend;
		CullState	m_cull;
		DepthState	m_depth;
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_STATES_HPP_