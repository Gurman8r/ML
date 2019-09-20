#include <ML/Engine/Script.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Lua.hpp>
#include <ML/Engine/Python.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Script::Script()
		: Script(Language::Python)
	{
	}

	Script::Script(Language language)
		: m_language(language)
		, m_filename()
		, m_text()
	{
	}

	Script::Script(const String & filename)
		: Script()

	{
		loadFromFile(filename);
	}

	Script::Script(const Script & copy)
		: m_language(copy.m_language)
		, m_filename(copy.m_filename)
		, m_text(copy.m_text)
	{
	}

	Script::~Script() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Script::loadFromFile(const String & filename)
	{
		if (m_text = ML_FS.getFileContents(m_filename = filename))
		{
			return true;
		}
		return false;
	}

	int32_t Script::execute() const
	{
		switch (m_language)
		{
		case Language::Lua: return 0;
		case Language::Python: return Py::RunString(m_text);
		default: return 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Script & Script::setLanguage(Language value)
	{
		m_language = value;
		return (*this);
	}

	Script & Script::setText(const String & value)
	{
		m_text = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}