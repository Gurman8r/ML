#include <ML/Engine/ScriptableObject.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ScriptableObject::ScriptableObject()
	{
	}

	ScriptableObject::~ScriptableObject()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ScriptableObject::dispose()
	{
		return true;
	}

	bool ScriptableObject::loadFromFile(const String & filename)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}