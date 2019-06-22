#ifndef _ML_RESOURCES_HPP_
#define _ML_RESOURCES_HPP_

#include <ML/Engine/Registry.hpp>
#include <ML/Engine/ManifestItem.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct	CubeMap;
	struct	Entity;
	struct	Font;
	struct	Image;
	struct	Material;
	struct	Mesh;
	struct	Model;
	struct	Script;
	struct	Shader;
	struct	Sound;
	struct	Sprite;
	struct	Surface;
	struct	Texture;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API Resources final
		: public I_Newable
		, public I_Disposable
		, public I_Readable
		, public I_NonCopyable
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
		Registry<Sound> 	sounds;
		Registry<Sprite>	sprites;
		Registry<Surface>	surfaces;
		Registry<Texture>	textures;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_RESOURCES_HPP_