#include <ML/Script/Args.hpp>
#include <ML/Core/StringUtility.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Args::Args()
		: m_values()
	{
	}
	
	Args::Args(int32_t argc, char ** argv)
		: m_values()
	{
		for (size_t i = 0, imax = (size_t)argc; i < imax; i++)
		{
			push_back("\"" + String(argv[i]) + "\"");
		}
	}
	
	Args::Args(const String & value)
		: m_values({ value })
	{
	}
	
	Args::Args(const List<String> & values)
		: m_values(values)
	{
	}
	
	Args::Args(const Initializer<String>& values)
		: m_values(values)
	{
	}
	
	Args::Args(const String & value, const String & delim)
		: m_values(StringUtility::Split(value, delim))
	{
	}
	
	Args::Args(const Args & copy)
		: m_values(copy.m_values)
	{
	}
	
	Args::~Args()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	String Args::pop()
	{
		return pop_front().front();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	String Args::at(size_t index) const
	{
		return (*this)[index];
	}
	
	String Args::back() const
	{
		if (!empty())
		{
			return m_values.back();
		}
		return String();
	}
	
	String Args::front() const
	{
		if (!empty())
		{
			return m_values.front();
		}
		return String();
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	size_t Args::count(const String & value) const
	{
		return count(begin(), end(), value);
	}
	
	size_t Args::count(const_iterator first, const_iterator last, const String & val) const
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
	
	size_t Args::indexOf(const String & value) const
	{
		auto it = find(value);

		if (it != end())
		{
			return (it - begin());
		}

		return size();
	}
	
	size_t Args::size() const
	{
		return m_values.size();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool Args::contains(const String & value) const
	{
		return find_first(value) != end();
	}
	
	bool Args::empty() const
	{
		return m_values.empty();
	}
	
	bool Args::inRange(size_t index) const
	{
		return (index < size());
	}
	
	bool Args::inRange(const_iterator it) const
	{
		return it < end();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Args & Args::assign(const List<String> & value)
	{
		m_values = value;
		return (*this);
	}
  		   
	Args & Args::clear()
	{
		m_values.clear();
		return (*this);
	}
  		   
	Args & Args::copy(const Args & other)
	{
		return copy(other, 0);
	}
  		   
	Args & Args::copy(const Args & other, size_t index)
	{
		if (other.inRange(index))
		{
			return copy(other.begin() + index, other.end());
		}

		return (*this);
	}
  		   
	Args & Args::copy(const Args & other, size_t index, size_t count)
	{
		if (other.inRange(index))
		{
			auto start = (other.begin() + index);

			return copy(start, start + count);
		}

		return (*this);
	}
  		   
	Args & Args::copy(const_iterator first, const_iterator last)
	{
		return assign(List<String>(first, last));
	}
  		   
	Args & Args::erase(size_t index, size_t count)
	{
		return erase(begin() + index, count);
	}
  		   
	Args & Args::erase(const_iterator it, size_t count)
	{
		return erase(it, it + count);
	}
  		   
	Args & Args::erase(const_iterator first, const_iterator last)
	{
		if (!empty() && first >= begin() && last != end())
		{
			m_values.erase(first, last);
		}
		return (*this);
	}
  		   
	Args & Args::insert(size_t index, char value)
	{
		return insert(index, String(1, value));
	}
  		   
	Args & Args::insert(size_t index, CString value)
	{
		return insert(index, String(value));
	}
  		   
	Args & Args::insert(size_t index, const String & value)
	{
		m_values.insert(begin() + index, value);
		return (*this);
	}
  		   
	Args & Args::pop_back()
	{
		if (!empty())
		{
			m_values.erase(end() - 1);
		}

		return (*this);
	}
  		   
	Args & Args::pop_front()
	{
		if (!empty())
		{
			m_values.erase(begin());
		}

		return (*this);
	}
  		   
	Args & Args::push_back(char value)
	{
		return push_back(String(1, value));
	}
  		   
	Args & Args::push_back(CString value)
	{
		return push_back(String(value));
	}
  		   
	Args & Args::push_back(const String & value)
	{
		m_values.push_back(value);
		return (*this);
	}
  		   
	Args & Args::push_back(const List<String> & value)
	{
		for (auto it = value.begin(); it != value.end(); it++)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
  		   
	Args & Args::push_back(const Args & value)
	{
		return push_back(value.m_values);
	}
  		   
	Args & Args::push_front(char value)
	{
		return push_front(String(1, value));
	}
  		   
	Args & Args::push_front(CString value)
	{
		return push_front(String(value));
	}
  		   
	Args & Args::push_front(const String & value)
	{
		return insert(0, value);
	}
  		   
	Args & Args::push_front(const List<String> & value)
	{
		for (auto it = value.end(); it != value.begin(); it--)
		{
			(*this) = push_back(*it);
		}
		return (*this);
	}
  		   
	Args & Args::push_front(const Args & value)
	{
		return push_front(value.m_values);
	}
  		   
	Args & Args::remove(const String & value)
	{
		return erase(find_first(value));
	}
  		   
	Args & Args::removeAll(const String & value)
	{
		const_iterator it;
		while ((it = find_first(value)) != end())
		{
			(*this) = erase(it);
		}
		return (*this);
	}
  		   
	Args & Args::resize(size_t size)
	{
		m_values.resize(size);
		return (*this);
	}
  		   
	Args & Args::reverse()
	{
		if (!empty())
		{
			std::reverse(begin(), end());
		}

		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * */

	Args::const_iterator Args::find(const String & value, size_t index) const
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
	
	Args::const_iterator Args::find_first(const String & value) const
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
	
	Args::const_iterator Args::find_first_not_of(const String & value, size_t index) const
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
	
	Args::const_iterator Args::find_last(const String & value) const
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
	
	Args::const_iterator Args::find_last_not_of(const String & value) const
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

	bool Args::find_and_erase(const String & value)
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

	bool Args::equals(const Args & value) const
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

	bool Args::lessThan(const Args & value) const
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