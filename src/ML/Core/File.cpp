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

	File::File(const size_t count, const CString * data)
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
		if (auto in = std::ifstream((m_path = filename), std::ios_base::binary))
		{
			if (dispose())
			{
				in.seekg(0, std::ios_base::end);

				std::streamsize size;
				if ((size = in.tellg()) > 0)
				{
					in.seekg(0, std::ios_base::beg);

					m_data.resize(static_cast<size_t>(size));

					in.read(&m_data[0], size);
				}

				m_data.push_back('\0');
			}

			in.close();
			return true;
		}
		return !dispose();
	}

	bool File::saveToFile(const String & filename) const
	{
		if (auto stream = std::ofstream(filename, std::ios_base::binary))
		{
			stream << (*this);
			stream.close();
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */

	void File::serialize(ostream & out) const
	{
		out << String(begin(), end());
	}

	/* * * * * * * * * * * * * * * * * * * * */
}