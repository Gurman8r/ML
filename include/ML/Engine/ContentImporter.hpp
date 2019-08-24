#ifndef _ML_CONTENT_IMPORTER_HPP_
#define _ML_CONTENT_IMPORTER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/Metadata.hpp>

#define ML_GEN_CONTENT_IMPORTER(T)									\
using value_type		= typename _ML detail::decay_t<T>;			\
using self_type			= typename ContentImporter<value_type>;		\
using pointer			= typename value_type *;					\
using reference			= typename value_type &;					\
using const_pointer		= typename const value_type *;				\
using const_reference	= typename const value_type &;				\
static constexpr _ML StringView name() { return ML_STRINGIFY(T); }	\
static constexpr _ML hash_t		hash() { return name().hash(); }	\
ContentImporter() = default;

namespace ml
{
	// Content Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <class ... T> struct ContentImporter;

	template <> struct ContentImporter<>
	{
		struct Util;
		ContentImporter() = delete;
	};


	// Entity Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Entity;
	template <> struct ML_ENGINE_API ContentImporter<Entity> final
	{
		ML_GEN_CONTENT_IMPORTER(Entity);
		Entity * operator()(const Metadata & md) const;
	};


	// Font Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Font;
	template <> struct ML_ENGINE_API ContentImporter<Font> final
	{
		ML_GEN_CONTENT_IMPORTER(Font);
		Font * operator()(const Metadata & md) const;
	};


	// Image Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Image;
	template <> struct ML_ENGINE_API ContentImporter<Image> final
	{
		ML_GEN_CONTENT_IMPORTER(Image);
		Image * operator()(const Metadata & md) const;
	};


	// Material Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Material;
	template <> struct ML_ENGINE_API ContentImporter<Material> final
	{
		ML_GEN_CONTENT_IMPORTER(Material);
		Material * operator()(const Metadata & md) const;
	};


	// Model Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Model;
	template <> struct ML_ENGINE_API ContentImporter<Model> final
	{
		ML_GEN_CONTENT_IMPORTER(Model);
		Model * operator()(const Metadata & md) const;
	};


	// Script Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Script;
	template <> struct ML_ENGINE_API ContentImporter<Script> final
	{
		ML_GEN_CONTENT_IMPORTER(Script);
		Script * operator()(const Metadata & md) const;
	};


	// Shader Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Shader;
	template <> struct ML_ENGINE_API ContentImporter<Shader> final
	{
		ML_GEN_CONTENT_IMPORTER(Shader);
		Shader * operator()(const Metadata & md) const;
	};


	// Sound Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Sound;
	template <> struct ML_ENGINE_API ContentImporter<Sound> final
	{
		ML_GEN_CONTENT_IMPORTER(Sound);
		Sound * operator()(const Metadata & md) const;
	};


	// Sprite Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Sprite;
	template <> struct ML_ENGINE_API ContentImporter<Sprite> final
	{
		ML_GEN_CONTENT_IMPORTER(Sprite);
		Sprite * operator()(const Metadata & md) const;
	};


	// Surface Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Surface;
	template <> struct ML_ENGINE_API ContentImporter<Surface> final
	{
		ML_GEN_CONTENT_IMPORTER(Surface);
		Surface * operator()(const Metadata & md) const;
	};


	// Texture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Texture;
	template <> struct ML_ENGINE_API ContentImporter<Texture> final
	{
		ML_GEN_CONTENT_IMPORTER(Texture);
		Texture * operator()(const Metadata & md) const;
	};
}

#endif // !_ML_CONTENT_IMPORTER_HPP_