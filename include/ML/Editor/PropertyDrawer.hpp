#ifndef _ML_PROPERTY_DRAWER_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Editor/EditorEvents.hpp>
#include <ML/Core/I_Newable.hpp>
#include <ML/Core/String.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct Texture;
	struct Uniform;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <
		class T
	> struct CustomPropertyDrawer
	{
		using value_type = typename T;
		virtual int32_t operator()(const String & label, value_type * value) const = 0;
	};

	template <
		class T
	> struct PropertyDrawer;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#define ML_GEN_PROPERTY_DRAWER(TYPE, BASE) \
template <> struct ::ml::PropertyDrawer<TYPE> \
{ \
	template <class ... Args> \
	inline auto operator()(Args && ... args) \
	{ \
		return ::ml::BASE()(std::forward<Args>(args)...); \
	} \
}; \

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API TextureDrawer : public CustomPropertyDrawer<const Texture>
	{
		static constexpr CString tag { "texture" };
		int32_t operator()(const String & label, const Texture * value) const override;
	};

	ML_GEN_PROPERTY_DRAWER(const Texture, TextureDrawer);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API UniformDrawer : public CustomPropertyDrawer<Uniform>
	{
		static constexpr CString tag { "uniform" };
		int32_t operator()(const String & label, Uniform * value) const override;
	};

	ML_GEN_PROPERTY_DRAWER(Uniform, UniformDrawer);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}


#endif // !_ML_PROPERTY_DRAWER_HPP_