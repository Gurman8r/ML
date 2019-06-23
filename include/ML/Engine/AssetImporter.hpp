#ifndef _ML_ASSET_IMPORTER_HPP_
#define _ML_ASSET_IMPORTER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Readable.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/Metadata.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct CubeMap;
	struct Entity;
	struct Font;
	struct Image;
	struct Material;
	struct Mesh;
	struct Model;
	struct Script;
	struct Shader;
	struct Sound;
	struct Sprite;
	struct Surface;
	struct Texture;
	struct Uniform;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct CustomAssetImporter
	{
		using	value_type = typename T;
		virtual value_type * operator()(const Metadata & md) const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct AssetImporter;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#define ML_GEN_ASSET_IMPORTER(OUTPUT, CUSTOM) \
template <> struct AssetImporter<OUTPUT> \
{ \
	static constexpr auto tag { CUSTOM::tag }; \
	template <class ... Args> \
	inline auto operator()(Args && ... args) \
	{ \
		return CUSTOM()(std::forward<Args>(args)...); \
	} \
};

namespace ml
{
	// CubeMap Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API CubeMapImporter final : public CustomAssetImporter<CubeMap>
	{
		static constexpr CString tag { "cubemap" };
		CubeMap * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(CubeMap, CubeMapImporter);


	// Entity Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API EntityImporter final : public CustomAssetImporter<Entity>
	{
		static constexpr CString tag { "entity" };
		Entity * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Entity, EntityImporter);


	// Font Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API FontImporter final : public CustomAssetImporter<Font>
	{
		static constexpr CString tag { "font" };
		Font * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Font, FontImporter);


	// Image Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API ImageImporter final : public CustomAssetImporter<Image>
	{
		static constexpr CString tag { "image" };
		Image * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Image, ImageImporter);


	// Material Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API MaterialImporter final : public CustomAssetImporter<Material>
	{
		static constexpr CString tag { "material" };
		Material * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Material, MaterialImporter);


	// Mesh Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API MeshImporter final : public CustomAssetImporter<Mesh>
	{
		static constexpr CString tag { "mesh" };
		Mesh * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Mesh, MeshImporter);


	// Model Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API ModelImporter final : public CustomAssetImporter<Model>
	{
		static constexpr CString tag { "model" };
		Model * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Model, ModelImporter);


	// Script Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API ScriptImporter final : public CustomAssetImporter<Script>
	{
		static constexpr CString tag { "script" };
		Script * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Script, ScriptImporter);


	// Shader Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API ShaderImporter final : public CustomAssetImporter<Shader>
	{
		static constexpr CString tag { "shader" };
		Shader * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Shader, ShaderImporter);


	// Sound Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API SoundImporter final : public CustomAssetImporter<Sound>
	{
		static constexpr CString tag { "sound" };
		Sound * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Sound, SoundImporter);


	// Sprite Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API SpriteImporter final : public CustomAssetImporter<Sprite>
	{
		static constexpr CString tag { "sprite" };
		Sprite * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Sprite, SpriteImporter);


	// Surface Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API SurfaceImporter final : public CustomAssetImporter<Surface>
	{
		static constexpr CString tag { "surface" };
		Surface * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Surface, SurfaceImporter);


	// Texture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API TextureImporter final : public CustomAssetImporter<Texture>
	{
		static constexpr CString tag { "texture" };
		Texture * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Texture, TextureImporter);

	
	// Uniform Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API UniformImporter final : public CustomAssetImporter<Uniform>
	{
		static constexpr CString tag { "uniform" };
		Uniform * operator()(const Metadata & md) const override;
	};
	ML_GEN_ASSET_IMPORTER(Uniform, UniformImporter);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_IMPORTER_HPP_