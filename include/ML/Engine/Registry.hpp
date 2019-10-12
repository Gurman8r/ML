#ifndef _ML_REGISTRY_HPP_
#define _ML_REGISTRY_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/StringUtility.hpp>

#define ML_Registry ::ml::Registry<>::getInstance()

#define ML_REGISTER_EXT(TYPE, INFO, FUNC)		\
bool Registry<TYPE>::s_registered {				\
	ML_Registry.registrate<TYPE>(INFO, FUNC)	\
}

#define ML_REGISTER(TYPE, INFO)					\
static void * ml_factory_##TYPE();				\
ML_REGISTER_EXT(TYPE, INFO, ml_factory_##TYPE);	\
void * ml_factory_##TYPE()

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

		using Code = typename hash_t;
		using Name = typename String;
		using Info = typename String;
		using Func = typename std::function<void *(void)>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto codes() const -> const HashMap<Name, Code> & { return m_codes; }
		inline auto funcs() const -> const HashMap<Name, Func> & { return m_funcs; }
		inline auto infos() const -> const HashMap<Name, Info> & { return m_infos; }
		inline auto names() const -> const HashMap<Code, Name> & { return m_names; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * generate(const Name & name) const
		{
			const Func func { this->get_func(name) };
			return (func ? func() : nullptr);
		}

		inline void * generate(Code code) const
		{
			const Func func { this->get_func(code) };
			return (func ? func() : nullptr);
		}

		template <class T> inline T * generate() const
		{
			return static_cast<T *>(this->generate(typeof<T>().name));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool registrate(const Name & name, const Info & info, Code code, Func func)
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

		template <class T, class F> inline bool registrate(const Info & info, F && func)
		{
			return this->registrate(typeof<T>().name, info, typeof<T>().hash, (Func)func);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const Code * get_code(const Name & name) const
		{
			auto it { m_codes.find(name) };
			return ((it != m_codes.end()) ? &it->second : nullptr);
		}

		inline const Func get_func(const Name & name) const
		{
			auto it { m_funcs.find(name) };
			return ((it != m_funcs.end()) ? it->second : nullptr);
		}

		inline const Func get_func(Code code) const
		{
			auto it { m_names.find(code) };
			return ((it != m_names.end()) ? this->get_func(it->second) : nullptr);
		}

		inline const String * get_info(const Name & name) const
		{
			auto it { m_infos.find(name) };
			return ((it != m_infos.end()) ? &it->second : nullptr);
		}

		inline const String * get_info(Code code) const
		{
			auto it { m_names.find(code) };
			return ((it != m_names.end()) ? this->get_info(it->second) : nullptr);
		}

		inline const String * get_name(Code code) const
		{
			auto it { m_names.find(code) };
			return ((it != m_names.end()) ? &it->second : nullptr);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct I_Singleton<Registry<>>;
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