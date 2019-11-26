#ifndef _ML_ASSET_HPP_
#define _ML_ASSET_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/MemoryTracker.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API Asset final
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		StringView	name;
		typeof<>	type;
		size_t		size;
		Trackable * data;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		constexpr Asset() noexcept
			: name{ "" }
			, type{ typeof<>{} }
			, size{ 0 }
			, data{ nullptr }
		{
		}

		template <class T>
		constexpr Asset(StringView const & name, typeof<T> const & type) noexcept
			: name{ name }
			, type{ type }
			, size{ sizeof(T) }
			, data{ nullptr }
		{
		}

		constexpr Asset(Asset const & copy) noexcept
			: name{ copy.name }
			, type{ copy.type }
			, size{ copy.size }
			, data{ copy.data }
		{
		}

		constexpr Asset(Asset && copy) noexcept
			: name{ std::move(copy.name) }
			, type{ std::move(copy.type) }
			, size{ std::move(copy.size) }
			, data{ std::move(copy.data) }
		{
			copy.data = nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline Asset & swap(Asset & other)
		{
			std::swap(this->name, other.name);
			std::swap(this->type, other.type);
			std::swap(this->size, other.size);
			std::swap(this->data, other.data);
			return (*this);
		}

		inline Asset & operator=(Asset const & other)
		{
			Asset temp{ other };
			return this->swap(temp);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_HPP_