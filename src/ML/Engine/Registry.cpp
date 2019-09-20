#include <ML/Engine/Registry.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Graphics/Renderer.hpp>
#include <ML/Graphics/Camera.hpp>
#include <ML/Graphics/Light.hpp>
#include <ML/Graphics/Transform.hpp>
#include <ML/Audio/Sound.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Material.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Engine/Script.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Registry<>::Registry() : m_codes(), m_funcs(), m_infos(), m_names() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_REGISTER(Entity,		"") { return new Entity(); }
	ML_REGISTER(Font,		"") { return new Font(); }
	ML_REGISTER(Image,		"") { return new Image(); }
	ML_REGISTER(Material,	"") { return new Material(); }
	ML_REGISTER(Model,		"") { return new Model(); }
	ML_REGISTER(Script,		"") { return new Script(); }
	ML_REGISTER(Shader,		"") { return new Shader(); }
	ML_REGISTER(Sound,		"") { return new Sound(); }
	ML_REGISTER(Sprite,		"") { return new Sprite(); }
	ML_REGISTER(Surface,	"") { return new Surface(); }
	ML_REGISTER(Texture,	"") { return new Texture(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ML_REGISTER(Camera,		"/Component") { return new Camera(); }
	ML_REGISTER(Light,		"/Component") { return new Light(); }
	ML_REGISTER(Renderer,	"/Component") { return new Renderer(); }
	ML_REGISTER(Transform,	"/Component") { return new Transform(); }
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}