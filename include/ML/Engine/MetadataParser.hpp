#ifndef _ML_METADATA_PARSER_HPP_
#define _ML_METADATA_PARSER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/Metadata.hpp>
#include <ML/Core/Disposable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API MetadataParser final : public Disposable, public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MetadataParser();
		explicit MetadataParser(const List<Metadata *> & data);
		~MetadataParser();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose() override;
		bool loadFromFile(const String & filename);
		bool loadElement(size_t index);
		bool loadAll(bool clearLists);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static const List<Metadata *> & loadLists(List<Metadata *> & data, const List<Map<String, String>> & value);
		static bool readFile(const String & filename, List<Metadata *> & list);
		static bool readLists(List<Metadata *> & list, std::istream & file, String & line);
		static bool readMetadata(Metadata *& data, std::istream & file, String & line);
		static bool parseMetadata(const Metadata & data);

		template <class S> inline static bool parseMetadata(const List<Map<S, S>> & value)
		{
			size_t count { 0 };
			for (const auto & elem : value)
			{
				count += parseMetadata(Metadata { elem });
			}
			return count;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto lists() const -> const List<Metadata *> & { return m_lists; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: List<Metadata *> m_lists;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_METADATA_PARSER_HPP_