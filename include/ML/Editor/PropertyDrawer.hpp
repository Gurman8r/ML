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
		using value_type		= typename T;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename const value_type *;
		using const_reference	= typename const value_type &;
		using self_type			= typename CustomPropertyDrawer<value_type>;
		using wrapper_type		= typename PropertyDrawer<value_type>;

		static constexpr auto getHash() -> hash_t	{ return wrapper_type::id;	}
		static constexpr auto getTag()	-> C_String	{ return wrapper_type::tag; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#define ML_GEN_PROPERTY_DRAWER_EXT(PREFIX, NAME, OUT, TAG, IMPL)	\
struct PREFIX NAME final : public ::ml::CustomPropertyDrawer<OUT>	\
##IMPL;																\
template <> struct ::ml::PropertyDrawer<OUT>						\
{																	\
	using type = typename OUT;										\
	static constexpr auto id	{ ::ml::Hash()(##TAG) };			\
	static constexpr auto tag	{ ##TAG };							\
	template <														\
		class ... Args												\
	> inline auto operator()(Args && ... args) const				\
	{																\
		return NAME()(_STD forward<Args>(args)...);					\
	}																\
};

#define ML_GEN_PROPERTY_DRAWER(NAME, OUT, TAG, IMPL) ML_GEN_PROPERTY_DRAWER_EXT( \
	ML_EDITOR_API, NAME, OUT, TAG, IMPL \
)

namespace ml
{
	// CubeMap Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(CubeMapPropertyDrawer, CubeMap, "cubemap",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Entity Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(EntityPropertyDrawer, Entity, "entity",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Font Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(FontPropertyDrawer, Font, "font",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Image Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(ImagePropertyDrawer, Image, "image",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Material Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(MaterialPropertyDrawer, Material, "material",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Mesh Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(MeshPropertyDrawer, Mesh, "mesh",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Model Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(ModelPropertyDrawer, Model, "model",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Script Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(ScriptPropertyDrawer, Script, "script",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Shader Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(ShaderPropertyDrawer, Shader, "shader",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Sound Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(SoundPropertyDrawer, Sound, "sound",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Sprite Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(SpritePropertyDrawer, Sprite, "sprite",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Surface Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(SurfacePropertyDrawer, Surface, "surface",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Texture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(TexturePropertyDrawer, Texture, "cubemap",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, reference value) const;
	});


	// Uniform Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	ML_GEN_PROPERTY_DRAWER(UniformPropertyDrawer, Uniform, "uniform",
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, const_reference value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	});
}

#endif // !_ML_PROPERTY_DRAWER_HPP_