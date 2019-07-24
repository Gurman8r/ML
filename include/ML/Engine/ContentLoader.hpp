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
		bool loadElement(size_t index);
		bool loadAll(bool clearLists);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static bool readFile(const String & filename, List<MetaData *> & list);
		static bool readLists(List<MetaData *> & list, Istream & file, String & line);
		static bool readMetadata(MetaData *& data, Istream & file, String & line);
		static bool parseMetadata(const MetaData & data);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto lists() const -> const List<MetaData *> & { return m_lists; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: List<MetaData *> m_lists;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTENT_LOADER_HPP_