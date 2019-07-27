#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <ML/Graphics/Shader.hpp>
#include <ML/Graphics/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Material is a shader associated with a collection of uniforms
	struct ML_GRAPHICS_API Material final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using iterator			= typename List<Uni *>::iterator;
		using const_iterator	= typename List<Uni *>::const_iterator;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Material();
		Material(const Shader * shader);
		Material(const Shader * shader, const List<Uni *> & uniforms);
		~Material();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadFromFile(const String & filename, const Tree<String, Texture *> * textures);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const Material & bind(bool bindTextures = true) const;
		const Material & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T = typename Uni
		> inline T * get(const String & name)
		{
			iterator it = std::find_if(begin(), end(), [&](auto && u)
			{
				return u && (u->name == name);
			});
			return (it != end()) ? dynamic_cast<T *>(*it) : nullptr;
		}

		template <
			class T = typename Uni
		> inline const T * get(const String & name) const
		{
			const_iterator it = std::find_if(cbegin(), cend(), [&](auto && u)
			{
				return u && (u->name == name);
			});
			return (it != cend()) ? dynamic_cast<const T *>(*it) : nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Uni * add(Uni * value)
		{
			if (value && 
				value->name && 
				!get(value->name) &&
				(std::find(begin(), end(), value) == end()))
			{
				m_uniforms.push_back(value);
				return value;
			}
			return nullptr;
		}

		inline bool erase(const String & name)
		{
			iterator it = std::find_if(begin(), end(), [&](auto && u)
			{
				return u && (u->name == name);
			});
			if (it != end())
			{
				if (*it) delete (*it);
				m_uniforms.erase(it);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto shader()			-> const Shader	* &		{ return m_shader; }
		inline auto shader()	const	-> const Shader	*		{ return m_shader; }
		inline auto uniforms()			-> List<Uni *> &		{ return m_uniforms; }
		inline auto uniforms()	const	-> const List<Uni *> &	{ return m_uniforms; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()			-> iterator			{ return m_uniforms.begin(); }
		inline auto begin() const	-> const_iterator	{ return m_uniforms.begin(); }
		inline auto cbegin() const	-> const_iterator	{ return m_uniforms.cbegin(); }
		inline auto end()			-> iterator			{ return m_uniforms.end(); }
		inline auto end() const		-> const_iterator	{ return m_uniforms.end(); }
		inline auto cend() const	-> const_iterator	{ return m_uniforms.cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const Shader *	m_shader;
		List<Uni *> m_uniforms;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_MATERIAL_HPP_
