#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <ML/Graphics/Shader.hpp>	

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct uni_base;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_GRAPHICS_API Material final
		: public INewable
		, public IDisposable
		, public IReadable
	{
	public:
		using UniformList = typename List<uni_base *>;
		using UniformMap = typename Map<String, uni_base *>;

	public:
		Material();
		Material(const Shader * shader);
		Material(const Shader * shader, const UniformList & uniforms);
		Material(const Material & copy);
		~Material();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

	public:
		void apply(const uni_base * value) const;
		bool bind() const;
		void unbind() const;

	public:
		inline bool find_any(const String & name) const
		{
			return (m_uniforms.find(name) != m_uniforms.end());
		}

		template <class T>
		inline const T * find_uni(const String & name) const
		{
			UniformMap::const_iterator it;
			return (((it = m_uniforms.find(name)) != m_uniforms.cend())
				? (dynamic_cast<const T *>(it->second))
				: (nullptr)
			);
		}

		template <class T>
		inline T * find_uni(const String & name)
		{
			UniformMap::iterator it;
			return (((it = m_uniforms.find(name)) != m_uniforms.end())
				? (dynamic_cast<T *>(it->second))
				: (nullptr)
			);
		}

	public:
		inline const Shader	*		shader()	const	{ return m_shader; }
		inline const UniformMap &	uniforms()	const	{ return m_uniforms; }

	public:
		inline const Shader	* &		shader()			{ return m_shader; }
		inline UniformMap &			uniforms()			{ return m_uniforms; }

	private:
		const Shader *	m_shader;
		UniformMap		m_uniforms;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_MATERIAL_HPP_
