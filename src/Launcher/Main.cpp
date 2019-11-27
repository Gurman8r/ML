/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <ML/Core/EventSystem.hpp>
#include <ML/Editor/Editor.hpp>
#include <ML/Engine/Engine.hpp>
#include <ML/Core/Debug.hpp>
#include <ML/Core/PerformanceTracker.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	static_assert("Unit Tests"

		&& sizeof(String) == sizeof(std::string)

		&& sizeof(mat4b) == (sizeof(unsigned char) * 16)
		&& sizeof(mat4u) == (sizeof(unsigned int) * 16)
		&& sizeof(mat4i) == (sizeof(int) * 16)
		&& sizeof(mat4f) == (sizeof(float) * 16)
		&& sizeof(mat4d) == (sizeof(double) * 16)

		&& typeof<>{ typeof<int>{} }.name() == "int"

		&& typeof<bool>::name()				== "bool"
		&& typeof<char>::name()				== "char"
		&& typeof<wchar_t>::name()			== "wchar_t"
		&& typeof<char16_t>::name()			== "char16_t"
		&& typeof<char32_t>::name()			== "char32_t"
		&& typeof<C_String>::name()			== "const char*"
		&& typeof<int8_t>::name()			== "signed char"
		&& typeof<int16_t>::name()			== "short"
		&& typeof<int32_t>::name()			== "int"
#ifdef ML_CC_MSC
		&& typeof<int64_t>::name()			== "__int64"
#else
		&& typeof<int64_t>::name()			== "long long"
#endif
		&& typeof<uint8_t>::name()			== "unsigned char"
		&& typeof<uint16_t>::name()			== "unsigned short"
		&& typeof<uint32_t>::name()			== "unsigned int"
#ifdef ML_CC_MSC
		&& typeof<uint64_t>::name()			== "unsigned __int64"
#else
		&& typeof<uint64_t>::name()			== "unsigned long long"
#endif
		&& typeof<float32_t>::name()		== "float"
		&& typeof<float64_t>::name()		== "double"
		&& typeof<float80_t>::name()		== "long double"
#ifdef ML_HAS_CONSTEXPR_17
		&& typeof<std::string>::name()		== "std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
		&& typeof<std::wstring>::name()		== "std::basic_string<wchar_t,struct std::char_traits<wchar_t>,class std::allocator<wchar_t> >"
		&& typeof<std::u16string>::name()	== "std::basic_string<char16_t,struct std::char_traits<char16_t>,class std::allocator<char16_t> >"
		&& typeof<std::u32string>::name()	== "std::basic_string<char32_t,struct std::char_traits<char32_t>,class std::allocator<char32_t> >"
#else
		&& typeof<std::string>::name()		== "std::string"
		&& typeof<std::wstring>::name()		== "std::wstring"
		&& typeof<std::u16string>::name()	== "std::u16string"
		&& typeof<std::u32string>::name()	== "std::u32string"
#endif
	);
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

ml::int32_t main()
{
	using namespace ml;

	// Init Systems
	ML_EventSystem; ML_Engine; ML_Editor;

	// Enter
	ML_EventSystem.fireEvent<EnterEvent>(ML_ARGC, ML_ARGV);

	// Load
	ML_EventSystem.fireEvent<LoadEvent>();

	// Start
	ML_EventSystem.fireEvent<StartEvent>();

	// Main Loop
	do
	{
		ML_Engine.time().beginStep();

		{ ML_BENCH("Begin Step");	ML_EventSystem.fireEvent<BeginStepEvent>(); }
		{ ML_BENCH("-Update");		ML_EventSystem.fireEvent<UpdateEvent>();	}
		{ ML_BENCH("-Draw");		ML_EventSystem.fireEvent<DrawEvent>();		}
		{ ML_BENCH("-Begin Gui");	ML_EventSystem.fireEvent<BeginGuiEvent>();	}
		{ ML_BENCH("--Gui");		ML_EventSystem.fireEvent<GuiEvent>();		}
		{ ML_BENCH("-End Gui");		ML_EventSystem.fireEvent<EndGuiEvent>();	}
		{ ML_BENCH("End Step");		ML_EventSystem.fireEvent<EndStepEvent>();	}
		
		ML_PerformanceTracker.endFrame();
		
		ML_Engine.time().endStep();
	
	} while (ML_Engine.window().isOpen());

	// Unload
	ML_EventSystem.fireEvent<UnloadEvent>();

	// Exit
	ML_EventSystem.fireEvent<ExitEvent>();

	// Goodbye!
	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */