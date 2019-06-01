#include <ML/Graphics/Skybox.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Graphics/OpenGL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Skybox::Skybox()
	{
	}

	Skybox::~Skybox()
	{
		dispose();
	}
	
	/* * * * * * * * * * * * * * * * * * * * */

	bool Skybox::dispose()
	{
		return true;
	}
	
	bool Skybox::loadFromFile(const String & filename)
	{
		return true;
	}
	
	/* * * * * * * * * * * * * * * * * * * * */
}