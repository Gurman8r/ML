#ifndef _ML_ASSET_IMPORTER_HPP_
#define _ML_ASSET_IMPORTER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/Metadata.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Entity;
	struct Font;
	struct Image;
	struct Material;
	struct Model;
	struct Script;
	struct Shader;
	struct Sound;
	struct Sprite;
	struct Surface;
	struct Texture;
	struct Uni;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct AssetImporter;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct CustomAssetImporter
	{
		using value_type	= typename detail::decay_t<T>;
		using self_type		= typename CustomAssetImporter<value_type>;
		using wrapper_type	= typename AssetImporter<value_type>;

		static constexpr auto getHash() -> hash_t	{ return wrapper_type::id;	}
		static constexpr auto getTag()	-> C_String	{ return wrapper_type::tag;	}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#define ML_GEN_ASSET_IMPORTER_EXT(PREFIX, NAME, ELEM, TAG, IMPL)	\
struct PREFIX NAME final : public _ML CustomAssetImporter<ELEM>		\
##IMPL;																\
template <> struct _ML AssetImporter<ELEM>							\
{																	\
	using type = typename ELEM;										\
	static constexpr auto id	{ _ML Hash(##TAG) };				\
	static constexpr auto tag	{ ##TAG };							\
	template <														\
		class ... Args												\
	> inline auto operator()(Args && ... args) const				\
	{																\
		return NAME()(_STD forward<Args>(args)...);					\
	}																\
};

#define ML_GEN_ASSET_IMPORTER(NAME, ELEM, TAG, IMPL) \
	ML_GEN_ASSET_IMPORTER_EXT(ML_ENGINE_API, NAME, ELEM, TAG, IMPL)

namespace ml
{
	// Entity Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(EntityAssetImporter, Entity, "entity",
	{
		Entity * operator()(const Metadata & md) const;
	});


	// Font Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(FontAssetImporter, Font, "font",
	{
		Font * operator()(const Metadata & md) const;
	});


	// Image Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(ImageAssetImporter, Image, "image",
	{
		Image * operator()(const Metadata & md) const;
	});


	// Material Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(MaterialAssetImporter, Material, "material",
	{
		Material * operator()(const Metadata & md) const;
	});


	// Model Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(ModelAssetImporter, Model, "model",
	{
		Model * operator()(const Metadata & md) const;
	});


	// Script Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(ScriptAssetImporter, Script, "script",
	{
		Script * operator()(const Metadata & md) const;
	});


	// Shader Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(ShaderAssetImporter, Shader, "shader",
	{
		Shader * operator()(const Metadata & md) const;
	});


	// Sound Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(SoundAssetImporter, Sound, "sound",
	{
		Sound * operator()(const Metadata & md) const;
	});


	// Sprite Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(SpriteAssetImporter, Sprite, "sprite",
	{
		Sprite * operator()(const Metadata & md) const;
	});


	// Surface Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(SurfaceAssetImporter, Surface, "surface",
	{
		Surface * operator()(const Metadata & md) const;
	});


	// Texture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_ASSET_IMPORTER(TextureAssetImporter, Texture, "texture",
	{
		Texture * operator()(const Metadata & md) const;
	});
}

#endif // !_ML_ASSET_IMPORTER_HPP_