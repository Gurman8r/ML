#ifndef _ML_REGISTRY_HPP_
#define _ML_REGISTRY_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/String.hpp>

#define ML_Registry ::ml::Registry<>::getInstance()

#define ML_FACTORY(TYPE) ML_CONCAT(ml_factory_, TYPE)

#define ML_REGISTER_EXT(TYPE, INFO, FUNC)		\
bool Registry<TYPE>::s_registered {				\
	ML_Registry.registrate<TYPE>(INFO, FUNC)	\
}

#define ML_REGISTER(TYPE, INFO)					\
static void * ML_FACTORY(TYPE)();				\
ML_REGISTER_EXT(TYPE, INFO, ML_FACTORY(TYPE));	\
void * ML_FACTORY(TYPE)()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct Registry;

	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ML_ENGINE_API Registry<T> final
	{
	private: Registry() = delete;
	protected: static bool s_registered;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	template <> struct ML_ENGINE_API Registry<> final : public I_Singleton<Registry<>>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Method = typename std::function<void *(void)>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto codes() const -> const Tree<String, hash_t> & { return m_codes; }
		inline auto funcs() const -> const Tree<String, Method> & { return m_funcs; }
		inline auto infos() const -> const Tree<String, String> & { return m_infos; }
		inline auto names() const -> const Tree<hash_t, String> & { return m_names; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * generate(const String & name) const
		{
			const Method func { get_func(name) };
			return (func ? func() : nullptr);
		}

		inline void * generate(hash_t code) const
		{
			const Method func { get_func(code) };
			return (func ? func() : nullptr);
		}

		template <class T> inline T * generate() const
		{
			return static_cast<T *>(generate(typeof<T>().name()));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool registrate(const String & name, const String & info, hash_t code, Method func)
		{
			if (m_funcs.find(name) == m_funcs.end())
			{
				m_codes.insert({ name, code });
				m_names.insert({ code, name });
				m_funcs.insert({ name, func });
				m_infos.insert({ name, info });
				return true;
			}
			return false;
		}

		template <class T, class F> inline bool registrate(const String & info, F && func)
		{
			return registrate(typeof<T>().name(), info, typeof<T>().hash(), (Method)func);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const hash_t * get_code(const String & name) const
		{
			Tree<String, hash_t>::const_iterator it { m_codes.find(name) };
			return ((it != m_codes.end()) ? &it->second : nullptr);
		}

		inline const Method get_func(const String & name) const
		{
			Tree<String, Method>::const_iterator it { m_funcs.find(name) };
			return ((it != m_funcs.end()) ? it->second : nullptr);
		}

		inline const Method get_func(hash_t code) const
		{
			Tree<hash_t, String>::const_iterator it { m_names.find(code) };
			return ((it != m_names.end()) ? this->get_func(it->second) : nullptr);
		}

		inline const String * get_info(const String & name) const
		{
			Tree<String, String>::const_iterator it { m_infos.find(name) };
			return ((it != m_infos.end()) ? &it->second : nullptr);
		}

		inline const String * get_info(hash_t code) const
		{
			Tree<hash_t, String>::const_iterator it { m_names.find(code) };
			return ((it != m_names.end()) ? this->get_info(it->second) : nullptr);
		}

		inline const String * get_name(hash_t code) const
		{
			Tree<hash_t, String>::const_iterator it { m_names.find(code) };
			return ((it != m_names.end()) ? &it->second : nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct I_Singleton<Registry<>>;
		Registry();
		Tree<String, hash_t> m_codes;
		Tree<String, Method> m_funcs;
		Tree<String, String> m_infos;
		Tree<hash_t, String> m_names;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REGISTRY_HPP_