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

	struct ML_GRAPHICS_API AlphaState final : public RenderSetting
	{
		bool enabled;
		GL::Predicate predicate;
		float_t coeff;

		explicit AlphaState(bool enabled, GL::Predicate predicate, float_t coeff)
			: enabled(enabled)
			, predicate(predicate)
			, coeff(coeff)
		{
		}

		AlphaState(const AlphaState & copy) 
			: AlphaState(copy.enabled, copy.predicate, copy.coeff)
		{
		}

		AlphaState(bool enabled) 
			: AlphaState(enabled, GL::Greater, 0.01f)
		{
		}

		AlphaState() 
			: AlphaState(true)
		{
		}

		inline AlphaState * clone() const override
		{
			return new AlphaState { enabled, predicate, coeff };
		}

		const AlphaState & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API BlendState final : public RenderSetting
	{
		bool enabled;
		GL::Factor srcRGB;
		GL::Factor srcAlpha;
		GL::Factor dstRGB;
		GL::Factor dstAlpha;

		explicit BlendState(bool enabled, GL::Factor srcRGB, GL::Factor srcAlpha, GL::Factor dstRGB, GL::Factor dstAlpha)
			: enabled(enabled)
			, srcRGB	(srcRGB)
			, srcAlpha	(srcAlpha)
			, dstRGB	(dstRGB)
			, dstAlpha	(dstAlpha)
		{
		}

		BlendState(const BlendState & copy)
			: BlendState(copy.enabled, copy.srcRGB, copy.srcAlpha, copy.dstRGB, copy.dstAlpha)
		{
		}

		BlendState(bool enabled, GL::Factor rgb, GL::Factor alpha)
			: BlendState(enabled, rgb, alpha, rgb, alpha)
		{
		}

		BlendState(bool enabled) 
			: BlendState(enabled, GL::SrcAlpha, GL::OneMinusSrcAlpha)
		{
		}

		BlendState()
			: BlendState(true)
		{
		}

		inline BlendState * clone() const override
		{
			return new BlendState { enabled, srcRGB, srcAlpha, dstRGB, dstAlpha };
		}

		const BlendState & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API CullState final : public RenderSetting
	{
		bool enabled;
		GL::Face face;

		explicit CullState(bool enabled, GL::Face face)
			: enabled(enabled)
			, face(face)
		{
		}

		CullState(const CullState & copy) 
			: CullState(copy.enabled, copy.face)
		{
		}

		CullState(bool enabled) 
			: CullState(enabled, GL::Back)
		{
		}

		CullState()
			: CullState(true)
		{
		}

		inline CullState * clone() const override
		{
			return new CullState { enabled, face };
		}

		const CullState & operator()() const override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API DepthState final : public RenderSetting
	{
		bool enabled;
		GL::Predicate predicate;
		bool mask;

		explicit DepthState(bool enabled, GL::Predicate predicate, bool mask)
			: enabled(enabled)
			, predicate(predicate)
			, mask(mask)
		{
		}

		DepthState(const DepthState & copy) 
			: DepthState(copy.enabled)
		{
		}

		DepthState(bool enabled) 
			: DepthState(enabled, GL::Less, true)
		{
		}

		DepthState()
			: DepthState(true)
		{
		}

		inline DepthState * clone() const override
		{
			return new DepthState { enabled, predicate, mask };
		}

		const DepthState & operator()() const override;
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

		static const RenderStates & get_default();

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