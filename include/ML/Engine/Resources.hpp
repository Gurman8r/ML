#ifndef _ML_RESOURCES_HPP_
#define _ML_RESOURCES_HPP_

#include <ML/Audio/Sound.hpp>
#include <ML/Core/List.hpp>
#include <ML/Engine/Registry.hpp>
#include <ML/Engine/ManifestItem.hpp>
#include <ML/Engine/Entity.hpp>
#include <ML/Engine/LuaScript.hpp>
#include <ML/Engine/Plugin.hpp>
#include <ML/Graphics/Surface.hpp>
#include <ML/Graphics/Font.hpp>
#include <ML/Graphics/Model.hpp>
#include <ML/Graphics/Skybox.hpp>
#include <ML/Graphics/Sprite.hpp>
#include <ML/Script/Script.hpp>

#define ML_Resources ml::Resources::getInstance()

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Resources final
		: public ITrackable
		, public IDisposable
		, public IReadable
		, public ISingleton<Resources>
	{
		friend class ISingleton<Resources>;

	public:
		Resources();
		~Resources();

	public:
		size_t cleanupAll();
		size_t reloadAll();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

	public:
		bool parseFile(SStream & file);
		bool parseItem(const ManifestItem & data);

	public:
		Registry<Surface>	surfaces;
		Registry<Entity>	entities;
		Registry<Font>		fonts;
		Registry<Image>		images;
		Registry<LuaScript>	lua;
		Registry<Material>	materials;
		Registry<Mesh>		meshes;
		Registry<Model>		models;
		Registry<Plugin>	plugins;
		Registry<Script>	scripts;
		Registry<Shader>	shaders;
		Registry<Skybox>	skyboxes;
		Registry<Sound> 	sounds;
		Registry<Sprite>	sprites;
		Registry<Texture>	textures;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RESOURCES_HPP_