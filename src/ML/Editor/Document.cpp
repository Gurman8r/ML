#include <ML/Editor/Document.hpp>
#include <imgui/imgui.h>

namespace ml
{
	// SourceFile
	/* * * * * * * * * * * * * * * * * * * * */

	Document::Document(CString name)
		: Document(name, 0)
	{
	}

	Document::Document(CString name, const char value)
		: data()
		, name()
		, open(true)
		, dirty(false)
	{
		strcpy(this->name, name);

		memset(data, value, sizeof(data));
	}

	Document::Document(CString name, const String & value)
		: Document(name, value.data(), value.size())
	{
	}

	Document::Document(CString name, CString value, const size_t size)
		: Document(name, value, size, 0)
	{
	}

	Document::Document(CString name, CString value, const size_t size, const size_t first)
		: Document(name)
	{
		size_t i;
		for (i = 0; i < size; i++)
		{
			data[first + i] = value[i];
		}
		for (i; i < sizeMax(); i++)
		{
			data[first + i] = 0;
		}
	}

	Document::Document(const Document & copy)
		: open(copy.open)
		, dirty(copy.dirty)
	{
		memcpy(name, copy.name, sizeof(name));
		memcpy(data, copy.data, sizeof(data));
	}

	/* * * * * * * * * * * * * * * * * * * * */

	size_t Document::sizeUsed() const
	{
		const_iterator it;
		return (((it = find(0)) != end())
			? (it - begin())
			: (0));
	}

	size_t Document::Document::sizeMax() const
	{
		return IM_ARRAYSIZE(data);
	}

	/* * * * * * * * * * * * * * * * * * * * */
}