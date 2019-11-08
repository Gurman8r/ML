#ifndef _ML_PROPERTY_DRAWER_HPP_
#define _ML_PROPERTY_DRAWER_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Engine/Metadata.hpp>

namespace ml
{
	template <class T> struct PropertyInfo final
	{
		constexpr PropertyInfo() = default;

		static constexpr auto hash { typeof<T>::hash };

		static constexpr auto name { typeof<T>::name };

		static constexpr auto brief_name { nameof<>::filter_namespace(name) };
	};
}

#define ML_GEN_PROPERTY_DRAWER(T)									\
using value_type		= typename _ML detail::decay_t<T>;			\
using self_type			= typename _ML PropertyDrawer<value_type>;	\
using pointer			= typename value_type *;					\
using reference			= typename value_type &;					\
using const_pointer		= typename const value_type *;				\
using const_reference	= typename const value_type &;				\
using Layout = typename PropertyDrawer<>::Layout;					\
static constexpr auto info() { return PropertyInfo<T>{}; }			\
PropertyDrawer() = default;

namespace ml
{
	// Property Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <class ... T> struct PropertyDrawer;

	template <> struct ML_EDITOR_API PropertyDrawer<>
	{
		struct Layout; PropertyDrawer() = delete;
	};


	// Entity Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Entity;
	template <> struct ML_EDITOR_API PropertyDrawer<Entity> final
	{
		ML_GEN_PROPERTY_DRAWER(Entity);
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Font Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Font;
	template <> struct ML_EDITOR_API PropertyDrawer<Font> final
	{
		ML_GEN_PROPERTY_DRAWER(Font);
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Image Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Image;
	template <> struct ML_EDITOR_API PropertyDrawer<Image> final
	{
		ML_GEN_PROPERTY_DRAWER(Image);
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Material Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Material;
	template <> struct ML_EDITOR_API PropertyDrawer<Material> final
	{
		ML_GEN_PROPERTY_DRAWER(Material);
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Model Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Model;
	template <> struct ML_EDITOR_API PropertyDrawer<Model> final
	{
		ML_GEN_PROPERTY_DRAWER(Model);
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Script Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Script;
	template <> struct ML_EDITOR_API PropertyDrawer<Script> final
	{
		ML_GEN_PROPERTY_DRAWER(Script);
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Shader Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Shader;
	template <> struct ML_EDITOR_API PropertyDrawer<Shader> final
	{
		ML_GEN_PROPERTY_DRAWER(Shader);
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Sound Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Sound;
	template <> struct ML_EDITOR_API PropertyDrawer<Sound> final
	{
		ML_GEN_PROPERTY_DRAWER(Sound);
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Sprite Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Sprite;
	template <> struct ML_EDITOR_API PropertyDrawer<Sprite> final
	{
		ML_GEN_PROPERTY_DRAWER(Sprite);
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Surface Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Surface;
	template <> struct ML_EDITOR_API PropertyDrawer<Surface> final
	{
		ML_GEN_PROPERTY_DRAWER(Surface);
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Texture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Texture;
	template <> struct ML_EDITOR_API PropertyDrawer<Texture> final
	{
		ML_GEN_PROPERTY_DRAWER(Texture);
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Uniform Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Uniform;
	template <> struct ML_EDITOR_API PropertyDrawer<Uniform> final
	{
		ML_GEN_PROPERTY_DRAWER(Uniform);
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};
}

#endif // !_ML_PROPERTY_DRAWER_HPP_