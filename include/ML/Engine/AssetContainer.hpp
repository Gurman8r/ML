#ifndef _ML_ASSET_CONTAINER_HPP_
#define _ML_ASSET_CONTAINER_HPP_

#include <ML/Core/I_Newable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	// Used to store a single I_Newable within Content
	struct AssetContainer final
		: public I_Newable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename I_Newable;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		String	name;
		pointer obj;
		int32_t flags;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		AssetContainer() = delete;

		template <
			class T
		> explicit AssetContainer(const String & name, T * obj, int32_t flags = 0)
			: name	{ name }
			, obj	{ static_cast<pointer>(obj) }
			, flags { flags }
		{
			static_assert(
				std::is_base_of<I_Newable, T>::value,
				"Asset containers must contain I_Newable objects."
			);

			static_assert(
				!std::is_base_of<AssetContainer, T>::value, 
				"Cannot contstruct nested AssetContainers."
			);
		}

		~AssetContainer()
		{ 
			if (obj)
			{
				delete obj;
				obj = nullptr;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline pointer			operator->()		{ return obj; }
		inline pointer			operator *()		{ return obj; }
		inline const_pointer	operator->() const	{ return obj; }
		inline const_pointer	operator *() const	{ return obj; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_CONTAINER_HPP_