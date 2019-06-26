#include <ML/Script/Arguments.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Arguments::Arguments()
		: m_values()
	{
	}
	
	Arguments::Arguments(int32_t argc, char ** argv)
		: m_values()
	{
		for (size_t i = 0, imax = (size_t)argc; i < imax; i++)
		{
			push_back("\"" + String(argv[i]) + "\"");
		}
	}
	
	Arguments::Arguments(const String & value)
		: m_values({ value })
	{
	}
	
	Arguments::Arguments(const List<String> & values)
		: m_values(values)
	{
	}
	
	Arguments::Arguments(const std::initializer_list<String>& values)
		: m_values(values)
	{
	}
	
	Arguments::Arguments(const String & value, const String & delim)
		: m_values(StringUtility::Split(value, delim))
	{
	}
	
	Arguments::Arguments(const Arguments & copy)
		: m_values(copy.m_values)
	{
	}
	
	Arguments::~Arguments()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	String Arguments::pop()
	{
		return pop_front().front();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	String Arguments::at(size_t index) const
	{
		return (*this)[index];
	}
	
	String Arguments::back() const
	{
		if (!empty())
		{
			return m_values.back();
		}
		return String();
	}
	
	String Arguments::front() const
	{
		if (!empty())
		{
			return m_values.front();
		}
		return String();
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	size_t Arguments::count(const String & value) const
	{
		return count(begin(), end(), value);
	}
	
	size_t Arguments::count(const_iterator first, const_iterator last, const String & val) const
	{
		difference_type ret = 0;

		while (first != last)
		{
			if (*first == val)
			{
				++ret;
			}

			++first;
		}

		return ret;
	}
	
	size_t Arguments::indexOf(const String & value) const
	{
		auto it = find(value);

		if (it != end())
		{
			return (it - begin());
		}

		return size();
	}
	
	size_t Arguments::size() const
	{
		return m_values.size();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Arguments::contains(const String & value) const
	{
		return find_first(value) != end();
	}
	
	bool Arguments::empty() const
	{
		return m_values.empty();
	}
	
	bool Arguments::inRange(size_t index) const
	{
		return (index < size());
	}
	
	bool Arguments::inRange(const_iterator it) const
	{
		return it < end();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Arguments & Arguments::assign(const List<String> & value)
	{
		m_values = value;
		return (*this);
	}
  		   
	Arguments & Arguments::clear()
	{
		m_values.clear();
		return (*this);
	}
  		   
	Arguments & Arguments::copy(const Arguments & other)
	{
		return copy(other, 0);
	}
  		   
	Arguments & Arguments::copy(const Arguments & other, size_t index)
	{
		if (other.inRange(index))
		{
			return copy(other.begin() + index, other.end());
		}

		return (*this);
	}
  		   
	Arguments & Arguments::copy(const Arguments & other, size_t index, size_t count)
	{
		if (other.inRange(index))
		{
			auto start = (other.begin() + index);

			return copy(start, start + count);
		}

		return (*this);
	}
  		   
	Arguments & Arguments::copy(const_iterator first, const_iterator last)
	{
		return assign(List<String>(first, last));
	}
  		   
	Arguments & Arguments::erase(size_t index, size_t count)
	{
		return erase(begin() + index, count);
	}
  		   
	Arguments & Arguments::erase(const_iterator it, size_t count)
	{
		return erase(it, it + count);
	}
  		   
	Arguments & Arguments::erase(const_iterator first, const_iterator last)
	{
		if (!empty() && first >= begin() && last != end())
		{
			m_values.erase(first, last);
		}
		return (*this);
	}
  		   
	Arguments & Arguments::insert(size_t index, char value)
	{
		return insert(index, String(1, value));
	}
  		   
	Arguments & Arguments::insert(size_t index, C_String value)
	{
		return insert(index, String(value));
	}
  		   
	Arguments & Arguments::insert(size_t index, const String & value)
	{
		m_values.insert(begin() + index, value);
		return (*this);
	}
  		   
	Arguments & Arguments::pop_back()
	{
		if (!empty())
		{
			m_values.erase(end() - 1);
		}

		return (*this);
	}
  		   
	Arguments & Arguments::pop_front()
	{
		if (!empty())
		{
			m_values.erase(begin());
		}

		return (*this);
	}
  		   
	Arguments & Arguments::push_back(char value)
	{
		return push_back(String(1, value));
	}
  		   
	Arguments & Arguments::push_back(C_String value)
	{
		return push_back(String(value));
	}
  		   
	Arguments & Arguments::push_back(const String & value)
	{
		m_values.push_back(value);
		return (*this);
	}
  		   
	Arguments & Arguments::push_back(const List<String> & value)
	{
		for (auto it = value.begin(); it != value.end(); it++)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
  		   
	Arguments & Arguments::push_back(const Arguments & value)
	{
		return push_back(value.m_values);
	}
  		   
	Arguments & Arguments::push_front(char value)
	{
		return push_front(String(1, value));
	}
  		   
	Arguments & Arguments::push_front(C_String value)
	{
		return push_front(String(value));
	}
  		   
	Arguments & Arguments::push_front(const String & value)
	{
		return insert(0, value);
	}
  		   
	Arguments & Arguments::push_front(const List<String> & value)
	{
		for (auto it = value.end(); it != value.begin(); it--)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
  		   
	Arguments & Arguments::push_front(const Arguments & value)
	{
		return push_front(value.m_values);
	}
  		   
	Arguments & Arguments::remove(const String & value)
	{
		return erase(find_first(value));
	}
  		   
	Arguments & Arguments::removeAll(const String & value)
	{
		const_iterator it;
		while ((it = find_first(value)) != end())
		{
			(*this) = erase(it);
		}
		return (*this);
	}
  		   
	Arguments & Arguments::resize(size_t size)
	{
		m_values.resize(size);
		return (*this);
	}
  		   
	Arguments & Arguments::reverse()
	{
		if (!empty())
		{
			std::reverse(begin(), end());
		}

		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Arguments::const_iterator Arguments::find(const String & value, size_t index) const
	{
		for (auto it = begin() + index; it != end(); it++)
		{
			if (*it == value)
			{
				return it;
			}
		}
		return end();
	}
	
	Arguments::const_iterator Arguments::find_first(const String & value) const
	{
		for (auto it = begin(); it != end(); it++)
		{
			if (*it == value)
			{
				return it;
			}
		}
		return end();
	}
	
	Arguments::const_iterator Arguments::find_first_not_of(const String & value, size_t index) const
	{
		for (auto it = begin(); it != end(); it++)
		{
			if (*it != value)
			{
				return it;
			}
		}
		return end();
	}
	
	Arguments::const_iterator Arguments::find_last(const String & value) const
	{
		for (auto it = end(); it != begin(); it--)
		{
			if (*it == value)
			{
				return it;
			}
		}
		return end();
	}
	
	Arguments::const_iterator Arguments::find_last_not_of(const String & value) const
	{
		for (auto it = end(); it != begin(); it--)
		{
			if (*it != value)
			{
				return it;
			}
		}
		return end();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Arguments::find_and_erase(const String & value)
	{
		const_iterator it;
		if ((it = find(value)) != end())
		{
			erase(it);
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Arguments::equals(const Arguments & value) const
	{
		if (size() == value.size())
		{
			for (size_t i = 0, imax = size(); i < imax; i++)
			{
				if ((*this)[i] != value[i])
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	bool Arguments::lessThan(const Arguments & value) const
	{
		if (size() != value.size())
		{
			for (size_t i = 0, imax = size(); i < imax; i++)
			{
				if ((*this)[i] >= value[i])
				{
					return false;
				}
			}
			return true;
		}
		return size() < value.size();
	}

	/* * * * * * * * * * * * * * * * * * * * */
}