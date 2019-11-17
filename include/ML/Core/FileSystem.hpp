#ifndef _ML_FILE_SYSTEM_HPP_
#define _ML_FILE_SYSTEM_HPP_

#include <ML/Core/File.hpp>

# ifdef ML_SYSTEM_WINDOWS
#	include <dirent/include/dirent.h>
#else
#	include <dirent.h>
#endif

#define ML_FS ::ml::FileSystem::getInstance()

#if defined(ML_SYSTEM_WINDOWS)
#	define ML_MAX_PATH		260
#	define ML_PATH_DELIM	"\\"
#else
#	define ML_MAX_PATH		255
#	define ML_PATH_DELIM	"/"
#endif

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct FileSystem final : public Singleton<FileSystem>
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

		inline bool setPath(const String & value)
		{
			if (dirExists(value))
			{
				std::filesystem::current_path(value.c_str());
				return true;
			}
			return false;
		}

		inline String getPath() const
		{
			return std::filesystem::current_path().string();
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool getDirContents(const String & dirName, List<char> & value) const
		{
			static String temp;
			if (getDirContents(dirName, temp))
			{
				value.assign(temp.begin(), temp.end());
				return true;
			}
			value.clear();
			return false;
		}

		inline bool getDirContents(const String & dirName, String & value) const
		{
			static SStream temp;
			if (getDirContents(dirName, temp))
			{
				value.assign(temp.str());
				return true;
			}
			value.clear();
			return false;
		}

		inline bool getDirContents(const String & dirName, SStream & value) const
		{
			value.str(String());
			if(DIR * dir = opendir(dirName.c_str()))
			{
				while (dirent * e = readdir(dir))
				{
					switch (e->d_type)
					{
					case DT_DIR	: value << e->d_name << "/" << endl; break;
					default		: value << e->d_name << " " << endl; break;
					}
				}
				closedir(dir);
				return true;
			}
			return false;
		}

		inline bool getDirContents(const String & dirName, Directory & value) const
		{
			value.clear();
			if (DIR * dir = opendir(dirName.c_str()))
			{
				while (dirent * e = readdir(dir))
				{
					char type;
					switch (e->d_type)
					{
					case DT_DIR: (type = '/'); break;
					case DT_REG: (type = ' '); break;
					case DT_LNK: (type = '@'); break;
					default:	 (type = '*'); break;
					}
					value[type].push_back(e->d_name);
				}
				closedir(dir);
				return true;
			}
			return false;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool getFileContents(const String & filename, List<char> & value) const
		{
			File file;
			file.loadFromFile(filename);
			value = file.data();
			return file;
		}

		inline bool getFileContents(const String & filename, String & value) const
		{
			File file;
			file.loadFromFile(filename);
			value = file.str();
			return file;
		}

		inline String getFileContents(const String & filename) const
		{
			String temp;
			getFileContents(filename, temp);
			return temp;
		}

		inline bool getFileContents(const String & filename, SStream & value) const
		{
			File file;
			file.loadFromFile(filename);
			value = file.sstr();
			return file;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		inline bool dirExists(const String & name) const
		{
			struct stat info;
			return 
				(stat(name.c_str(), &info) == EXIT_SUCCESS) && 
				(info.st_mode & S_IFDIR);
		}

		inline bool fileExists(const String & filename) const
		{
			return (bool)(std::ifstream(filename));
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	
		inline String getFileType(const String & value) const
		{
			size_t i;
			return
				((i = value.find_last_of('.')) != String::npos) ||
				((i = value.find_last_of('/')) != String::npos) ||
				((i = value.find_last_of('\\')) != String::npos)
				? String(value.substr(i + 1, value.size() - i - 1))
				: value;
		}

		inline String getFileName(const String & value) const
		{
			size_t i;
			return
				((i = value.find_last_of('/')) != String::npos) ||
				((i = value.find_last_of('\\')) != String::npos)
				? String(value.substr(i + 1, value.size() - i - 1))
				: value;
		}

		inline String getFilePath(const String & value) const
		{
			size_t i;
			return (
				((i = value.find_last_of('/')) != String::npos) ||
				((i = value.find_last_of('\\')) != String::npos)
				? String(value.substr(0, i))
				: value
			);
		}

		inline size_t getFileSize(const String & value) const
		{
			std::ifstream stream;
			return (stream = std::ifstream(value, std::ifstream::ate | std::ifstream::binary))
				? (size_t)stream.tellg()
				: 0;
		}

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