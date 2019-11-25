#ifndef _ML_STRING_HPP_
#define _ML_STRING_HPP_

#include <ML/Core/TypeOf.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// just a wrapper for _STD basic_string<>
	template <class Ch> struct BasicString final : public _STD basic_string<
		Ch, _STD char_traits<Ch>, _STD allocator<Ch>
	>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using value_type			= typename Ch;
		using self_type				= typename _ML BasicString<Ch>;
		using traits_type			= typename _STD char_traits<Ch>;
		using allocator_type		= typename _STD allocator<Ch>;
		using base_type				= typename _STD basic_string<Ch, traits_type, allocator_type>;
		using sstream_type			= typename _STD basic_stringstream<Ch, traits_type, allocator_type>;
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		BasicString()
			: base_type()
		{
		}

		explicit BasicString(allocator_type const & alloc) noexcept
			: base_type(alloc)
		{
		}

		BasicString(base_type && value) noexcept
			: base_type(value)
		{
		}

		BasicString(base_type && value, allocator_type const & alloc)
			: base_type(value, alloc)
		{
		}
		
		BasicString(base_type const & value)
			: base_type(value)
		{
		}
		
		BasicString(base_type const & value, allocator_type const & alloc)
			: base_type(value, alloc)
		{
		}
		
		BasicString(base_type const & value, const size_type off, allocator_type const & alloc = allocator_type())
			: base_type(value, off, alloc)
		{
		}
		
		BasicString(base_type const & value, const size_type off, const size_type count, allocator_type const & alloc = allocator_type())
			: base_type(value, off, count, alloc)
		{
		}
		
		BasicString(Ch const * const value)
			: base_type(value)
		{
		}
		
		BasicString(Ch const * const value, const size_type count)
			: base_type(value, count)
		{
		}
		
		BasicString(Ch const * const value, const size_type count, allocator_type const & alloc)
			: base_type(value, count, alloc)
		{
		}
		
		BasicString(const size_type count, const Ch value)
			: base_type(count, value)
		{
		}
		
		BasicString(const size_type count, const Ch value, allocator_type const & alloc)
			: base_type(count, value, alloc)
		{
		}
		
		template <
			class Iter
		> BasicString(Iter begin, Iter end, _STD input_iterator_tag)
			: base_type(begin, end, _STD input_iterator_tag())
		{
		}
		
		template <
			class Iter
		> BasicString(Iter begin, Iter end, _STD forward_iterator_tag)
			: base_type(begin, end, _STD forward_iterator_tag())
		{
		}
		
		template <
			class Iter
		> BasicString(Iter begin, Iter end, allocator_type const & alloc = allocator_type())
			: base_type(begin, end, alloc)
		{
		}
		
		BasicString(Ch * const first, Ch * const last, _STD random_access_iterator_tag)
			: base_type(first, last, _STD random_access_iterator_tag())
		{
		}

		BasicString(StringView const & copy)
			: base_type(copy.str())
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline operator bool() const
		{
			return !this->empty();
		}

		inline operator base_type &()
		{
			return static_cast<base_type &>(*this);
		}

		inline operator base_type const &() const
		{
			return static_cast<base_type const &>(*this);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline hash_t hash() const
		{
			return Hash(this->data(), this->size());
		}
		
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class Arg0, class ... Args
		> inline self_type & format(Arg0 const & arg0, Args && ... args)
		{
			sstream_type ss {}; ss << arg0 << '\n';
			
			int32_t sink[] = { 0, ((void)(ss << args << '\n'), 0)... }; (void)sink;
			
			return this->format(ss);
		}

		inline self_type & format(sstream_type & ss)
		{
			for (size_type i = 0; ss.good(); i++)
			{
				self_type line;
				if (_STD getline(ss, line))
				{
					this->replace_all(("{" + _STD to_string(i) + "}"), line);
				}
			}
			return (*this);
		}

		template <
			class Arg0, class ... Args
		> inline self_type format(Arg0 const & arg0, Args && ... args) const
		{
			return self_type{ *this }.format(arg0, _STD forward<Args>(args)...);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline self_type & replace_all(self_type const & to_replace, self_type const & value)
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

		inline self_type replace_all(self_type const & f, self_type const & r) const
		{
			return self_type{ *this }.replace_all(f, r);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline self_type & remove_all(self_type const & to_remove)
		{
			return this->replace_all(to_remove, self_type {});
		}

		inline self_type remove_all(self_type const & to_remove) const
		{
			return self_type{ *this }.remove_all(to_remove);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline self_type & trim(Ch c)
		{
			while (!this->empty() && (this->front() == c)) { this->erase(this->begin()); }
			
			while (!this->empty() && (this->back() == c)) { this->pop_back(); }
			
			return (*this);
		}
		
		inline self_type & trim()
		{
			return (*this)
				.trim(static_cast<Ch>(' '))
				.trim(static_cast<Ch>('\t'))
				.trim(static_cast<Ch>('\r'))
				.trim(static_cast<Ch>('\n'))
				;
		}

		inline self_type trim() const
		{
			return self_type{ *this }.trim();
		}

		inline self_type trim(Ch c) const
		{
			return self_type { *this }.trim(c);
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