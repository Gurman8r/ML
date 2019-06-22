#ifndef _ML_ASSET_IMPORTER_HPP_
#define _ML_ASSET_IMPORTER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	// WIP
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_ENGINE_API AssetImporter
		: public I_Newable
		, public I_NonCopyable
	{
	public:
		AssetImporter();
		~AssetImporter();

	private:

	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		size_t N, class T
	> struct CustomImporter;

	template <> struct CustomImporter<1, int32_t>
	{
		inline bool operator()(istream & in, int32_t & out) const
		{
			return false;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_ASSET_IMPORTER_HPP_