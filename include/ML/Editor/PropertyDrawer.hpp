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
	> struct CustomPropertyDrawer
	{
		using value_type = typename T;
		virtual bool operator()(const String & label, value_type value) const = 0;
	};

	template <
		class T
	> struct PropertyDrawer;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#define ML_GEN_PROPERTY_DRAWER(OUTPUT, CUSTOM, TAG) \
template <> struct ::ml::PropertyDrawer<OUTPUT> \
{ \
	constexpr static auto tag { #TAG }; \
	template <class ... Args> \
	inline auto operator()(Args && ... args) \
	{ \
		return ::ml:: CUSTOM()(std::forward<Args>(args)...); \
	} \
};

namespace ml
{
	// CubeMap Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API CubeMapDrawer final : public CustomPropertyDrawer<const CubeMap *>
	{
		bool operator()(const String & label, const CubeMap * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const CubeMap *, CubeMapDrawer, "cubemap");


	// Entity Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API EntityDrawer final : public CustomPropertyDrawer<const Entity *>
	{
		bool operator()(const String & label, const Entity * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const Entity *, EntityDrawer, "entity");


	// Font Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API FontDrawer final : public CustomPropertyDrawer<const Font *>
	{
		bool operator()(const String & label, const Font * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const Font *, FontDrawer, "font");


	// Image Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API ImageDrawer final : public CustomPropertyDrawer<const Image *>
	{
		bool operator()(const String & label, const Image * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const Image *, ImageDrawer, "image");


	// Material Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API MaterialDrawer final : public CustomPropertyDrawer<const Material *>
	{
		bool operator()(const String & label, const Material * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const Material *, MaterialDrawer, "material");


	// Mesh Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API MeshDrawer final : public CustomPropertyDrawer<const Mesh *>
	{
		bool operator()(const String & label, const Mesh * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const Mesh *, MeshDrawer, "mesh");


	// Model Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API ModelDrawer final : public CustomPropertyDrawer<const Model *>
	{
		bool operator()(const String & label, const Model * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const Model *, ModelDrawer, "model");


	// Script Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API ScriptDrawer final : public CustomPropertyDrawer<const Script *>
	{
		bool operator()(const String & label, const Script * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const Script *, ScriptDrawer, "script");


	// Shader Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API ShaderDrawer final : public CustomPropertyDrawer<const Shader *>
	{
		bool operator()(const String & label, const Shader * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const Shader *, ShaderDrawer, "shader");


	// Sound Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API SoundDrawer final : public CustomPropertyDrawer<const Sound *>
	{
		bool operator()(const String & label, const Sound * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const Sound *, SoundDrawer, "sound");


	// Sprite Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API SpriteDrawer final : public CustomPropertyDrawer<const Sprite *>
	{
		bool operator()(const String & label, const Sprite * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const Sprite *, SpriteDrawer, "sprite");


	// Surface Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API SurfaceDrawer final : public CustomPropertyDrawer<const Surface *>
	{
		bool operator()(const String & label, const Surface * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const Surface *, SurfaceDrawer, "surface");


	// Texture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API TextureDrawer : public CustomPropertyDrawer<const Texture *>
	{
		bool operator()(const String & label, const Texture * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(const Texture *, TextureDrawer, "texture");


	// Uniform Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_EDITOR_API UniformDrawer : public CustomPropertyDrawer<Uniform *>
	{
		bool operator()(const String & label, Uniform * value) const override;
	};
	ML_GEN_PROPERTY_DRAWER(Uniform *, UniformDrawer, "uniform");

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


#endif // !_ML_PROPERTY_DRAWER_HPP_