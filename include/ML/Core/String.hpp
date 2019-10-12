#ifndef _ML_STRING_HPP_
#define _ML_STRING_HPP_

#include <ML/Core/TypeOf.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// just a wrapper for std::basic_string<>
	template <class Ch> struct BasicString final
		: public std::basic_string<Ch, std::char_traits<Ch>, std::allocator<Ch>>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using value_type			= typename Ch;
		using traits_type			= typename std::char_traits<Ch>;
		using allocator_type		= typename std::allocator<Ch>;
		using self_type				= typename BasicString<Ch>;
		using base_type				= typename std::basic_string<Ch, traits_type, allocator_type>;
		using sstream_type			= typename std::basic_stringstream<Ch, traits_type, allocator_type>;
		using pointer				= typename base_type::pointer;
		using reference				= typename base_type::reference;
		using const_pointer			= typename base_type::const_pointer;
		using const_reference		= typename base_type::const_reference;
		using size_type				= typename base_type::size_type;
		using difference_type		= typename base_type::difference_type;
		using iterator				= typename base_type::iterator;
		using const_iterator		= typename base_type::const_iterator;
		using reverse_iterator		= typename base_type::reverse_iterator;
		using const_reverse_iterator= typename base_type::const_reverse_iterator;
		using alty_type				= typename base_type::_Alty;
		using alty_traits_type		= typename base_type::_Alty_traits;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		BasicString()
			: base_type()
		{
		}

		explicit BasicString(const allocator_type & alloc) noexcept
			: base_type(alloc)
		{
		}

		BasicString(base_type && value) noexcept
			: base_type(value)
		{
		}

		BasicString(base_type && value, const allocator_type & alloc)
			: base_type(value, alloc)
		{
		}
		
		BasicString(const base_type & value)
			: base_type(value)
		{
		}
		
		BasicString(const base_type & value, const allocator_type & alloc)
			: base_type(value, alloc)
		{
		}
		
		BasicString(const base_type & value, const size_type off, const allocator_type & alloc = allocator_type())
			: base_type(value, off, alloc)
		{
		}
		
		BasicString(const base_type & value, const size_type off, const size_type count, const allocator_type & alloc = allocator_type())
			: base_type(value, off, count, alloc)
		{
		}
		
		BasicString(const Ch * const value)
			: base_type(value)
		{
		}
		
		BasicString(const Ch * const value, const size_type count)
			: base_type(value, count)
		{
		}
		
		BasicString(const Ch * const value, const size_type count, const allocator_type & alloc)
			: base_type(value, count, alloc)
		{
		}
		
		BasicString(const size_type count, const Ch value)
			: base_type(count, value)
		{
		}
		
		BasicString(const size_type count, const Ch value, const allocator_type & alloc)
			: base_type(count, value, alloc)
		{
		}
		
		template <
			class Iter
		> BasicString(Iter begin, Iter end, std::input_iterator_tag)
			: base_type(begin, end, std::input_iterator_tag())
		{
		}
		
		template <
			class Iter
		> BasicString(Iter begin, Iter end, std::forward_iterator_tag)
			: base_type(begin, end, std::forward_iterator_tag())
		{
		}
		
		template <
			class Iter
		> BasicString(Iter begin, Iter end, const allocator_type & alloc = allocator_type())
			: base_type(begin, end, alloc)
		{
		}
		
		BasicString(Ch * const first, Ch * const last, std::random_access_iterator_tag)
			: base_type(first, last, std::random_access_iterator_tag())
		{
		}

		BasicString(const StringView & copy)
			: base_type(copy.str())
		{
		}
		
		~BasicString() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline operator bool() const
		{
			return !this->empty();
		}

		inline operator base_type &()
		{
			return static_cast<base_type &>(*this);
		}

		inline operator const base_type &() const
		{
			return static_cast<const base_type &>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline hash_t hash() const
		{
			return Hash { this->data(), this->size() };
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Arg0, class ... Args
		> inline self_type & format(const Arg0 & arg0, Args && ... args)
		{
			sstream_type ss; ss << arg0 << endl;

			int32_t sink[] = { 0, ((void)(ss << args << endl), 0)... }; (void)sink;

			return this->format(ss);
		}

		inline self_type & format(sstream_type & ss)
		{
			for (size_type i = 0; ss.good(); i++)
			{
				self_type line;
				if (std::getline(ss, line))
				{
					this->replaceAll(("{" + std::to_string(i) + "}"), line);
				}
			}
			return (*this);
		}

		template <
			class Arg0, class ... Args
		> inline self_type format(const Arg0 & arg0, Args && ... args) const
		{
			return self_type(*this).format(arg0, std::forward<Args>(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline self_type & replaceAll(const self_type & to_replace, const self_type & value)
		{
			if (!this->empty() && !to_replace.empty())
			{
				for (size_t i = 0; (i = this->find(to_replace, i)) != self_type::npos;)
				{
					this->replace(i, to_replace.size(), value);
					i += value.size();
				}
			}
			return (*this);
		}

		inline self_type replaceAll(const self_type & f, const self_type & r) const
		{
			return self_type(*this).replaceAll(f, r);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline self_type & trim()
		{
			auto is_whitespace = [&](Ch c)
			{
				return (!this->empty() && (
					(c == static_cast<Ch>(' ')) ||
					(c == static_cast<Ch>('\t'))
				));
			};
			while (is_whitespace(this->front())) this->erase(this->begin());
			while (is_whitespace(this->back()))  this->pop_back();
			return (*this);
		}

		inline self_type trim() const
		{
			return self_type(*this).trim();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	using String		= BasicString<char>;
	using W_String		= BasicString<wchar_t>;
	using U16_String	= BasicString<char16_t>;
	using U32_String	= BasicString<char32_t>;
	
	using SStream		= typename String::sstream_type;
	using W_SStream		= typename W_String::sstream_type;
	using U16_SStream	= typename U16_String::sstream_type;
	using U32_SStream	= typename U32_String::sstream_type;

	/* * * * * * * * * * * * * * * * * * * * */
}

template <class Ch> struct _STD hash<_ML BasicString<Ch>>
{
	inline _STD size_t operator()(const _ML BasicString<Ch> & value) const noexcept
	{
		return static_cast<_STD size_t>(value.hash());
	}
};

#endif // !_ML_STRING_HPP_