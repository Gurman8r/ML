#ifndef _ML_RENDER_STATES_HPP_
#define _ML_RENDER_STATES_HPP_

#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/Matrix.hpp>
#include <ML/Graphics/Export.hpp>
#include <ML/Graphics/GL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderSetting : public I_Newable
	{
		inline size_t get_id() const
		{ 
			return typeid(*this).hash_code(); 
		}
		
		virtual ~RenderSetting() {}

		virtual RenderSetting * clone() const = 0;

		virtual const RenderSetting & operator()() const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API AlphaTestState final : public RenderSetting
	{
		bool enabled;
		GL::Comp comp;
		float_t coeff;

		explicit AlphaTestState(bool enabled, GL::Comp comp, float_t coeff)
			: enabled(enabled)
			, comp(comp)
			, coeff(coeff)
		{
		}

		AlphaTestState(const AlphaTestState & copy) 
			: AlphaTestState(copy.enabled, copy.comp, copy.coeff)
		{
		}

		AlphaTestState(bool enabled) 
			: AlphaTestState(enabled, GL::Greater, 0.01f)
		{
		}

		AlphaTestState() 
			: AlphaTestState(true)
		{
		}

		inline AlphaTestState * clone() const override
		{
			return new AlphaTestState { enabled, comp, coeff };
		}

		const AlphaTestState & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API BlendFuncState final : public RenderSetting
	{
		bool enabled;
		GL::Factor srcRGB;
		GL::Factor srcAlpha;
		GL::Factor dstRGB;
		GL::Factor dstAlpha;

		explicit BlendFuncState(bool enabled, GL::Factor srcRGB, GL::Factor srcAlpha, GL::Factor dstRGB, GL::Factor dstAlpha)
			: enabled(enabled)
			, srcRGB	(srcRGB)
			, srcAlpha	(srcAlpha)
			, dstRGB	(dstRGB)
			, dstAlpha	(dstAlpha)
		{
		}

		BlendFuncState(const BlendFuncState & copy)
			: BlendFuncState(copy.enabled, copy.srcRGB, copy.srcAlpha, copy.dstRGB, copy.dstAlpha)
		{
		}

		BlendFuncState(bool enabled, GL::Factor rgb, GL::Factor alpha)
			: BlendFuncState(enabled, rgb, alpha, rgb, alpha)
		{
		}

		BlendFuncState(bool enabled) 
			: BlendFuncState(enabled, GL::SrcAlpha, GL::OneMinusSrcAlpha)
		{
		}

		BlendFuncState()
			: BlendFuncState(true)
		{
		}

		inline BlendFuncState * clone() const override
		{
			return new BlendFuncState { enabled, srcRGB, srcAlpha, dstRGB, dstAlpha };
		}

		const BlendFuncState & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API CullFaceState final : public RenderSetting
	{
		bool enabled;
		GL::Face face;

		explicit CullFaceState(bool enabled, GL::Face face)
			: enabled(enabled)
			, face(face)
		{
		}

		CullFaceState(const CullFaceState & copy) 
			: CullFaceState(copy.enabled, copy.face)
		{
		}

		CullFaceState(bool enabled) 
			: CullFaceState(enabled, GL::Back)
		{
		}

		CullFaceState()
			: CullFaceState(true)
		{
		}

		inline CullFaceState * clone() const override
		{
			return new CullFaceState { enabled, face };
		}

		const CullFaceState & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API DepthMaskState final : public RenderSetting
	{
		bool enabled;

		explicit DepthMaskState(bool enabled)
			: enabled(enabled)
		{
		}

		DepthMaskState(const DepthMaskState & copy) 
			: DepthMaskState(copy.enabled)
		{
		}

		DepthMaskState()
			: DepthMaskState(true)
		{
		}

		inline DepthMaskState * clone() const override
		{
			return new DepthMaskState { enabled };
		}

		const DepthMaskState & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API DepthTestState final : public RenderSetting
	{
		bool enabled;
		GL::Comp comp;

		explicit DepthTestState(bool enabled, GL::Comp comp)
			: enabled(enabled)
			, comp(comp)
		{
		}

		DepthTestState(const DepthTestState & copy) 
			: DepthTestState(copy.enabled)
		{
		}

		DepthTestState(bool enabled) 
			: DepthTestState(enabled, GL::Less)
		{
		}

		DepthTestState()
			: DepthTestState(true)
		{
		}

		inline DepthTestState * clone() const override
		{
			return new DepthTestState { enabled, comp };
		}

		const DepthTestState & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API PolygonModeState final : public RenderSetting
	{
		bool enabled;
		GL::Face face;
		GL::Mode mode;

		explicit PolygonModeState(bool enabled, GL::Face face, GL::Mode mode)
			: enabled(enabled)
			, face(face)
			, mode(mode)
		{
		}

		PolygonModeState(bool enabled) 
			: PolygonModeState(enabled, GL::FrontAndBack, GL::Fill)
		{
		}

		PolygonModeState(const PolygonModeState & copy) 
			: PolygonModeState(copy.enabled, copy.face, copy.mode)
		{
		}

		PolygonModeState()
			: PolygonModeState(true)
		{
		}

		inline PolygonModeState * clone() const override
		{
			return new PolygonModeState { enabled, face, mode };
		}

		const PolygonModeState & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API ScissorTestState final : public RenderSetting
	{
		bool enabled;

		explicit ScissorTestState(bool enabled)
			: enabled(enabled)
		{
		}

		ScissorTestState(const ScissorTestState & copy) 
			: ScissorTestState(copy.enabled)
		{
		}

		ScissorTestState()
			: ScissorTestState(true)
		{
		}

		inline ScissorTestState * clone() const override
		{
			return new ScissorTestState { enabled };
		}

		const ScissorTestState & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API RenderStates final 
		: public I_Newable
		, public I_Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type			= typename HashMap<size_t, RenderSetting *>;
		using key_type			= typename base_type::key_type;
		using mapped_type		= typename base_type::mapped_type;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderStates();
		RenderStates(List<mapped_type> && data);
		RenderStates(const RenderStates & copy);
		~RenderStates();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		const RenderStates & apply() const;
		const RenderStates & revert() const; // WIP

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(key_type key)
		{ 
			return m_map.find(key);
		}

		inline const_iterator find(key_type key) const
		{ 
			return m_map.find(key);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline iterator find()
		{
			return this->find(typeid(T).hash_code());
		}

		template <
			class T
		> inline const_iterator find() const
		{
			return this->find(typeid(T).hash_code());
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator insert(key_type key, mapped_type value)
		{
			return m_map.insert({ key, value }).first;
		}

		template <
			class T
		> inline T * insert(mapped_type value)
		{
			iterator it;
			return ((value && ((it = this->find<T>()) == this->end()))
				? reinterpret_cast<T *>(this->insert(value->get_id(), value)->second)
				: nullptr
			);
		}

		template <
			class T, class ... Args
		> inline T * create(Args && ... args)
		{
			return ((this->find(typeid(T).hash_code()) == this->end())
				? this->insert<T>(new T { std::forward<Args>(args)... })
				: nullptr
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T
		> inline T * get()
		{
			iterator it;
			return (((it = this->find<T>()) != this->end())
				? reinterpret_cast<T *>(it->second)
				: nullptr
			);
		}

		template <
			class T
		> inline const T * get() const
		{
			const_iterator it;
			return (((it = this->find<T>()) != this->end())
				? reinterpret_cast<const T *>(it->second)
				: nullptr
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()			-> iterator			{ return m_map.begin(); }
		inline auto begin() const	-> const_iterator	{ return m_map.begin(); }
		inline auto cbegin() const	-> const_iterator	{ return m_map.cbegin(); }
		inline auto end()			-> iterator			{ return m_map.end(); }
		inline auto end() const		-> const_iterator	{ return m_map.end(); }
		inline auto cend() const	-> const_iterator	{ return m_map.cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: base_type m_map;
	
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_STATES_HPP_