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
	> struct AssetImporter;

	template <
		class T
	> struct CustomAssetImporter
	{
		using value_type	= typename T;
		using self_type		= typename CustomAssetImporter<value_type>;
		using impl_type		= typename AssetImporter<value_type>;
		
		virtual value_type * operator()(const Metadata & md) const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#define ML_GEN_ASSET_IMPORTER_EXT(PREFIX, STRUCT_NAME, OUTPUT, TAG, STRUCT_BODY) \
struct PREFIX STRUCT_NAME final : public ::ml::CustomAssetImporter<OUTPUT> \
##STRUCT_BODY; \
template <> struct ::ml::AssetImporter<OUTPUT> \
{ \
	static constexpr auto tag { ##TAG }; \
	template < \
		class ... Args \
	> inline auto operator()(Args && ... args) \
	{ \
		return ::ml::##CUSTOM()(std::forward<Args>(args)...); \
	} \
};

#define ML_GEN_ASSET_IMPORTER(STRUCT_NAME, OUTPUT, TAG, STRUCT_BODY) \
ML_GEN_ASSET_IMPORTER_EXT(ML_ENGINE_API, STRUCT_NAME, OUTPUT, TAG, STRUCT_BODY)

namespace ml
{
	// CubeMap Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(CubeMapImporter, CubeMap, "cubemap",
	{
		CubeMap * operator()(const Metadata & md) const override;
	});


	// Entity Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(EntityImporter, Entity, "entity",
	{
		Entity * operator()(const Metadata & md) const override;
	});


	// Font Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(FontImporter, Font, "font",
	{
		Font * operator()(const Metadata & md) const override;
	});


	// Image Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(ImageImporter, Image, "image",
	{
		Image * operator()(const Metadata & md) const override;
	});


	// Material Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(MaterialImporter, Material, "material",
	{
		Material * operator()(const Metadata & md) const override;
	});


	// Mesh Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(MeshImporter, Mesh, "mesh",
	{
		Mesh * operator()(const Metadata & md) const override;
	});


	// Model Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(ModelImporter, Model, "model",
	{
		Model * operator()(const Metadata & md) const override;
	});


	// Script Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(ScriptImporter, Script, "script",
	{
		Script * operator()(const Metadata & md) const override;
	});


	// Shader Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(ShaderImporter, Shader, "shader",
	{
		Shader * operator()(const Metadata & md) const override;
	});


	// Sound Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(SoundImporter, Sound, "sound",
	{
		Sound * operator()(const Metadata & md) const override;
	});


	// Sprite Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(SpriteImporter, Sprite, "sprite",
	{
		Sprite * operator()(const Metadata & md) const override;
	});


	// Surface Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(SurfaceImporter, Surface, "surface",
	{
		Surface * operator()(const Metadata & md) const override;
	});


	// Texture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(TextureImporter, Texture, "texture",
	{
		Texture * operator()(const Metadata & md) const override;
	});

	
	// Uniform Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(UniformImporter, Uniform, "uniform",
	{
		Uniform * operator()(const Metadata & md) const override;
	});


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_IMPORTER_HPP_