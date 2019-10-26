#ifndef _ML_REF_HPP_
#define _ML_REF_HPP_

#include <ML/Engine/ContentManager.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
	// Pointer to something that lives in Content
	// Automatically retrieves the value from Content and acts like a pointer to it
	template <class T> struct Ref final : public Newable, public Disposable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using value_type		= typename T;
		using self_type			= typename Ref<value_type>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		Ref() : m_name {}, m_data { nullptr } {}

		explicit Ref(const String & name) : m_name { name }, m_data { nullptr } {}

		Ref(const self_type & copy) : m_name { copy.m_name }, m_data { copy.m_data } {}

		Ref(self_type && copy) : Ref {}
		{
			std::swap(m_name, copy.m_name);
			std::swap(m_data, copy.m_data);
		}

		~Ref() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dispose() override
		{
			m_data = nullptr;
			return !m_data;
		}

		inline self_type & update(const String & name, pointer data)
		{
			if (m_name != name && m_data != data)
			{
				m_name = name;
				m_data = data;
			}
			return (*this);
		}

		inline self_type & update(const_pointer value)
		{
			if (const String name { ML_Content.get_name(value) })
			{
				m_name = name;
				m_data = std::remove_cv_t<pointer>(value);
			}
			return (*this);
		}

		template <class ... Args>
		inline pointer create(Args && ... args)
		{
			return (m_data = ML_Content.create<value_type>(m_name, std::forward<Args>(args)...));
		}

		inline pointer get() 
		{ 
			return (m_data = ML_Content.get<value_type>(m_name));
		}

		inline const_pointer get() const 
		{
			return (m_data = ML_Content.get<value_type>(m_name));
		}

		inline bool destroy()
		{
			return ML_Content.destroy<value_type>(m_name);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const String & name() const { return m_name; }

		inline const_pointer data() const { return m_data; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline operator			bool()			const	{ return this->get(); }
		inline operator			pointer()				{ return this->get(); }
		inline operator			const_pointer() const	{ return this->get(); }
		inline pointer			operator->()			{ return this->get(); }
		inline const_pointer	operator->()	const	{ return this->get(); }
		inline reference		operator *()			{ return (*this->get()); }
		inline const_reference	operator *()	const	{ return (*this->get()); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		String m_name;
		mutable pointer m_data;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REF_HPP_