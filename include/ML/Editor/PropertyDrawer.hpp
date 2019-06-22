#ifndef _ML_PROPERTY_DRAWER_HPP_

#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct Uniform;

	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API PropertyDrawer
		: public I_Newable
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		PropertyDrawer();
		virtual ~PropertyDrawer();

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	template <
		class T
	> struct CustomPropertyDrawer : public PropertyDrawer
	{
		virtual bool render(CString label, T * value) const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API UniformDrawer : public CustomPropertyDrawer<Uniform>
	{
		using value_type = typename Uniform;

		bool render(CString label, Uniform * value) const override;
	};

	/* * * * * * * * * * * * * * * * * * * * */
}


#endif // !_ML_PROPERTY_DRAWER_HPP_