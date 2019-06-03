#ifndef _ML_BUILDER_GUI_HPP_
#define _ML_BUILDER_GUI_HPP_

#include <ML/Editor/EditorGui.hpp>
#include <ML/Core/File.hpp>

#define ML_TEST_SHADER "built_shader"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class Shader;

	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API BuilderGui final 
		: public EditorGui
	{
		friend class Editor;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		struct BuildFile : public IObject, public INonCopyable
		{
			enum : size_t 
			{
				MaxName = 32,
				MaxSize = 2048 
			};

			using Source = typename char[MaxSize];

			String	name;
			Source	text;
			bool	open;
			bool	dirty;

			BuildFile(const String & name, const String & text = String())
				: name(name)
				, text()
				, open(true)
				, dirty(false)
			{
				if (text) { std::strcpy(this->text, text.c_str()); }
			}
		};

		using BuildMap = typename List<BuildFile *>;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		explicit BuilderGui(EventSystem & eventSystem);
		~BuilderGui();

	protected:
		/* * * * * * * * * * * * * * * * * * * * */
		bool drawGui(const GuiEvent & ev) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		BuildMap m_files;
		Shader * m_shader;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUILDER_GUI_HPP_