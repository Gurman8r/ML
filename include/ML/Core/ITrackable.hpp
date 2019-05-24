#ifndef _ML_I_TRACKABLE_HPP_
#define _ML_I_TRACKABLE_HPP_

#include <ML/Core/MemoryTracker.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API ITrackable
	{
	public:
		/* * * * * * * * * * * * * * * * * * * * */
		virtual ~ITrackable() {}

		inline virtual void serialize(std::ostream & out) const
		{
			out << this->GetTypeName();
		}

		inline virtual void deserialize(std::istream & in)
		{
		}

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline String	GetTypeName()	const { return typeid(*this).name(); }
		inline size_t	GetTypeHash()	const { return typeid(*this).hash_code(); }
		inline SStream	ToStream()		const { SStream s; s << (*this); return s; }
		inline String	ToString()		const { return ToStream().str(); }
		inline CString	ToCString()		const { return ToString().c_str(); }

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline void * operator	new		 (size_t size) { return ML_new(size); }
		inline void * operator	new[]	 (size_t size) { return ML_new(size); }
		inline void	  operator	delete	 (void * ptr)  { return ML_free(ptr); }
		inline void	  operator	delete[] (void * ptr)  { return ML_free(ptr); }

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		inline friend std::ostream & operator<<(std::ostream & out, const ITrackable & value)
		{
			value.serialize(out); return out;
		}

		inline friend std::istream & operator>>(std::istream & in, ITrackable & value)
		{
			value.deserialize(in); return in;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_I_TRACKABLE_HPP_