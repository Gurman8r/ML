#include <ML/Core/Random.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Random::Random()
	{
		this->seed();
	}

	Random::~Random()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void Random::seed()
	{
		return seed((uint32_t)std::time(NULL));
	}

	void Random::seed(const uint32_t value)
	{
		return srand(value);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}