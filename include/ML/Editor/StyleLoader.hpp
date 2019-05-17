#ifndef _ML_STYLE_LOADER_HPP_
#define _ML_STYLE_LOADER_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Core/File.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API StyleLoader final
		: public ITrackable
		, public IDisposable
		, public IReadable
	{
	public:
		StyleLoader();
		~StyleLoader();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

	public:
		void serialize(std::ostream & out) const override;
		void deserialize(std::istream & in) override;

	private:
		File m_file;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STYLE_LOADER_HPP_