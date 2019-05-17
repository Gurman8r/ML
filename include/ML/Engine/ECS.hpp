#ifndef _ML_ECS_HPP_
#define _ML_ECS_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/ITrackable.hpp>
#include <ML/Core/IDisposable.hpp>
#include <ML/Core/List.hpp>

#define ML_ECS ml::ECS::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API ECS final
		: public ITrackable
		, public IDisposable
		, public ISingleton<ECS>
	{
		friend class ISingleton<ECS>;

	public:
		using value_type	= typename void *;
		using data_map		= typename HashMap<size_t, value_type>;
		using type_map		= typename HashMap<size_t, data_map>;

	private:
		ECS();
		~ECS();

	public:
		bool dispose() override;

	public:
		void *	createAlloc(const size_t type, const size_t index, const size_t size);
		bool	freeAlloc(const size_t type, const size_t index, void * ptr);

	public:
		template <class Component>
		inline static size_t hash()
		{
			return (size_t)(&typeid(Component))->hash_code();
		}

	private:
		type_map m_cmp;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ECS_HPP_