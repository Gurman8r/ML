#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <ML/Graphics/Shader.hpp>	

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Uniform;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_GRAPHICS_API Material final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using UniformList = typename List<Uniform *>;
		using UniformMap = typename Map<String, Uniform *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Material();
		Material(const Shader * shader);
		Material(const Shader * shader, const UniformList & uniforms);
		Material(const Material & copy);
		~Material();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void apply(const Uniform * value) const;
		bool bind() const;
		void unbind() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool hasUniform(const String & name) const
		{
			return m_uniforms.find(name) != m_uniforms.end();
		}

		template <class T>
		inline const T * getUniform(const String & name) const
		{
			UniformMap::const_iterator it;
			return (((it = m_uniforms.find(name)) != m_uniforms.cend())
				? (dynamic_cast<const T *>(it->second))
				: (nullptr)
			);
		}

		template <class T>
		inline T * getUniform(const String & name)
		{
			UniformMap::iterator it;
			return (((it = m_uniforms.find(name)) != m_uniforms.end())
				? (dynamic_cast<T *>(it->second))
				: (nullptr)
			);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto shader() const	-> const Shader	*		{ return m_shader; }
		inline auto uniforms() const-> const UniformMap &	{ return m_uniforms; }
		inline auto shader()		-> const Shader	* &		{ return m_shader; }
		inline auto uniforms()		-> UniformMap &			{ return m_uniforms; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		const Shader *	m_shader;
		UniformMap		m_uniforms;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_MATERIAL_HPP_
