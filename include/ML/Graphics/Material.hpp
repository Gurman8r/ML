#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <ML/Graphics/Shader.hpp>
#include <ML/Graphics/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Material is a shader associated with a collection of uniforms
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
		Material(const Shader * shader);
		Material(const Shader * shader, List<Uniform *> && uniforms);
		Material(const Material & copy);
		~Material();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename);
		bool loadFromFile(const String & filename, const Map<String, Texture *> * textures);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Material & setShader(const Shader * value);

		const Material & bind() const;

		const Material & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline iterator find(Uniform * value)
		{
			return std::find(begin(), end(), value);
		}

		inline const_iterator find(Uniform * value) const
		{
			return std::find(cbegin(), cend(), value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool contains(Uniform * value) const
		{
			return ((value && value->name) && get(value->name) && (find(value) != cend()));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Uniform * insert(Uniform * value)
		{
			if (value && !contains(value))
			{
				m_uniforms.push_back(value);
				return m_uniforms.back();
			}
			return nullptr;
		}

		template <class U, class T> 
		inline Uniform * insert(const String & name, const T & value)
		{
			if (name && !get(name))
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
			iterator it { std::find_if(begin(), end(), [&](auto && u)
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
			iterator it { std::find_if(begin(), end(), [&](auto && u)
			{
				return u && (u->name == name);
			}) };
			return (it != end()) ? dynamic_cast<U *>(*it) : nullptr;
		}

		template <class U = typename Uniform> inline const U * get(const String & name) const
		{
			const_iterator it { std::find_if(cbegin(), cend(), [&](auto && u)
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

		inline auto shader()			-> const Shader	*&			{ return m_shader; }
		inline auto shader()	const	-> const Shader	*			{ return m_shader; }
		inline auto uniforms()			-> List<Uniform *> &		{ return m_uniforms; }
		inline auto uniforms()	const	-> const List<Uniform *> &	{ return m_uniforms; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()				-> iterator					{ return m_uniforms.begin(); }
		inline auto begin()		const	-> const_iterator			{ return m_uniforms.begin(); }
		inline auto cbegin()	const	-> const_iterator			{ return m_uniforms.cbegin(); }
		inline auto end()				-> iterator					{ return m_uniforms.end(); }
		inline auto end()		const	-> const_iterator			{ return m_uniforms.end(); }
		inline auto cend()		const	-> const_iterator			{ return m_uniforms.cend(); }
		inline auto rbegin()			-> reverse_iterator			{ return m_uniforms.rbegin(); }
		inline auto rbegin()	const	-> const_reverse_iterator	{ return m_uniforms.rbegin(); }
		inline auto rend()				-> reverse_iterator			{ return m_uniforms.rend(); }
		inline auto rend()		const	-> const_reverse_iterator	{ return m_uniforms.rend(); }
		inline auto crbegin()	const	-> const_reverse_iterator	{ return m_uniforms.crbegin(); }
		inline auto crend()		const	-> const_reverse_iterator	{ return m_uniforms.crend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const Shader *	m_shader;
		List<Uniform *>	m_uniforms;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_MATERIAL_HPP_