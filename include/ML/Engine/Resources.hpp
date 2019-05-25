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
	class Material;
	class Mesh;
	class Model;
	class Script;
	class Shader;
	class Skybox;
	class Sound;
	class Sprite;
	class Surface;
	class Texture;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Resources final
		: public IObject
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
		bool parseItem(const ManifestItem & data);

	public:
		Registry<Entity>	entities;
		Registry<Font>		fonts;
		Registry<Image>		images;
		Registry<Material>	materials;
		Registry<Mesh>		meshes;
		Registry<Model>		models;
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