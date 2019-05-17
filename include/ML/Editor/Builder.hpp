#ifndef _ML_BUILDER_HPP_
#define _ML_BUILDER_HPP_

#include <ML/Editor/GUI_Window.hpp>
#include <ML/Editor/Document.hpp>

#define ML_Builder ml::Builder::getInstance()
#define ML_TEST_SHADER "built_shader"

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Shader;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Builder final
		: public GUI_Window
		, public ISingleton<Builder>
	{
		friend class ISingleton<Builder>;

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		struct BuildFile : public ITrackable, public INonCopyable
		{
			enum : size_t { MaxSize = 2048 };

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
		Builder();
		~Builder();

	public:
		/* * * * * * * * * * * * * * * * * * * * */
		void onEvent(const IEvent * value) override;
		bool drawGui(bool * p_open) override;

	private:
		/* * * * * * * * * * * * * * * * * * * * */
		BuildMap m_files;
		Shader * m_shader;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_BUILDER_HPP_