#ifndef _ML_PROPERTY_DRAWER_HPP_
#define _ML_PROPERTY_DRAWER_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

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
	struct Uniform;
	
	/* * * * * * * * * * * * * * * * * * * * */
}

#define ML_GEN_DRAWER_DETAILS(T)									\
using value_type		= typename _ML detail::decay_t<T>;			\
using self_type			= typename PropertyDrawer<T>;				\
using pointer			= typename value_type *;					\
using reference			= typename value_type &;					\
using const_pointer		= typename const value_type *;				\
using const_reference	= typename const value_type &;				\
static constexpr _ML StringView name() { return ML_STRINGIFY(T); }	\
static constexpr _ML hash_t		hash() { return name().hash(); }	\
PropertyDrawer() = default;

namespace ml
{
	// Base
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <class ... T>
	struct PropertyDrawer;


	// Entity Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_EDITOR_API PropertyDrawer<Entity> final
	{
		ML_GEN_DRAWER_DETAILS(Entity);
		bool operator()(const String & label, const_pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, reference value, int32_t flags = 0) const;
	};


	// Font Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_EDITOR_API PropertyDrawer<Font> final
	{
		ML_GEN_DRAWER_DETAILS(Font);
		bool operator()(const String & label, const_pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, reference value, int32_t flags = 0) const;
	};


	// Image Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_EDITOR_API PropertyDrawer<Image> final
	{
		ML_GEN_DRAWER_DETAILS(Image);
		bool operator()(const String & label, const_pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, reference value, int32_t flags = 0) const;
	};


	// Material Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_EDITOR_API PropertyDrawer<Material> final
	{
		ML_GEN_DRAWER_DETAILS(Material);
		bool operator()(const String & label, const_pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, reference value, int32_t flags = 0) const;
	};


	// Model Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_EDITOR_API PropertyDrawer<Model> final
	{
		ML_GEN_DRAWER_DETAILS(Model);
		bool operator()(const String & label, const_pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, reference value, int32_t flags = 0) const;
	};


	// Script Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_EDITOR_API PropertyDrawer<Script> final
	{
		ML_GEN_DRAWER_DETAILS(Script);
		bool operator()(const String & label, const_pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, reference value, int32_t flags = 0) const;
	};


	// Shader Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_EDITOR_API PropertyDrawer<Shader> final
	{
		ML_GEN_DRAWER_DETAILS(Shader);
		bool operator()(const String & label, const_pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, reference value, int32_t flags = 0) const;
	};


	// Sound Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_EDITOR_API PropertyDrawer<Sound> final
	{
		ML_GEN_DRAWER_DETAILS(Sound);
		bool operator()(const String & label, const_pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, reference value, int32_t flags = 0) const;
	};


	// Sprite Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_EDITOR_API PropertyDrawer<Sprite> final
	{
		ML_GEN_DRAWER_DETAILS(Sprite);
		bool operator()(const String & label, const_pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, reference value, int32_t flags = 0) const;
	};


	// Surface Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_EDITOR_API PropertyDrawer<Surface> final
	{
		ML_GEN_DRAWER_DETAILS(Surface);
		bool operator()(const String & label, const_pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, reference value, int32_t flags = 0) const;
	};


	// Texture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_EDITOR_API PropertyDrawer<Texture> final
	{
		ML_GEN_DRAWER_DETAILS(Texture);
		bool operator()(const String & label, const_pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, reference value, int32_t flags = 0) const;
	};


	// Uniform Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_EDITOR_API PropertyDrawer<Uniform> final
	{
		ML_GEN_DRAWER_DETAILS(Uniform);
		bool operator()(const String & label, const_pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, pointer & value, int32_t flags = 0) const;
		bool operator()(const String & label, reference value, int32_t flags = 0) const;
	};
}

#endif // !_ML_PROPERTY_DRAWER_HPP_