#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <ML/Graphics/Shader.hpp>
#include <ML/Graphics/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Materials are collections of uniforms
	struct ML_GRAPHICS_API Material final
		: public Newable
		, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using iterator					= typename List<Uniform *>::iterator;
		using const_iterator			= typename List<Uniform *>::const_iterator;
		using reverse_iterator			= typename List<Uniform *>::reverse_iterator;
		using const_reverse_iterator	= typename List<Uniform *>::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Material();
		Material(List<Uniform *> && uniforms);
		Material(const Material & copy);
		~Material();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename);
		bool loadFromFile(const String & filename, const Map<String, Texture *> * textures);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Uniform * insert(Uniform * value)
		{
			if (!value) { return nullptr; }
			auto it { std::find_if(begin(), end(), [&](auto u)
			{
				return u && (u->name == value->name);
			}) };
			if (it == end())
			{
				m_uniforms.push_back(std::move(value));
				return m_uniforms.back();
			}
			return nullptr;
		}

		template <class U, class T> 
		inline Uniform * insert(const String & name, const T & value)
		{
			if (!name) { return nullptr; }
			auto it { std::find_if(begin(), end(), [&](auto u)
			{
				return u && (u->name == name);
			}) };
			if (it == end())
			{
				m_uniforms.push_back(new U { name, value });
				return m_uniforms.back();
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool erase(Uniform * value)
		{
			return (value && value->name) && erase(value->name);
		}

		inline bool erase(const String & name)
		{
			iterator it { std::find_if(begin(), end(), [&](auto u)
			{
				return u && (u->name == name);
			}) };
			if (it != end())
			{
				if (*it) { delete (*it); }
				m_uniforms.erase(it);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U = typename Uniform> inline U * get(const String & name)
		{
			iterator it { std::find_if(begin(), end(), [&](auto u)
			{
				return u && (u->name == name);
			}) };
			return (it != end()) ? dynamic_cast<U *>(*it) : nullptr;
		}

		template <class U = typename Uniform> inline const U * get(const String & name) const
		{
			const_iterator it { std::find_if(cbegin(), cend(), [&](auto u)
			{
				return u && (u->name == name);
			}) };
			return (it != cend()) ? dynamic_cast<const U *>(*it) : nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U, class T> inline bool set(const String & name, const T & value)
		{
			if (auto * u { get<U>(name) })
			{
				u->data = value; 
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		inline auto uniforms()			-> List<Uniform *> &		{ return m_uniforms; }
		inline auto uniforms()	const	-> const List<Uniform *> &	{ return m_uniforms; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()				-> iterator			{ return m_uniforms.begin(); }
		inline auto begin()		const	-> const_iterator	{ return m_uniforms.begin(); }
		inline auto cbegin()	const	-> const_iterator	{ return m_uniforms.cbegin(); }
		inline auto end() 				-> iterator			{ return m_uniforms.end(); }
		inline auto end()		const	-> const_iterator	{ return m_uniforms.end(); }
		inline auto cend()		const	-> const_iterator	{ return m_uniforms.cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		List<Uniform *>	m_uniforms;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_MATERIAL_HPP_