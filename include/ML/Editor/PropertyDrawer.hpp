#ifndef _ML_PROPERTY_DRAWER_HPP_
#define _ML_PROPERTY_DRAWER_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/String.hpp>

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
	> struct PropertyDrawer;

	template <
		class T
	> struct CustomPropertyDrawer
	{
		using value_type	= typename T;
		using self_type		= typename CustomPropertyDrawer<value_type>;
		using impl_type		= typename PropertyDrawer<value_type>;

		virtual bool operator()(const String & label, value_type value) const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#define ML_GEN_PROPERTY_DRAWER_EXT(PREFIX, STRUCT_NAME, OUTPUT, TAG, STRUCT_BODY) \
struct PREFIX STRUCT_NAME final : public ::ml::CustomPropertyDrawer<OUTPUT> \
##STRUCT_BODY; \
template <> struct ::ml::PropertyDrawer<OUTPUT> \
{ \
	constexpr static auto tag { ##TAG }; \
	template < \
		class ... Args \
	> inline auto operator()(Args && ... args) \
	{ \
		return ::ml::##STRUCT_NAME()(std::forward<Args>(args)...); \
	} \
};

#define ML_GEN_PROPERTY_DRAWER(STRUCT_NAME, OUTPUT, TAG, STRUCT_BODY) \
ML_GEN_PROPERTY_DRAWER_EXT(ML_EDITOR_API, STRUCT_NAME, OUTPUT, TAG, STRUCT_BODY)

namespace ml
{
	// CubeMap Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(CubeMapDrawer, const CubeMap *, "cubemap",
	{
		bool operator()(const String & label, const CubeMap * value) const override;
	});


	// Entity Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(EntityDrawer, const Entity *, "entity",
	{
		bool operator()(const String & label, const Entity * value) const override;
	});


	// Font Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(FontDrawer, const Font *, "font",
	{
		bool operator()(const String & label, const Font * value) const override;
	});


	// Image Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(ImageDrawer, const Image *, "image",
	{
		bool operator()(const String & label, const Image * value) const override;
	});


	// Material Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(MaterialDrawer, const Material *, "material",
	{
		bool operator()(const String & label, const Material * value) const override;
	});


	// Mesh Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(MeshDrawer, const Mesh *, "mesh",
	{
		bool operator()(const String & label, const Mesh * value) const override;
	});


	// Model Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(ModelDrawer, const Model *, "model",
	{
		bool operator()(const String & label, const Model * value) const override;
	});


	// Script Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(ScriptDrawer, const Script *, "script",
	{
		bool operator()(const String & label, const Script * value) const override;
	});


	// Shader Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(ShaderDrawer, const Shader *, "shader",
	{
		bool operator()(const String & label, const Shader * value) const override;
	});


	// Sound Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(SoundDrawer, const Sound *, "sound",
	{
		bool operator()(const String & label, const Sound * value) const override;
	});


	// Sprite Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(SpriteDrawer, const Sprite *, "sprite",
	{
		bool operator()(const String & label, const Sprite * value) const override;
	});


	// Surface Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(SurfaceDrawer, const Surface *, "surface",
	{
		bool operator()(const String & label, const Surface * value) const override;
	});


	// Texture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(TextureDrawer, const Texture *, "cubemap",
	{
		bool operator()(const String & label, const Texture * value) const override;
	});


	// Uniform Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(UniformDrawer, Uniform *, "uniform",
	{
		bool operator()(const String & label, Uniform * value) const override;
	});


	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


#endif // !_ML_PROPERTY_DRAWER_HPP_