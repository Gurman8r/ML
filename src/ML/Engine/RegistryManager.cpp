#include <ML/Engine/RegistryManager.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/Transform.hpp>

#define ML_REGISTER_TYPE_EXT(T, FUN) static void * FUN(); \
bool _ML Registry<T>::s_registered = ML_Registry.production<T>(FUN); \
void * FUN()

#define ML_REGISTER_TYPE(T) ML_REGISTER_TYPE_EXT(T, ML_CONCAT(ml_production_, T))

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_REGISTER_TYPE(Camera)	{ return new Camera(); };
	ML_REGISTER_TYPE(Light)		{ return new Light(); }
	ML_REGISTER_TYPE(Renderer)	{ return new Renderer(); }
	ML_REGISTER_TYPE(Transform)	{ return new Transform(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}