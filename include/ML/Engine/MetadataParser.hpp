#ifndef _ML_METADATA_PARSER_HPP_
#define _ML_METADATA_PARSER_HPP_

#include <ML/Engine/Export.hpp>
#include <ML/Engine/Metadata.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_ENGINE_API MetadataParser final : public NonCopyable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		MetadataParser();
		explicit MetadataParser(ArrayList<Metadata *> const & data);
		~MetadataParser() { this->dispose(); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool dispose();

		bool loadFromFile(String const & filename);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		
		static ArrayList<Metadata *> const & loadLists(ArrayList<Metadata *> & data, ArrayList<Dict<String, String>> const & value);
		
		static bool readFile(String const & filename, ArrayList<Metadata *> & list);
		
		static bool readLists(ArrayList<Metadata *> & list, std::istream & file, String & line);
		
		static bool readMetadata(Metadata *& data, std::istream & file, String & line);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline auto lists() const -> ArrayList<Metadata *> const & { return m_lists; }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private: ArrayList<Metadata *> m_lists;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_METADATA_PARSER_HPP_