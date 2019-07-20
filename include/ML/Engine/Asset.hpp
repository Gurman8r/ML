#ifndef _ML_ASSET_HPP_
#define _ML_ASSET_HPP_

#include <ML/Core/I_Newable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
	// Used to store a single I_Newable within Content
	struct AssetBase 
		: public I_Newable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename I_Newable;
		using self_type			= typename AssetBase;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		enum Flags { None };

		/* * * * * * * * * * * * * * * * * * * * */

		pointer	obj;
		Flags	flags;

		/* * * * * * * * * * * * * * * * * * * * */

		AssetBase() = delete;

		template <
			class T
		> explicit AssetBase(T * obj, Flags flags)
			: obj	{ static_cast<pointer>(obj) }
			, flags { flags }
		{
			static_assert(std::is_base_of<I_Newable, T>::value,
				"Asset containers must contain I_Newable objects."
			);
			static_assert(!std::is_base_of<AssetBase, T>::value, 
				"Asset containers cannot contain other asset containers."
			);
		}

		virtual ~AssetBase()
		{ 
			if (*this)
			{
				delete this->obj;
				this->obj = nullptr;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * */
		
		inline operator bool() const 
		{
			return (this->obj);
		}

		template <class T> inline T * as()
		{
			return static_cast<T *>(this->obj);
		}

		template <class T> inline const T * as() const
		{
			return static_cast<const T *>(this->obj);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Associates an AssetBase with a type
	template <
		class T
	> struct Asset final : public AssetBase
	{
		/* * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using self_type			= typename Asset<value_type>;
		using base_type			= typename AssetBase;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class U
		> explicit Asset(U * obj, Flags flags)
			: base_type { obj, flags }
		{
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline pointer		 get()				{ return this->as<value_type>(); }
		inline const_pointer get()	const		{ return this->as<value_type>(); }
		inline pointer		 operator->()		{ return this->get(); }
		inline pointer		 operator *()		{ return this->get(); }
		inline const_pointer operator->() const	{ return this->get(); }
		inline const_pointer operator *() const	{ return this->get(); }

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class U
		> static inline self_type * cast(U * value)
		{
			return reinterpret_cast<self_type *>(value);
		}

		template <
			class U
		> static inline const self_type * cast(const U * value)
		{
			return reinterpret_cast<const self_type *>(value);
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_HPP_