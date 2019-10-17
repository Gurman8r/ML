#include <ML/Engine/Script.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Engine/Lua.hpp>
#include <ML/Engine/Python.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Script::Script()
		: Script { Language::Unknown }
	{
	}

	Script::Script(Language language)
		: Script { language, String() }
	{
	}

	Script::Script(Language language, const String & text)
		: m_lang { language }
		, m_path { }
		, m_text { text }
	{
	}

	Script::Script(const String & filename)
		: Script {}

	{
		loadFromFile(filename);
	}

	Script::Script(const Script & copy)
		: m_lang(copy.m_lang)
		, m_path(copy.m_path)
		, m_text(copy.m_text)
	{
	}

	Script::~Script() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Script::loadFromFile(const String & filename)
	{
		if (m_text = ML_FS.getFileContents(m_path = filename))
		{
			switch (ML_FS.getFileType(m_path).hash())
			{
			case Hash { "py" }: m_lang = Language::Python; break;
			case Hash { "lua" }: m_lang = Language::Lua; break;
			default: m_lang = Language::Unknown; break;
			}
			return true;
		}
		return false;
	}

	int32_t Script::execute() const
	{
		return execute({});
	}

	int32_t Script::execute(const List<String>& args) const
	{
		switch (m_lang)
		{
		case Language::Lua: return ML_Lua.doString(m_text);
		case Language::Python: return ML_Py.doString(m_text);
		default: return 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Script & Script::setLanguage(Language value)
	{
		m_lang = value;
		return (*this);
	}

	Script & Script::setText(const String & value)
	{
		m_text = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}