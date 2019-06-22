#ifndef _ML_ASSET_IMPORTER_HPP_
#define _ML_ASSET_IMPORTER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/I_Readable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */
	
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

	/* * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct CustomImporter
		: public I_Newable
		, public I_Readable
	{
		virtual ~CustomImporter() {}

		virtual bool loadFromFile(const String & filename) override = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API AssetImporter
		: public I_Newable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		AssetImporter();
		~AssetImporter();

		template <
			class T
		> inline T * load()
		{
			CustomImporter<T> in;
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */

	struct MeshImporter : public CustomImporter<Mesh>
	{
		bool loadFromFile(const String & filename) override { return false; }
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_IMPORTER_HPP_