/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Launcher/Launcher.hpp>

#include <ML/Engine/Entity.hpp>

namespace ml
{
	using uniform_variant = typename std::variant<
		bool, int32_t, float_t,
		vec2, vec3, vec4, mat3, mat4,
		const Texture *
	>;

	template <
		class T, class ... Args
	> static constexpr uniform_variant make_uniform(Args && ... args)
	{
		return uniform_variant { T { std::forward<Args>(args)... } };
	}

	template <
		class T
	> static constexpr T * uniform_cast(uniform_variant & value)
	{
		if (std::holds_alternative<T>(value))
		{
			return static_cast<T *>(&std::get<T>(value));
		}
		return nullptr;
	}

	template <
		class T
	> static constexpr const T * uniform_cast(const uniform_variant & value)
	{
		if (std::holds_alternative<T>(value))
		{
			return static_cast<const T *>(&std::get<T>(value));
		}
		return nullptr;
	}

	template <
		class ... U
	> static constexpr int32_t uniform_type(const std::variant<U...> & value)
	{
		if (std::holds_alternative<const Texture *>(value)) return 8;
		else if (std::holds_alternative<mat4>(value)) return 7;
		else if (std::holds_alternative<mat3>(value)) return 6;
		else if (std::holds_alternative<vec4>(value)) return 5;
		else if (std::holds_alternative<vec3>(value)) return 4;
		else if (std::holds_alternative<vec2>(value)) return 3;
		else if (std::holds_alternative<int32_t>(value)) return 2;
		else if (std::holds_alternative<float_t>(value)) return 1;
		else if (std::holds_alternative<bool>(value)) return 0;
		else return -1;
	}

	template <
		class ... U
	> static constexpr C_String uniform_name(const std::variant<U...> & value)
	{
		switch (uniform_type(value))
		{
		case 0: return "bool";
		case 1: return "float";
		case 2: return "int";
		case 3: return "vec2";
		case 4: return "vec3";
		case 5: return "vec4";
		case 6: return "mat3";
		case 7: return "mat4";
		case 8: return "sampler";
		default: return "";
		}
	}
}

ml::int32_t main()
{
	using namespace ml;

	//uniform_variant u;
	//cout << uniform_name(u = false) << endl;
	//cout << uniform_name(u = 1) << endl;
	//cout << uniform_name(u = 1.0f) << endl;
	//cout << uniform_name(u = vec2::one()) << endl;
	//cout << uniform_name(u = vec3::one()) << endl;
	//cout << uniform_name(u = vec4::one()) << endl;
	//cout << uniform_name(u = mat3::one()) << endl;
	//cout << uniform_name(u = mat4::one()) << endl;
	//cout << uniform_name(u = (const Texture *)nullptr) << endl;

	//return Debug::pause(0);

	static_assert(typeof<int32_t>::name		== "int");
	static_assert(typeof<uint32_t>::name	== "unsigned int");
	static_assert(typeof<float_t>::name		== "float");
	static_assert(typeof<C_String>::name	== "const char*");
	static_assert(typeof<Debug>::name		== "struct ml::Debug");
	static_assert(typeof<std::string>::name == "class std::string");
	static_assert(typeof<String>::name		== "struct ml::BasicString<char>");
	static_assert(typeof<List<String>>::name== "struct ml::List<struct ml::BasicString<char> >");
	
	return ML_Launcher(ML_ARGC, ML_ARGV);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */