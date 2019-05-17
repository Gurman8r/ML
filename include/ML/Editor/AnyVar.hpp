#ifndef _ML_ANY_VAR_HPP_
#define _ML_ANY_VAR_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Core/ITrackable.hpp>
#include <ML/Core/IDisposable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API AnyVar
		: public ITrackable
		, public IDisposable
	{
	public:
		enum : int32_t
		{
			T_None = -1,
			T_Int, 
			T_Float,
			MAX_ANY_VAR_TYPE
		};

	public:
		AnyVar();
		AnyVar(const AnyVar & copy);
		~AnyVar();

	public:
		bool dispose() override;
		void serialize(std::ostream & out) const override;

	public:
		void * allocate(const size_t size);
		float * allocFloat(const size_t size);

	public:
		const float *	getFloat() const;
		const int32_t * getInt() const;

	public:
		float *			setFloat(const float * value);
		int32_t *		setInt(const int32_t * value);

	private:
		int32_t m_type;
		void *	m_data;
		size_t	m_size;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ANY_VAR_HPP_