#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <ML/Graphics/Shader.hpp>
#include <ML/Graphics/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Material final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Material();
		Material(const Shader * shader);
		Material(const Shader * shader, const List<Uniform *> & uniforms);
		~Material();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void apply(const Uniform * value) const;
		bool bind() const;
		void unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Uniform * findUniform(const String & name)
		{
			auto it = std::find_if(m_uniforms.begin(), m_uniforms.end(), [&](auto && u)
			{
				return u && (u->name == name);
			});
			return (it != m_uniforms.end()) ? (*it) : nullptr;
		}

		inline const Uniform * findUniform(const String & name) const
		{
			auto it = std::find_if(m_uniforms.cbegin(), m_uniforms.cend(), [&](auto && u)
			{
				return u && (u->name == name);
			});
			return (it != m_uniforms.cend()) ? (*it) : nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T>
		inline T * getUniform(const String & name)
		{
			return dynamic_cast<T *>(this->findUniform(name));
		}

		template <class T>
		inline const T * getUniform(const String & name) const
		{
			return dynamic_cast<const T *>(this->findUniform(name));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Uniform * addUniform(Uniform * value)
		{
			if (value && value->name && !this->findUniform(value->name))
			{
				m_uniforms.push_back(value);
				return value;
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto shader() const	-> const Shader	*			{ return m_shader; }
		inline auto uniforms() const-> const List<Uniform *> &	{ return m_uniforms; }
		inline auto shader()		-> const Shader	* &			{ return m_shader; }
		inline auto uniforms()		-> List<Uniform *> &		{ return m_uniforms; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const Shader *	m_shader;
		List<Uniform *> m_uniforms;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_MATERIAL_HPP_
