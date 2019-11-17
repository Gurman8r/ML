#ifndef _ML_CONTENT_IMPORTER_HPP_
#define _ML_CONTENT_IMPORTER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/Metadata.hpp>

#define ML_GEN_CONTENT_IMPORTER(T)									\
using value_type		= typename _ML detail::decay_t<T>;			\
using self_type			= typename _ML ContentImporter<value_type>;	\
using pointer			= typename ptr_t<value_type>;				\
using reference			= typename value_type &;					\
using const_pointer		= typename const_ptr_t<value_type>;			\
using const_reference	= typename const value_type &;				\
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

		static bool loadMetadata(const Metadata & value);

		static int32_t loadMetadata(const List<Metadata> & value);

		template <class S>
		static inline int32_t loadMetadata(const List<Map<S, S>> & value)
		{
			List<Metadata> md{};
			md.reserve(value.size());
			for (const auto & elem : value)
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
		using pointer			= typename ptr_t<value_type>;
		using reference			= typename value_type &;
		using const_pointer		= typename const_ptr_t<value_type>;
		using const_reference	= typename const value_type &;

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
		ptr_t<Entity> operator()(const Metadata & md) const;
	};

	// Font Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Font; template <
	> struct ML_ENGINE_API ContentImporter<Font> final : public BaseContentImporter<Font>
	{
		ptr_t<Font> operator()(const Metadata & md) const;
	};


	// Image Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Image; template <
	> struct ML_ENGINE_API ContentImporter<Image> final : public BaseContentImporter<Image>
	{
		ptr_t<Image> operator()(const Metadata & md) const;
	};


	// Material Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Material; template <
	> struct ML_ENGINE_API ContentImporter<Material> final : public BaseContentImporter<Material>
	{
		ptr_t<Material> operator()(const Metadata & md) const;
	};


	// Model Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Model; template <
	> struct ML_ENGINE_API ContentImporter<Model> final : public BaseContentImporter<Model>
	{
		ptr_t<Model> operator()(const Metadata & md) const;
	};


	// Script Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Script; template <
	> struct ML_ENGINE_API ContentImporter<Script> final : public BaseContentImporter<Script>
	{
		ptr_t<Script> operator()(const Metadata & md) const;
	};


	// Shader Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Shader; template <
	> struct ML_ENGINE_API ContentImporter<Shader> final : public BaseContentImporter<Shader>
	{
		ptr_t<Shader> operator()(const Metadata & md) const;
	};


	// Sound Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Sound; template <
	> struct ML_ENGINE_API ContentImporter<Sound> final : public BaseContentImporter<Sound>
	{
		ptr_t<Sound> operator()(const Metadata & md) const;
	};


	// Sprite Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Sprite; template <
	> struct ML_ENGINE_API ContentImporter<Sprite> final : public BaseContentImporter<Sprite>
	{
		ptr_t<Sprite> operator()(const Metadata & md) const;
	};


	// Surface Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Surface; template <
	> struct ML_ENGINE_API ContentImporter<Surface> final : public BaseContentImporter<Surface>
	{
		ptr_t<Surface> operator()(const Metadata & md) const;
	};


	// Texture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct Texture; template <
	> struct ML_ENGINE_API ContentImporter<Texture> final : public BaseContentImporter<Texture>
	{
		ptr_t<Texture> operator()(const Metadata & md) const;
	};
}

#endif // !_ML_CONTENT_IMPORTER_HPP_