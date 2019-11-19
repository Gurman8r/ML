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

	Script::Script(Language language, String const & text)
		: m_lang { language }
		, m_path { }
		, m_text { text }
	{
	}

	Script::Script(String const & filename)
		: Script {}

	{
		loadFromFile(filename);
	}

	Script::Script(Script const & copy)
		: m_lang(copy.m_lang)
		, m_path(copy.m_path)
		, m_text(copy.m_text)
	{
	}

	Script::~Script() {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool Script::loadFromFile(String const & filename)
	{
		if (m_text = ML_FS.getFileContents(m_path = filename))
		{
			switch (ML_FS.getFileType(m_path).hash())
			{
			case Hash("py"): m_lang = Language::Python; break;
			case Hash("lua"): m_lang = Language::Lua; break;
			default: m_lang = Language::Unknown; break;
			}
			return true;
		}
		return false;
	}

	int32_t Script::execute() const
	{
		switch (m_lang)
		{
			case Language::Lua: return ML_Lua.doString(m_text);
			case Language::Python: return ML_Py.doString(m_text);
			default: return 0;
		}
	}

	Script & Script::setLanguage(Language value)
	{
		m_lang = value;
		return (*this);
	}

	Script & Script::setText(String const & value)
	{
		m_text = value;
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}