#ifndef _ML_REQUEST_HPP_
#define _ML_REQUEST_HPP_

#include <ML/Core/IRequest.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_CORE_API TestRequest
		: public IRequest
	{
	public:
		using LogFunc = void(*)(const ml::String &);

	public:
		TestRequest();
		~TestRequest();

	public:
		void setValue(int32_t value);
		void setFunc(LogFunc value);

	public:
		void process() override;
		void finish() override;

	private:
		int32_t m_value;
		LogFunc m_func;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_REQUEST_HPP_