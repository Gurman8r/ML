#ifndef _ML_ASSET_IMPORTER_HPP_
#define _ML_ASSET_IMPORTER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Readable.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/List.hpp>
#include <ML/Core/Metadata.hpp>

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

	// Base Importers
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	template <
		class T
	> struct CustomAssetImporter
	{
		using	value_type = typename T;
		virtual value_type * operator()(const Metadata & md) const = 0;
	};

	template <
		class T
	> struct AssetImporter;


	// CubeMap Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API CubeMapImporter final : public CustomAssetImporter<CubeMap>
	{
		static constexpr CString tag { "cubemap" };
		CubeMap * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<CubeMap>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return CubeMapImporter()(std::forward<Args>(args)...);
		}
	};


	// Entity Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API EntityImporter final : public CustomAssetImporter<Entity>
	{
		static constexpr CString tag { "entity" };
		Entity * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Entity>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return EntityImporter()(std::forward<Args>(args)...);
		}
	};


	// Font Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API FontImporter final : public CustomAssetImporter<Font>
	{
		static constexpr CString tag { "font" };
		Font * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Font>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return FontImporter()(std::forward<Args>(args)...);
		}
	};


	// Image Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API ImageImporter final : public CustomAssetImporter<Image>
	{
		static constexpr CString tag { "image" };
		Image * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Image>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return ImageImporter()(std::forward<Args>(args)...);
		}
	};


	// Material Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API MaterialImporter final : public CustomAssetImporter<Material>
	{
		static constexpr CString tag { "material" };
		Material * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Material>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return MaterialImporter()(std::forward<Args>(args)...);
		}
	};


	// Mesh Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API MeshImporter final : public CustomAssetImporter<Mesh>
	{
		static constexpr CString tag { "mesh" };
		Mesh * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Mesh>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return MeshImporter()(std::forward<Args>(args)...);
		}
	};


	// Model Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API ModelImporter final : public CustomAssetImporter<Model>
	{
		static constexpr CString tag { "model" };
		Model * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Model>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return ModelImporter()(std::forward<Args>(args)...);
		}
	};


	// Script Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API ScriptImporter final : public CustomAssetImporter<Script>
	{
		static constexpr CString tag { "script" };
		Script * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Script>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return ScriptImporter()(std::forward<Args>(args)...);
		}
	};


	// Shader Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API ShaderImporter final : public CustomAssetImporter<Shader>
	{
		static constexpr CString tag { "shader" };
		Shader * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Shader>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return ShaderImporter()(std::forward<Args>(args)...);
		}
	};


	// Sound Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API SoundImporter final : public CustomAssetImporter<Sound>
	{
		static constexpr CString tag { "sound" };
		Sound * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Sound>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return SoundImporter()(std::forward<Args>(args)...);
		}
	};


	// Sprite Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API SpriteImporter final : public CustomAssetImporter<Sprite>
	{
		static constexpr CString tag { "sprite" };
		Sprite * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Sprite>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return SpriteImporter()(std::forward<Args>(args)...);
		}
	};


	// Surface Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API SurfaceImporter final : public CustomAssetImporter<Surface>
	{
		static constexpr CString tag { "surface" };
		Surface * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Surface>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return SurfaceImporter()(std::forward<Args>(args)...);
		}
	};


	// Texture Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API TextureImporter final : public CustomAssetImporter<Texture>
	{
		static constexpr CString tag { "texture" };
		Texture * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Texture>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return TextureImporter()(std::forward<Args>(args)...);
		}
	};

	
	// Uniform Importer
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	struct ML_ENGINE_API UniformImporter final : public CustomAssetImporter<Uniform>
	{
		static constexpr CString tag { "uniform" };
		Uniform * operator()(const Metadata & md) const override;
	};

	template <> struct AssetImporter<Uniform>
	{
		template <
			class ... Args
		> inline auto operator()(Args && ... args) const
		{
			return UniformImporter()(std::forward<Args>(args)...);
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_IMPORTER_HPP_