#ifndef _ML_PROPERTY_DRAWER_HPP_
#define _ML_PROPERTY_DRAWER_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Engine/Metadata.hpp>

namespace ml
{
	// Base Property Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct PropertyDrawer;

	template <class ... T> struct BasePropertyDrawer;

	template <> struct BasePropertyDrawer<>
	{
		BasePropertyDrawer() = delete;
	};

	template <
	> struct ML_EDITOR_API PropertyDrawer<> final : public BasePropertyDrawer<>
	{
		struct Layout;
		PropertyDrawer() = delete;
	};

	template <class T> struct BasePropertyDrawer<T>
	{
		using value_type		= typename _ML detail::decay_t<T>;
		using self_type			= typename _ML PropertyDrawer<value_type>;
		using pointer			= typename ptr_t<value_type>;
		using reference			= typename value_type &;
		using const_pointer		= typename const_ptr_t<value_type>;
		using const_reference	= typename const value_type &;
		using Layout			= typename PropertyDrawer<>::Layout;

		struct Info final
		{
			static constexpr auto hash	{ typeof<value_type>::hash };
			static constexpr auto name	{ typeof<value_type>::name };
			static constexpr auto brief	{ nameof<>::filter_namespace(name) };
		} info;
	};
	


	// Entity Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Entity;
	template <
	> struct ML_EDITOR_API PropertyDrawer<Entity> final : public BasePropertyDrawer<Entity>
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Font Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Font;
	template <
	> struct ML_EDITOR_API PropertyDrawer<Font> final : public BasePropertyDrawer<Font>
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Image Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Image;
	template <
	> struct ML_EDITOR_API PropertyDrawer<Image> final : public BasePropertyDrawer<Image>
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Material Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Material;
	template <
	> struct ML_EDITOR_API PropertyDrawer<Material> final : public BasePropertyDrawer<Material>
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Model Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Model;
	template <
	> struct ML_EDITOR_API PropertyDrawer<Model> final : public BasePropertyDrawer<Model>
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Script Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Script;
	template <
	> struct ML_EDITOR_API PropertyDrawer<Script> final : public BasePropertyDrawer<Script>
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Shader Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Shader;
	template <
	> struct ML_EDITOR_API PropertyDrawer<Shader> final : public BasePropertyDrawer<Shader>
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Sound Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Sound;
	template <
	> struct ML_EDITOR_API PropertyDrawer<Sound> final : public BasePropertyDrawer<Sound>
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Sprite Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Sprite;
	template <
	> struct ML_EDITOR_API PropertyDrawer<Sprite> final : public BasePropertyDrawer<Sprite>
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Surface Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Surface;
	template <
	> struct ML_EDITOR_API PropertyDrawer<Surface> final : public BasePropertyDrawer<Surface>
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Texture Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Texture;
	template <
	> struct ML_EDITOR_API PropertyDrawer<Texture> final : public BasePropertyDrawer<Texture>
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};


	// Uniform Drawer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Uniform;
	template <
	> struct ML_EDITOR_API PropertyDrawer<Uniform> final : public BasePropertyDrawer<Uniform>
	{
		bool operator()(const String & label, const_pointer & value) const;
		bool operator()(const String & label, pointer & value) const;
		bool operator()(const String & label, reference value) const;
	};
}

#endif // !_ML_PROPERTY_DRAWER_HPP_