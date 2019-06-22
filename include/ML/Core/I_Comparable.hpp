#ifndef _ML_I_COMPARABLE_HPP_
#define _ML_I_COMPARABLE_HPP_

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	template <class T>
	class I_Comparable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		using comp_type = typename T;
		using self_type = typename I_Comparable<comp_type>;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		virtual bool equals(const comp_type & value) const = 0;
		virtual bool lessThan(const comp_type & value) const = 0;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline bool notEquals(const comp_type & value) const
		{
			return !equals(value);
		}

		inline bool greaterThan(const comp_type & value) const
		{
			return !lessThan(value);
		}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline friend bool operator==(const self_type & lhs, const comp_type & rhs)
		{
			return lhs.equals(rhs);
		}
		
		inline friend bool operator!=(const self_type & lhs, const comp_type & rhs)
		{
			return lhs.notEquals(rhs);
		}
		
		inline friend bool operator >(const self_type & lhs, const comp_type & rhs)
		{
			return lhs.greaterThan(rhs);
		}
		
		inline friend bool operator <(const self_type & lhs, const comp_type & rhs)
		{
			return lhs.lessThan(rhs);
		}

		inline friend bool operator>=(const self_type & lhs, const comp_type & rhs)
		{
			return (lhs.equals(rhs) || lhs.greaterThan(rhs));
		}

		inline friend bool operator<=(const self_type & lhs, const comp_type & rhs)
		{
			return (lhs.equals(rhs) || lhs.lessThan(rhs));
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_COMPARABLE_HPP_