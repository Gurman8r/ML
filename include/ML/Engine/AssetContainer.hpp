#ifndef _ML_ASSET_CONTAINER_HPP_
#define _ML_ASSET_CONTAINER_HPP_

#include <ML/Core/I_Newable.hpp>

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

		int32_t flags	{ 0 };
		pointer object	{ nullptr };

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		AssetContainer() = delete;

		template <
			class T
		> explicit AssetContainer(int32_t flags, T * object)
			: flags  { flags }
			, object { static_cast<pointer>(object) }
		{
		}

		template <
			class T
		> explicit AssetContainer(T * object)
			: AssetContainer { 0, object }
		{
		}

		~AssetContainer()
		{ 
			if (object)
			{
				delete object;
				object = nullptr;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline pointer			operator->()		{ return object; }
		inline pointer			operator *()		{ return object; }
		inline const_pointer	operator->() const	{ return object; }
		inline const_pointer	operator *() const	{ return object; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_CONTAINER_HPP_