#ifndef _ML_RESOURCES_HPP_
#define _ML_RESOURCES_HPP_

#include <ML/Engine/Registry.hpp>
#include <ML/Engine/ManifestItem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Entity;
	class Font;
	class Image;
	class LuaScript;
	class Material;
	class Mesh;
	class Model;
	class Plugin;
	class Script;
	class Shader;
	class Skybox;
	class Sound;
	class Sprite;
	class Surface;
	class Texture;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Resources final
		: public ITrackable
		, public IDisposable
		, public IReadable
		, public INonCopyable
	{
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
		Registry<Surface>	surfaces;
		Registry<Texture>	textures;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RESOURCES_HPP_