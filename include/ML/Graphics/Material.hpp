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

		using iterator					= typename Map<String, Uniform *>::iterator;
		using const_iterator			= typename Map<String, Uniform *>::const_iterator;
		using reverse_iterator			= typename Map<String, Uniform *>::reverse_iterator;
		using const_reverse_iterator	= typename Map<String, Uniform *>::const_reverse_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Material();
		Material(const Shader * shader);
		Material(const Shader * shader, Map<String, Uniform *> && uniforms);
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

		inline Uniform * insert(Uniform * value)
		{
			if (value && (m_uniforms.find(value->name) == end()))
			{
				return m_uniforms.insert({ value->name, std::move(value) }).first->second;
			}
			return nullptr;
		}

		template <class U, class T> 
		inline Uniform * insert(const String & name, const T & value)
		{
			if (name && (m_uniforms.find(name) == end()))
			{
				return m_uniforms.insert({ name, new U { name, value } }).first->second;
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool erase(Uniform * value)
		{
			return value && erase(value->name);
		}

		inline bool erase(const String & name)
		{
			auto it { m_uniforms.find(name) };
			if (it != end())
			{
				if (it->second) { delete it->second; }
				m_uniforms.erase(it);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U = typename Uniform> inline U * get(const String & name)
		{
			auto it { m_uniforms.find(name) };
			return (it != end()) ? dynamic_cast<U *>(it->second) : nullptr;
		}

		template <class U = typename Uniform> inline const U * get(const String & name) const
		{
			auto it { m_uniforms.find(name) };
			return (it != cend()) ? dynamic_cast<const U *>(it->second) : nullptr;
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

		inline auto shader()			-> const Shader	*& { return m_shader; }
		inline auto shader()	const	-> const Shader	* { return m_shader; }
		inline auto uniforms()			-> Map<String, Uniform *> & { return m_uniforms; }
		inline auto uniforms()	const	-> const Map<String, Uniform *> & { return m_uniforms; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()				-> iterator			{ return m_uniforms.begin(); }
		inline auto begin()		const	-> const_iterator	{ return m_uniforms.begin(); }
		inline auto cbegin()	const	-> const_iterator	{ return m_uniforms.cbegin(); }
		inline auto end()				-> iterator			{ return m_uniforms.end(); }
		inline auto end()		const	-> const_iterator	{ return m_uniforms.end(); }
		inline auto cend()		const	-> const_iterator	{ return m_uniforms.cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const Shader *	m_shader;
		Map<String, Uniform *>	m_uniforms;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_MATERIAL_HPP_