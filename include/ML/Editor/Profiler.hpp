#ifndef _ML_PROFILER_HPP_
#define _ML_PROFILER_HPP_

#include <ML/Editor/BaseWidget.hpp>
#include <ML/Core/Vector2.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	class ML_EDITOR_API Profiler
		: public BaseWidget
	{
		friend class Editor;

	private:
		Profiler();
		~Profiler();

	public:
		void onEvent(const IEvent * value) override;
		bool drawGui(const GuiEvent * ev, bool * p_open) override;

		struct GraphLines
		{
			enum { Size = 90U };

			float	values[Size]= { 0 };
			int32_t	offset		= 0;
			float	refresh		= 0.0f;
			float	min			= 1.f;
			float	max			= 128.f;
			vec2	size		= { 0, 80 };

			void update(CString label, const float sample, const String & fmt = "{0}");
		};

	private:
		GraphLines graph;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_PROFILER_HPP_
