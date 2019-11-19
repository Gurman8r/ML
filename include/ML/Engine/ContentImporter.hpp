#ifndef _ML_CONTENT_IMPORTER_HPP_
#define _ML_CONTENT_IMPORTER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/Metadata.hpp>

#define ML_GEN_CONTENT_IMPORTER(T)									\
using value_type		= typename _ML detail::decay_t<T>;			\
using self_type			= typename _ML ContentImporter<value_type>;	\
using pointer			= typename value_type *;				\
using reference			= typename value_type &;					\
using const_pointer		= typename value_type const *;			\
using const_reference	= typename value_type const &;				\
static constexpr typeof<> info() { return typeof<T>{}; }			\
ContentImporter() = default;

namespace ml
{
	// Content Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... T> struct ContentImporter;

	template <class ... T> struct BaseContentImporter;

	template <> struct BaseContentImporter<>
	{
		BaseContentImporter() = delete;
	};

	template <> struct ContentImporter<> : public BaseContentImporter<>
	{
		ContentImporter() = delete;

		static bool loadMetadata(Metadata const & value);

		static int32_t loadMetadata(ArrayList<Metadata> const & value);

		template <class S>
		static inline int32_t loadMetadata(ArrayList<Dict<S, S>> const & value)
		{
			ArrayList<Metadata> md{};
			md.reserve(value.size());
			for (auto const & elem : value)
			{
				md.push_back(Metadata{ elem });
			}
			return loadMetadata(md);
		}
	};

	template <class T> struct BaseContentImporter<T>
	{
		using value_type		= typename detail::decay_t<T>;
		using self_type			= typename ContentImporter<value_type>;
		using pointer			= typename value_type *;
		using reference			= typename value_type &;
		using const_pointer		= typename value_type const *;
		using const_reference	= typename value_type const &;

		struct Info final
		{
			static constexpr auto hash	{ typeof<value_type>::hash };
			static constexpr auto name	{ typeof<value_type>::name };
			static constexpr auto brief	{ nameof<>::filter_namespace(name) };
		} info;
	};


	// Entity Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Entity; template <
	> struct ML_ENGINE_API ContentImporter<Entity> final : public BaseContentImporter<Entity>
	{
		Entity * operator()(Metadata const & md) const;
	};

	// Font Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Font; template <
	> struct ML_ENGINE_API ContentImporter<Font> final : public BaseContentImporter<Font>
	{
		Font * operator()(Metadata const & md) const;
	};


	// Image Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Image; template <
	> struct ML_ENGINE_API ContentImporter<Image> final : public BaseContentImporter<Image>
	{
		Image * operator()(Metadata const & md) const;
	};


	// Material Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Material; template <
	> struct ML_ENGINE_API ContentImporter<Material> final : public BaseContentImporter<Material>
	{
		Material * operator()(Metadata const & md) const;
	};


	// Model Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Model; template <
	> struct ML_ENGINE_API ContentImporter<Model> final : public BaseContentImporter<Model>
	{
		Model * operator()(Metadata const & md) const;
	};


	// Script Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Script; template <
	> struct ML_ENGINE_API ContentImporter<Script> final : public BaseContentImporter<Script>
	{
		Script * operator()(Metadata const & md) const;
	};


	// Shader Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Shader; template <
	> struct ML_ENGINE_API ContentImporter<Shader> final : public BaseContentImporter<Shader>
	{
		Shader * operator()(Metadata const & md) const;
	};


	// Sound Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Sound; template <
	> struct ML_ENGINE_API ContentImporter<Sound> final : public BaseContentImporter<Sound>
	{
		Sound * operator()(Metadata const & md) const;
	};


	// Sprite Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Sprite; template <
	> struct ML_ENGINE_API ContentImporter<Sprite> final : public BaseContentImporter<Sprite>
	{
		Sprite * operator()(Metadata const & md) const;
	};


	// Surface Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Surface; template <
	> struct ML_ENGINE_API ContentImporter<Surface> final : public BaseContentImporter<Surface>
	{
		Surface * operator()(Metadata const & md) const;
	};


	// Texture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Texture; template <
	> struct ML_ENGINE_API ContentImporter<Texture> final : public BaseContentImporter<Texture>
	{
		Texture * operator()(Metadata const & md) const;
	};
}

#endif // !_ML_CONTENT_IMPORTER_HPP_