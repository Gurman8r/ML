#ifndef _ML_REGISTRY_MANAGER_HPP_
#define _ML_REGISTRY_MANAGER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Disposable.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/String.hpp>

#define ML_Registry _ML RegistryManager::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ML_ENGINE_API Registry final
	{
		using type = typename detail::decay_t<T>;
		static bool s_registered;
		Registry() = default;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API RegistryManager : public I_Singleton<RegistryManager>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using generator = typename std::function<void *(void)>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool production(const String & name, hash_t code, generator fun)
		{
			m_codes.insert({ name, code });
			auto it { m_data.find(name) };
			return ((it == m_data.end())
				? (m_data.insert({ name, fun }).first->second != nullptr)
				: false
			);
		}

		template <class T, class Fun> inline bool production(Fun && fun)
		{
			return this->production(typeof<T>().name(), typeid(T).hash_code(), (generator)fun);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline void * generate(const String & name) const
		{
			auto it { m_data.find(name) };
			return (it != m_data.end()) ? it->second() : nullptr;
		}

		template <class T> inline T * generate() const
		{
			return static_cast<T *>(this->generate(typeof<T>().name()));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto codes() const -> const Tree<String, hash_t> & { return m_codes; }
		inline auto data() const -> const Tree<String, generator> & { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct I_Singleton<RegistryManager>;

		RegistryManager() = default;

		Tree<String, hash_t> m_codes;
		Tree<String, generator> m_data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REGISTRY_MANAGER_HPP_