#ifndef _ML_CONTENT_LOADER_HPP_
#define _ML_CONTENT_LOADER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Core/Metadata.hpp>
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
		explicit ContentLoader(const List<Metadata *> & data);
		~ContentLoader();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename) override;
		bool loadElement(size_t index);
		bool loadAll(bool clearLists);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static const List<Metadata *> & loadLists(List<Metadata *> & data, const List<Tree<String, String>> & value);
		static bool readFile(const String & filename, List<Metadata *> & list);
		static bool readLists(List<Metadata *> & list, Istream & file, String & line);
		static bool readMetadata(Metadata *& data, Istream & file, String & line);
		static bool parseMetadata(const Metadata & data);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto lists() const -> const List<Metadata *> & { return m_lists; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: List<Metadata *> m_lists;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CONTENT_LOADER_HPP_