#ifndef _ML_STRING_UTILITY_HPP_
#define _ML_STRING_UTILITY_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/String.hpp>
#include <ML/Core/List.hpp>


namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// this whlole thing is awful and it should be removed -Melody
	class ML_CORE_API StringUtility final
	{
	public:
		static String	Replace(const String & src, const String & find, const String & replace);
		static String	Sprintf(const String fmt, ...);	
		static String	ToLower(const String & src);
		static String	ToUpper(const String & src);

		static List<String> Split(const String & value, const String & delim);

	public: // Checks
		static bool		IsAlpha(const String & src);
		static bool		IsAlnum(const String & src);
		static bool		IsBool(const String & src);
		static bool		IsDecimal(const String & src);
		static bool		IsInt(const String & src);
		static bool		IsLower(const String & src);
		static bool		IsName(const String & src);
		static bool		IsNumber(const String & src);
		static bool		IsText(const String & src);
		static bool		IsUpper(const String & src);
		static bool		IsWrap(const String & src, char c);
		static bool		IsWrap(const String & src, char front, char back);
		static bool		IsWrap(const String & src, const String & s);
		static bool		IsWrap(const String & src, const String & front, const String & back);

	public: // Conversions
		static bool		ToBool(const String & src);
		static int32_t	ToInt(const String & src);
		static float64_t	ToDecimal(const String & src);
		static float_t	ToFloat(const String & src);

		static bool		MakeBool(const String & src, bool& value);
		static bool		MakeInt(const String & src, int32_t& value);
		static bool		MakeDouble(const String & src, float64_t& value);
		static bool		MakeFloat(const String & src, float_t& value);
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_STRING_UTILITY_HPP_