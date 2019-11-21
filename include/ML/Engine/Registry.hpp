#ifndef _ML_REGISTRY_HPP_
#define _ML_REGISTRY_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/StringUtility.hpp>

#define ML_Registry ::ml::Registry<>::getInstance()

#define ML_REGISTER_EX(T, INFO, FUNC)			\
	static void * FUNC();						\
	bool Registry<T>::s_registered {			\
		ML_Registry.registrate<T>(INFO, FUNC)	\
	};											\
	void * FUNC()

#define ML_REGISTER(T, INFO) \
	ML_REGISTER_EX(T, INFO, ML_CONCAT(ML_FACTORY_, T))

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

	template <> struct ML_ENGINE_API Registry<> final : public Singleton<Registry<>>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Code = typename hash_t;
		using Name = typename String;
		using Info = typename String;
		using Func = typename std::function<void *()>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto codes() const -> const HashMap<Name, Code> & { return m_codes; }
		inline auto funcs() const -> const HashMap<Name, Func> & { return m_funcs; }
		inline auto infos() const -> const HashMap<Name, Info> & { return m_infos; }
		inline auto names() const -> const HashMap<Code, Name> & { return m_names; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * generate(Name const & name) const
		{
			const Func func { get_func(name) };
			return (func ? func() : nullptr);
		}

		inline void * generate(Code code) const
		{
			const Func func { get_func(code) };
			return (func ? func() : nullptr);
		}

		template <class T> inline T * generate() const
		{
			return static_cast<T *>(generate(typeof<T>::name));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool registrate(Name const & name, Info const & info, Code code, Func func)
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

		template <class T, class F> inline bool registrate(Info const & info, F && func)
		{
			return registrate(typeof<T>::name, info, typeof<T>::hash, (Func)func);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Code const * get_code(Name const & name) const
		{
			auto it { m_codes.find(name) };
			return ((it != m_codes.end()) ? &it->second : nullptr);
		}

		inline const Func get_func(Name const & name) const
		{
			auto it { m_funcs.find(name) };
			return ((it != m_funcs.end()) ? it->second : nullptr);
		}

		inline const Func get_func(Code code) const
		{
			auto it { m_names.find(code) };
			return ((it != m_names.end()) ? get_func(it->second) : nullptr);
		}

		inline String const *	get_info(Name const & name) const
		{
			auto it { m_infos.find(name) };
			return ((it != m_infos.end()) ? &it->second : nullptr);
		}

		inline String const * get_info(Code code) const
		{
			auto it { m_names.find(code) };
			return ((it != m_names.end()) ? get_info(it->second) : nullptr);
		}

		inline String const * get_name(Code code) const
		{
			auto it { m_names.find(code) };
			return ((it != m_names.end()) ? &it->second : nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend Singleton<Registry<>>;
		Registry() : m_codes(), m_funcs(), m_infos(), m_names() {}
		~Registry() {}
		HashMap<String, Code>	m_codes; // 
		HashMap<String, Func>	m_funcs; // 
		HashMap<String, String> m_infos; // 
		HashMap<Code,	String> m_names; // 
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REGISTRY_HPP_