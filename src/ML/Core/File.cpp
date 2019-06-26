#include <ML/Core/File.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Debug.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	File::File()
		: m_data()
		, m_path()
	{
	}

	File::File(const size_t count, const C_String * data)
		: m_data()
		, m_path()
	{
		for (size_t i = 0; i < count; i++)
		{
			String line(data[i]);

			line = String::ReplaceAll(line, "\0", "");
			
			m_data.insert(end(), line.begin(), line.end());
		}
		m_data.push_back('\0');
	}

	File::File(const String & data)
		: m_data(data.begin(), data.end())
		, m_path()
	{
	}

	File::File(const List<char> & data)
		: m_data(data)
		, m_path()
	{
	}

	File::File(const File & copy)
		: m_data(copy.m_data)
		, m_path(copy.m_path)
	{
	}

	File::~File()
	{
		dispose();
	}

	/* * * * * * * * * * * * * * * * * * * * */

	bool File::dispose()
	{
		m_data.clear();
		return empty();
	}

	bool File::loadFromFile(const String & filename)
	{
		if (auto file = Ifstream((m_path = filename), std::ios_base::binary))
		{
			if (dispose())
			{
				file.seekg(0, std::ios_base::end);

				streamsize size;
				if ((size = file.tellg()) > 0)
				{
					file.seekg(0, std::ios_base::beg);

					m_data.resize(static_cast<size_t>(size));

					file.read(&m_data[0], size);
				}

				m_data.push_back('\0');
			}

			file.close();
			return true;
		}
		return !dispose();
	}

	bool File::saveToFile(const String & filename) const
	{
		if (Ofstream stream { filename, std::ios_base::binary })
		{
			stream << (*this);
			stream.close();
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}