#ifndef _ML_CONTENT_LOADER_HPP_
#define _ML_CONTENT_LOADER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/MetaData.hpp>
#include <ML/Core/I_Readable.hpp>
#include <ML/Core/I_Disposable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API ContentLoader final
		: public I_Readable
		, public I_Disposable
		, public I_NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		ContentLoader();
		~ContentLoader();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static bool readFile(const String & filename, List<MetaData *> & list);
		static bool readLists(List<MetaData *> & list, Istream & file, String & line);
		static bool readMetadata(MetaData *& data, Istream & file, String & line);
		static bool parseMetadata(const MetaData & data);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: List<MetaData *> m_list;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTENT_LOADER_HPP_