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

	struct ML_GRAPHICS_API AlphaTest final : public RenderSetting
	{
		bool enabled;
		GL::Comp comp;
		float_t coeff;

		explicit AlphaTest(bool enabled, GL::Comp comp, float_t coeff)
			: enabled	{ enabled }
			, comp		{ comp }
			, coeff		{ coeff }
		{
		}

		AlphaTest(const AlphaTest & copy) 
			: AlphaTest(copy.enabled, copy.comp, copy.coeff)
		{
		}

		AlphaTest(bool enabled) 
			: AlphaTest(enabled, GL::Greater, 0.01f)
		{
		}

		AlphaTest() 
			: AlphaTest(true)
		{
		}

		inline AlphaTest * clone() const override
		{
			return new AlphaTest(enabled, comp, coeff);
		}

		const AlphaTest & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API BlendFunc final : public RenderSetting
	{
		bool enabled;
		GL::Factor srcRGB;
		GL::Factor srcAlpha;
		GL::Factor dstRGB;
		GL::Factor dstAlpha;

		explicit BlendFunc(bool enabled, GL::Factor srcRGB, GL::Factor srcAlpha, GL::Factor dstRGB, GL::Factor dstAlpha)
			: enabled	{ enabled }
			, srcRGB	{ srcRGB }
			, srcAlpha	{ srcAlpha }
			, dstRGB	{ dstRGB }
			, dstAlpha	{ dstAlpha }
		{
		}

		BlendFunc(const BlendFunc & copy)
			: BlendFunc(copy.enabled, copy.srcRGB, copy.srcAlpha, copy.dstRGB, copy.dstAlpha)
		{
		}

		BlendFunc(bool enabled, GL::Factor rgb, GL::Factor alpha)
			: BlendFunc(enabled, rgb, alpha, rgb, alpha)
		{
		}

		BlendFunc(bool enabled) 
			: BlendFunc(enabled, GL::SrcAlpha, GL::OneMinusSrcAlpha)
		{
		}

		BlendFunc()
			: BlendFunc(true)
		{
		}

		inline BlendFunc * clone() const override
		{
			return new BlendFunc(enabled, srcRGB, srcAlpha, dstRGB, dstAlpha);
		}

		const BlendFunc & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API CullFace final : public RenderSetting
	{
		bool enabled;
		GL::Face face;

		explicit CullFace(bool enabled, GL::Face face)
			: enabled	{ enabled }
			, face		{ face }
		{
		}

		CullFace(const CullFace & copy) 
			: CullFace(copy.enabled, copy.face)
		{
		}

		CullFace(bool enabled) 
			: CullFace(enabled, GL::Back)
		{
		}

		CullFace()
			: CullFace(true)
		{
		}

		inline CullFace * clone() const override
		{
			return new CullFace(enabled, face);
		}

		const CullFace & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API DepthMask final : public RenderSetting
	{
		bool enabled;

		explicit DepthMask(bool enabled)
			: enabled	{ enabled }
		{
		}

		DepthMask(const DepthMask & copy) 
			: DepthMask(copy.enabled)
		{
		}

		DepthMask()
			: DepthMask(true)
		{
		}

		inline DepthMask * clone() const override
		{
			return new DepthMask(enabled);
		}

		const DepthMask & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API DepthTest final : public RenderSetting
	{
		bool enabled;
		GL::Comp comp;

		explicit DepthTest(bool enabled, GL::Comp comp)
			: enabled	{ enabled }
			, comp		{ comp }
		{
		}

		DepthTest(const DepthTest & copy) 
			: DepthTest(copy.enabled)
		{
		}

		DepthTest(bool enabled) 
			: DepthTest(enabled, GL::Less)
		{
		}

		DepthTest()
			: DepthTest(true)
		{
		}

		inline DepthTest * clone() const override
		{
			return new DepthTest(enabled, comp);
		}

		const DepthTest & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API PolygonMode final : public RenderSetting
	{
		bool	 enabled;
		GL::Face face;
		GL::Mode mode;

		explicit PolygonMode(bool enabled, GL::Face face, GL::Mode mode)
			: enabled	{ enabled }
			, face		{ face }
			, mode		{ mode }
		{
		}

		PolygonMode(bool enabled) 
			: PolygonMode(enabled, GL::FrontAndBack, GL::Fill)
		{
		}

		PolygonMode(const PolygonMode & copy) 
			: PolygonMode(copy.enabled, copy.face, copy.mode)
		{
		}

		PolygonMode()
			: PolygonMode(true)
		{
		}

		inline PolygonMode * clone() const override
		{
			return new PolygonMode(enabled, face, mode);
		}

		const PolygonMode & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API ScissorTest final : public RenderSetting
	{
		bool enabled;

		explicit ScissorTest(bool enabled)
			: enabled	{ enabled }
		{
		}

		ScissorTest(const ScissorTest & copy) 
			: ScissorTest(copy.enabled)
		{
		}

		ScissorTest()
			: ScissorTest(true)
		{
		}

		inline ScissorTest * clone() const override
		{
			return new ScissorTest(enabled);
		}

		const ScissorTest & operator()() const override;
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
		using pointer			= typename base_type::pointer;
		using reference			= typename base_type::reference;
		using const_pointer		= typename base_type::const_pointer;
		using const_reference	= typename base_type::const_reference;
		using iterator			= typename base_type::iterator;
		using const_iterator	= typename base_type::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		RenderStates();
		RenderStates(List<mapped_type> && data);
		RenderStates(const RenderStates & copy);
		~RenderStates();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			if (!m_map.empty())
			{
				for (auto & pair : m_map)
				{
					if (pair.second) { delete pair.second; }
				}
				m_map.clear();
			}
			return m_map.empty();
		}

		inline const RenderStates & apply() const
		{
			for (const auto & pair : (*this))
			{
				if (pair.second) { (*pair.second)(); }
			}
			return (*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(key_type value) 
		{ 
			return m_map.find(value); 
		}

		inline const_iterator find(key_type value) const 
		{ 
			return m_map.find(value); 
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
			if (value && ((it = this->find<T>()) == this->end()))
			{
				return reinterpret_cast<T *>(this->insert(
					value->get_id(), value
				)->second);
			}
			return nullptr;
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
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RENDER_STATES_HPP_