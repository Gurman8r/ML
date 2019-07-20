#ifndef _ML_ASSET_HPP_
#define _ML_ASSET_HPP_

#include <ML/Engine/Content.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	// Wrapper around a pointer to something in Content
	template <
		class T
	> struct Asset final : public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using self_type			= typename Asset<value_type>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		String name;
		mutable pointer	data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Asset()
			: name(String())
			, data(nullptr)
		{
		}

		explicit Asset(const String & name)
			: name(name)
			, data(nullptr)
		{
		}

		template <
			class U
		> explicit Asset(const String & name, U * data)
			: name(name)
			, data(static_cast<pointer>(data))
		{
			static_assert(std::is_base_of<I_Newable, U>::value,
				"Asset containers must contain I_Newable objects."
			);
			static_assert(!std::is_base_of<Asset, U>::value, 
				"Asset containers cannot contain other asset containers."
			);
		}

		Asset(const self_type & copy)
			: name(copy.name)
			, data(copy.data)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline pointer get() 
		{ 
			if (!this->data && this->name)
			{
				this->data = ML_Content.get<value_type>(this->name);
			}
			return static_cast<pointer>(this->data);
		}

		inline const_pointer get() const 
		{
			if (!this->data && this->name)
			{
				this->data = ML_Content.get<value_type>(this->name);
			}
			return static_cast<const_pointer>(this->data);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator			bool()			const	{ return this->get(); }
		inline operator			pointer()				{ return this->get(); }
		inline operator			const_pointer() const	{ return this->get(); }
		inline pointer			operator->()			{ return this->get(); }
		inline const_pointer	operator->()	const	{ return this->get(); }
		inline reference		operator*()				{ return (*this->get()); }
		inline const_reference	operator*()		const	{ return (*this->get()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_HPP_