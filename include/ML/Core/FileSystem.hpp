#ifndef _ML_FILE_SYSTEM_HPP_
#define _ML_FILE_SYSTEM_HPP_

#include <ML/Core/File.hpp>

#define ML_FS ::ml::FileSystem::getInstance()

# if defined(ML_SYSTEM_WINDOWS)
#	define ML_MAX_PATH		260
#	define ML_PATH_DELIM	"\\"
# else
#	define ML_MAX_PATH		255
#	define ML_PATH_DELIM	"/"
# endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API FileSystem final : public Singleton<FileSystem>
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		using Directory = typename HashMap<char, List<String>>;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline const String & root() const
		{ 
			return m_root;
		}

		inline String pathTo(const String & value) const
		{
			return (m_root + ML_PATH_DELIM + value);
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool	setPath(const String & value);
		String	getPath() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool	getDirContents(const String & dirName, List<char> & value) const;
		bool	getDirContents(const String & dirName, String & value) const;
		bool	getDirContents(const String & dirName, SStream & value) const;
		bool	getDirContents(const String & dirName, Directory & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool	getFileContents(const String & filename, List<char> & value) const;
		bool	getFileContents(const String & filename, String & value) const;
		String	getFileContents(const String & filename) const;
		bool	getFileContents(const String & filename, SStream & value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		bool	dirExists(const String & dirName) const;
		bool	fileExists(const String & filename) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		String	getFileType(const String & filename) const;
		String  getFileName(const String & filename) const;
		String	getFilePath(const String & filename) const;
		size_t	getFileSize(const String & filename) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	private:
		friend struct Singleton<FileSystem>;
		
		FileSystem() : m_root { getPath() } {}
		
		~FileSystem() {}

		String m_root;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_FILE_SYSTEM_HPP_