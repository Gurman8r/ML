#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <ML/Graphics/Shader.hpp>
#include <ML/Graphics/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Materials are collections of uniforms
	struct ML_GRAPHICS_API Material final : public Trackable, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename List<Uniform *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Material();
		Material(const Material & copy);
		~Material();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename);
		bool loadFromFile(const String & filename, const Map<String, Texture *> * textures);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline base_type::iterator find(const String & name)
		{
			return std::find_if(this->begin(), this->end(), [&](auto u)
			{
				return (u && u->getName() == name);
			});
		}

		inline base_type::const_iterator find(const String & name) const
		{
			return std::find_if(this->cbegin(), this->cend(), [&](auto u)
			{
				return (u && u->getName() == name);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Uniform * insert(Uniform * value)
		{
			if (!value) { return nullptr; }
			if (auto it{ this->find(value->getName()) }; it == this->end())
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
			if (auto it{ this->find(value->getName()) }; it == this->end())
			{
				m_uniforms.push_back(new U { name, value });
				return m_uniforms.back();
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool erase(Uniform * value)
		{
			return (value && value->getName()) && erase(value->getName());
		}

		inline bool erase(const String & name)
		{
			if (auto it{ this->find(name) }; it != this->end())
			{
				if (*it) { delete (*it); }
				m_uniforms.erase(it);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class U = typename Uniform
		> inline U * get(const String & name)
		{
			auto it { this->find(name) };
			return (it != this->end()) ? dynamic_cast<U *>(*it) : nullptr;
		}

		template <
			class U = typename Uniform
		> inline U const * get(const String & name) const
		{
			auto it { this->find(name) };
			return (it != this->cend()) ? dynamic_cast<U const *>(*it) : nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U, class T> inline bool set(const String & name, const T & value)
		{
			if (auto u { this->get<U>(name) })
			{
				u->setData(value);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool rename(const String & from, const String & to)
		{
			auto it{ this->find(from) };
			if ((it != end()) && (to && (this->find(to) == this->end())))
			{
				(*it)->setName(to);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool empty() const { return m_uniforms.empty(); }
		
		inline auto size() const -> size_t { return m_uniforms.size(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto begin()			-> base_type::iterator			{ return m_uniforms.begin(); }
		inline auto begin() const	-> base_type::const_iterator	{ return m_uniforms.begin(); }
		inline auto cbegin() const	-> base_type::const_iterator	{ return m_uniforms.cbegin(); }
		inline auto end()			-> base_type::iterator			{ return m_uniforms.end(); }
		inline auto end() const		-> base_type::const_iterator	{ return m_uniforms.end(); }
		inline auto cend() const	-> base_type::const_iterator	{ return m_uniforms.cend(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		base_type	m_uniforms;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_MATERIAL_HPP_