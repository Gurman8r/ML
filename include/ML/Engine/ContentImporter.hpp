#ifndef _ML_CONTENT_IMPORTER_HPP_
#define _ML_CONTENT_IMPORTER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/Metadata.hpp>

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

#define ML_GEN_IMPORTER_DETAILS(T)									\
using value_type		= typename _ML detail::decay_t<T>;			\
using self_type			= typename ContentImporter<T>;				\
using pointer			= typename value_type *;					\
using reference			= typename value_type &;					\
using const_pointer		= typename const value_type *;				\
using const_reference	= typename const value_type &;				\
static constexpr _ML StringView name() { return ML_STRINGIFY(T); }	\
static constexpr _ML hash_t		hash() { return name().hash(); }	\
ContentImporter() = default;

namespace ml
{
	// Base
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <class ... T>
	struct ContentImporter;


	// Entity Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_ENGINE_API ContentImporter<Entity> final
	{
		ML_GEN_IMPORTER_DETAILS(Entity);
		Entity * operator()(const Metadata & md) const;
	};


	// Font Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_ENGINE_API ContentImporter<Font> final
	{
		ML_GEN_IMPORTER_DETAILS(Font);
		Font * operator()(const Metadata & md) const;
	};


	// Image Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_ENGINE_API ContentImporter<Image> final
	{
		ML_GEN_IMPORTER_DETAILS(Image);
		Image * operator()(const Metadata & md) const;
	};


	// Material Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_ENGINE_API ContentImporter<Material> final
	{
		ML_GEN_IMPORTER_DETAILS(Material);
		Material * operator()(const Metadata & md) const;
	};


	// Model Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_ENGINE_API ContentImporter<Model> final
	{
		ML_GEN_IMPORTER_DETAILS(Model);
		Model * operator()(const Metadata & md) const;
	};


	// Script Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_ENGINE_API ContentImporter<Script> final
	{
		ML_GEN_IMPORTER_DETAILS(Script);
		Script * operator()(const Metadata & md) const;
	};


	// Shader Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_ENGINE_API ContentImporter<Shader> final
	{
		ML_GEN_IMPORTER_DETAILS(Shader);
		Shader * operator()(const Metadata & md) const;
	};


	// Sound Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_ENGINE_API ContentImporter<Sound> final
	{
		ML_GEN_IMPORTER_DETAILS(Sound);
		Sound * operator()(const Metadata & md) const;
	};


	// Sprite Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_ENGINE_API ContentImporter<Sprite> final
	{
		ML_GEN_IMPORTER_DETAILS(Sprite);
		Sprite * operator()(const Metadata & md) const;
	};


	// Surface Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_ENGINE_API ContentImporter<Surface> final
	{
		ML_GEN_IMPORTER_DETAILS(Surface);
		Surface * operator()(const Metadata & md) const;
	};


	// Texture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <> struct ML_ENGINE_API ContentImporter<Texture> final
	{
		ML_GEN_IMPORTER_DETAILS(Texture);
		Texture * operator()(const Metadata & md) const;
	};
}

#endif // !_ML_CONTENT_IMPORTER_HPP_