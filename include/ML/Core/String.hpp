#ifndef _ML_STRING_HPP_
#define _ML_STRING_HPP_

#include <ML/Core/C_String.hpp>

namespace ml
{
	// just a wrapper for std::basic_string<>
	template <
		class Elem,
		class Traits = typename CharTraits<Elem>,
		class Alloc  = typename Allocator<Elem>
	> struct BasicString : public std::basic_string<Elem, Traits, Alloc>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		using value_type			= typename Elem;
		using traits_type			= typename Traits;
		using allocator_type		= typename Alloc;
		using self_type				= typename BasicString<value_type, traits_type, allocator_type>;
		using base_type				= typename std::basic_string<value_type, traits_type, allocator_type>;
		using sstream_type			= typename std::basic_stringstream<value_type, traits_type, allocator_type>;
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
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
		
		BasicString(const value_type * const value)
			: base_type(value)
		{
		}
		
		BasicString(const value_type * const value, const size_type count)
			: base_type(value, count)
		{
		}
		
		BasicString(const value_type * const value, const size_type count, const allocator_type & alloc)
			: base_type(value, count, alloc)
		{
		}
		
		BasicString(const size_type count, const value_type value)
			: base_type(count, value)
		{
		}
		
		BasicString(const size_type count, const value_type value, const allocator_type & alloc)
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
		
		BasicString(value_type * const first, value_type * const last, std::random_access_iterator_tag)
			: base_type(first, last, std::random_access_iterator_tag())
		{
		}
		
		virtual ~BasicString() noexcept {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		template <
			class Arg0, class ... Args
		> inline self_type & format(const Arg0 & arg0, Args && ... args)
		{
			sstream_type ss; ss << arg0 << endl;

			int32_t sink[] = { 0, ((void)(ss << args << endl), 0)... }; (void)sink;

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		inline self_type & trim()
		{
			auto is_whitespace = [&](const_reference c)
			{
				return !this->empty() && ((c == ' ') || (c == '\t'));
			};
			while (is_whitespace(this->front())) this->erase(this->begin());
			while (is_whitespace(this->back()))  this->pop_back();
			return (*this);
		}

		inline self_type trim() const
		{
			return self_type(*this).trim();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace std
{
	template <
		class Elem, class Traits, class Alloc
	> struct hash<_ML BasicString<Elem, Traits, Alloc>>
	{
		using argument_type = _ML BasicString<Elem, Traits, Alloc>;

		inline _ML hash_t operator()(const argument_type & value) const noexcept
		{
			return _ML Hash(value.size(), value.data());
		}
	};
}

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	using String	= BasicString<char>;
	using WString	= BasicString<wchar_t>;
	using String16	= BasicString<char16_t>;
	using String32	= BasicString<char32_t>;
	
	using SStream	= typename String::sstream_type;
	using WSStream	= typename WString::sstream_type;
	using SStream16 = typename String16::sstream_type;
	using SStream32 = typename String32::sstream_type;
}

/* * * * * * * * * * * * * * * * * * * * */

#endif // !_ML_STRING_HPP_