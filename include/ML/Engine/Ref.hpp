#ifndef _ML_REF_HPP_
#define _ML_REF_HPP_

#include <ML/Engine/ContentDatabase.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	// Pointer to something that lives in Content
	// Automatically retrieves the value from Content and acts like a pointer to it
	template <
		class T
	> struct Ref final : public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using self_type			= typename Ref<value_type>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Ref()
			: m_name(nullptr)
			, m_data(nullptr)
		{
		}

		explicit Ref(const String & name)
			: m_name(name)
			, m_data(nullptr)
		{
		}

		Ref(const self_type & copy)
			: m_name(copy.m_name)
			, m_data(copy.m_data)
		{
		}

		~Ref() {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <
			class ... Args
		> inline pointer create(Args && ... args)
		{
			if (m_changed || (!m_data && m_name))
			{
				m_data = ML_Content.create<value_type>(m_name, std::forward<Args>(args)...);
			}
			return m_data;
		}

		inline pointer get() 
		{ 
			if (m_changed || (!m_data && m_name))
			{
				m_data = ML_Content.get<value_type>(m_name);
				m_changed = false;
			}
			return m_data;
		}

		inline const_pointer get() const 
		{
			if (m_changed || (!m_data && m_name))
			{
				m_data = ML_Content.get<value_type>(m_name);
				m_changed = false;
			}
			return m_data;
		}

		inline bool destroy()
		{
			if (m_data && m_name)
			{
				return ML_Content.erase<value_type>(m_name);
			}
			return !m_data;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const_pointer data() const 
		{ 
			return m_data; 
		}

		inline const String & name() const 
		{
			return m_name; 
		}

		inline const String & rename(const String & value)
		{
			if (value != m_name)
			{
				m_name = value;
				m_changed = true;
			}
			return m_name;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator			bool()			const	{ return this->get(); }
		inline operator			pointer()				{ return this->get(); }
		inline operator			const_pointer() const	{ return this->get(); }
		inline pointer			operator->()			{ return this->get(); }
		inline const_pointer	operator->()	const	{ return this->get(); }
		inline reference		operator *()			{ return (*this->get()); }
		inline const_reference	operator *()	const	{ return (*this->get()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		String			m_name;
		mutable pointer m_data;
		mutable bool	m_changed;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REF_HPP_