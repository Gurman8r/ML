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

		bool loadFromFile(String const & filename);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static const List<Metadata *> & loadLists(List<Metadata *> & data, const List<Map<String, String>> & value);
		
		static bool readFile(String const & filename, List<Metadata *> & list);
		
		static bool readLists(List<Metadata *> & list, std::istream & file, String & line);
		
		static bool readMetadata(Metadata *& data, std::istream & file, String & line);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto lists() const -> const List<Metadata *> & { return m_lists; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: List<Metadata *> m_lists;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_METADATA_PARSER_HPP_