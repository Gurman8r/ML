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
		bool loadFromFile(const String & filename) override;
		bool loadFromFile(const String & filename, const Tree<String, Texture *> * textures);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		const Material & bind(bool bindTextures = true) const;
		const Material & unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class T = typename Uniform
		> inline T * get(const String & name)
		{
			iterator it = std::find_if(this->begin(), this->end(), [&](auto && u)
			{
				return u && (u->name == name);
			});
			return (it != this->end()) ? dynamic_cast<T *>(*it) : nullptr;
		}

		template <
			class T = typename Uniform
		> inline const T * get(const String & name) const
		{
			const_iterator it = std::find_if(this->cbegin(), this->cend(), [&](auto && u)
			{
				return u && (u->name == name);
			});
			return (it != this->cend()) ? dynamic_cast<const T *>(*it) : nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Uniform * add(Uniform * value)
		{
			if (value && 
				value->name && 
				!this->get(value->name) &&
				(std::find(this->begin(), this->end(), value) == this->end()))
			{
				m_uniforms.push_back(value);
				return value;
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool erase(Uniform * value)
		{
			return (value && value->name) && this->erase(value->name);
		}

		inline bool erase(const String & name)
		{
			iterator it = std::find_if(this->begin(), this->end(), [&](auto && u)
			{
				return u && (u->name == name);
			});
			if (it != this->end())
			{
				if (*it) delete (*it);
				m_uniforms.erase(it);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto shader()			-> const Shader	*&		{ return m_shader; }
		inline auto shader()	const	-> const Shader	*		{ return m_shader; }
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
		List<Uniform *>		m_uniforms;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_MATERIAL_HPP_
