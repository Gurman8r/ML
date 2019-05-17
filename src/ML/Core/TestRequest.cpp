#include <ML/Core/TestRequest.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	TestRequest::TestRequest()
	{
	}

	TestRequest::~TestRequest()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void TestRequest::setValue(int32_t value)
	{
		m_value = value;
	}

	void TestRequest::setFunc(LogFunc value)
	{
		m_func = value;
	}

	void TestRequest::process()
	{
		m_func(String("Processing Request: {0}").format(m_value));
	}

	void TestRequest::finish()
	{
		m_func(String("Finishing Request: {0}").format(m_value));
	}

	/* * * * * * * * * * * * * * * * * * * * */
}