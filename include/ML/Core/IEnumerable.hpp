#ifndef _ML_I_ENUMERABLE_HPP_
#define _ML_I_ENUMERABLE_HPP_

#include <ML/Core/Iterator.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// Supports simple iteration of a fixed array
	template <
		class T,
		size_t N
	> class IEnumerable
	{
	public:
		using value_type			= typename T;
		using array_type			= typename T[N];

		using pointer				= typename value_type *;
		using const_pointer			= typename const value_type *;
		using reference				= typename value_type &;
		using const_reference		= typename const value_type &;

		using iterator				= typename std::_Array_iterator<T, N>;
		using const_iterator		= typename std::_Array_const_iterator<T, N>;
		using reverse_iterator		= typename std::reverse_iterator<iterator>;
		using const_reverse_iterator= typename std::reverse_iterator<const_iterator>;

	public:
		IEnumerable(pointer data)
			: m_data(data)
		{
		}

		virtual ~IEnumerable() {}

	public:
		inline iterator					begin()				{ return iterator(m_data, 0); }
		inline iterator					end()				{ return iterator(m_data, N); }
		inline const_iterator			begin()		const	{ return const_iterator(m_data, 0); }
		inline const_iterator			end()		const	{ return const_iterator(m_data, N); }
		inline const_iterator			cbegin()	const	{ return begin(); }
		inline const_iterator			cend()		const	{ return end(); }
		inline reverse_iterator			rbegin()			{ return reverse_iterator(end()); }
		inline reverse_iterator			rend()				{ return reverse_iterator(begin()); }
		inline const_reverse_iterator	rbegin()	const	{ return const_reverse_iterator(end()); }
		inline const_reverse_iterator	rend()		const	{ return const_reverse_iterator(begin()); }
		inline const_reverse_iterator	crbegin()	const	{ return rbegin(); }
		inline const_reverse_iterator	crend()		const	{ return rend(); }

	private:
		pointer m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_ENUMERABLE_HPP_