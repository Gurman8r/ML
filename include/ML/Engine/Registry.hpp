#ifndef _ML_REGISTRY_HPP_
#define _ML_REGISTRY_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/StringUtility.hpp>

#define ML_Registry ::ml::Registry<>::getInstance()

#define ML_REGISTER_EX(T, info, func)			\
	static Trackable * func();					\
	bool Registry<T>::s_registered {			\
		ML_Registry.registrate<T>(info, func)	\
	};											\
	Trackable * func()

#define ML_REGISTER(T, info) \
	ML_REGISTER_EX(T, info, ML_CONCAT(ML_FACTORY_, T))

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
		using Func = typename std::function<Trackable *()>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto codes() const -> const std::unordered_map<Name, Code> & { return m_codes; }
		inline auto funcs() const -> const std::unordered_map<Name, Func> & { return m_funcs; }
		inline auto infos() const -> const std::unordered_map<Name, Info> & { return m_infos; }
		inline auto names() const -> const std::unordered_map<Code, Name> & { return m_names; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Trackable * generate(Name const & name) const
		{
			Func const * func { this->get_func(name) };
			return func ? (*func)() : nullptr;
		}

		inline Trackable * generate(Code const & code) const
		{
			Func const * func { this->get_func(code) };
			return func ? (*func)() : nullptr;
		}

		template <class T> inline T * generate() const
		{
			return static_cast<T *>(this->generate(typeof<T>::name()));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool registrate(Name const & name, Info const & info, Code const & code, Func const & func)
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
			return this->registrate(typeof<T>::name(), info, typeof<T>::hash(), (Func)func);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Code const * get_code(Name const & name) const
		{
			auto const it { m_codes.find(name) };
			return ((it != m_codes.end()) ? &it->second : nullptr);
		}

		inline Func const * get_func(Name const & name) const
		{
			auto const it { m_funcs.find(name) };
			return ((it != m_funcs.end()) ? &it->second : nullptr);
		}

		inline Func const * get_func(Code code) const
		{
			auto const it { m_names.find(code) };
			return ((it != m_names.end()) ? this->get_func(it->second) : nullptr);
		}

		inline String const * get_info(Name const & name) const
		{
			auto const it { m_infos.find(name) };
			return ((it != m_infos.end()) ? &it->second : nullptr);
		}

		inline String const * get_info(Code code) const
		{
			auto const it { m_names.find(code) };
			return ((it != m_names.end()) ? this->get_info(it->second) : nullptr);
		}

		inline String const * get_name(Code code) const
		{
			auto const it { m_names.find(code) };
			return ((it != m_names.end()) ? &it->second : nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend Singleton<Registry<>>;

		Registry() 
			: m_codes(), m_funcs(), m_infos(), m_names() 
		{
		}
		
		~Registry() {}
		
		std::unordered_map<String, Code>	m_codes; // 
		std::unordered_map<String, Func>	m_funcs; // 
		std::unordered_map<String, String> m_infos; // 
		std::unordered_map<Code,	String> m_names; // 
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REGISTRY_HPP_