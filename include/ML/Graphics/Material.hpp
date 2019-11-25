#ifndef _ML_MATERIAL_HPP_
#define _ML_MATERIAL_HPP_

#include <ML/Graphics/Shader.hpp>
#include <ML/Graphics/Uniform.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Materials are collections of uniforms
	struct ML_GRAPHICS_API Material final : public Trackable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using base_type = typename std::vector<Uniform *>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Material();
		Material(Material const & copy);
		~Material() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();
		bool loadFromFile(String const & filename);
		bool loadFromFile(String const & filename, std::map<String, Texture *> const * textures);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline base_type::iterator find(String const & name)
		{
			return std::find_if(this->begin(), this->end(), [&](auto u)
			{
				return (u && u->name() == name);
			});
		}

		inline base_type::const_iterator find(String const & name) const
		{
			return std::find_if(this->cbegin(), this->cend(), [&](auto u)
			{
				return (u && u->name() == name);
			});
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Uniform * insert(Uniform * value)
		{
			if (!value || !value->name()) { return nullptr; }
			if (auto it{ this->find(value->name()) }; it == this->end())
			{
				m_uniforms.push_back(std::move(value));
				return m_uniforms.back();
			}
			return nullptr;
		}

		template <class U, class T> 
		inline Uniform * insert(String const & name, T const & value)
		{
			if (!name) { return nullptr; }
			if (auto it{ this->find(value->name()) }; it == this->end())
			{
				m_uniforms.push_back(new U { name, value });
				return m_uniforms.back();
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool erase(Uniform * value)
		{
			return (value && value->name()) && this->erase(value->name());
		}

		inline bool erase(String const & name)
		{
			if (auto it{ this->find(name) }; it != this->end())
			{
				Uniform *& ptr{ *it };
				if (ptr) delete ptr;
				m_uniforms.erase(it);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Uniform * get(String const & name)
		{
			if (auto it{ this->find(name) }; it != this->end())
			{
				return (*it);
			}
			return nullptr;
		}

		template <hash_t H> inline Uniform * get(String const & name)
		{
			if (auto u{ this->get(name) }; u && (u->get_self_id() == H))
			{
				return u;
			}
			return nullptr;
		}

		template <class U> inline U * get(String const & name)
		{
			if (auto u{ this->get<typeof<U>::hash>(name) })
			{
				return static_cast<U *>(u);
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Uniform const * get(String const & name) const
		{
			if (auto it{ this->find(name) }; it != this->cend())
			{
				return (*it);
			}
			return nullptr;
		}

		template<hash_t H> inline Uniform const * get(String const & name) const
		{
			if (auto u{ this->get(name) }; u && (u->get_self_id() == H))
			{
				return u;
			}
			return nullptr;
		}

		template <class U> inline U const * get(String const & name) const
		{
			if (auto u{ this->get<typeof<U>::hash>(name) })
			{
				return static_cast<U const *>(u);
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class U, class T> inline U * set(String const & name, T const & value)
		{
			if (auto u{ this->get<U>(name) })
			{
				return &(u->set(value));
			}
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Uniform const * rename(String const & from, String const & to)
		{
			if (auto u{ this->get(from) }; u && !this->get(to))
			{
				return &(u->rename(to));
			}
			return nullptr;
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

	private: base_type m_uniforms;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}
#endif // !_ML_MATERIAL_HPP_