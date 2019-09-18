#include <ML/Engine/RegistryManager.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/Transform.hpp>

#define ML_GEN_FUNC(name) ML_CONCAT(ml_generate_, name)

#define ML_REGISTER_EXT(TYPE, FUN) \
static void * FUN(); \
bool _ML Registry<TYPE>::s_registered = ML_Registry.production<TYPE>(FUN); \
void * FUN()

#define ML_REGISTER(T) ML_REGISTER_EXT(T, ML_GEN_FUNC(T))

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_REGISTER(Camera)		{ return new Camera(); };
	ML_REGISTER(Light)		{ return new Light(); }
	ML_REGISTER(Renderer)	{ return new Renderer(); }
	ML_REGISTER(Transform)	{ return new Transform(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}