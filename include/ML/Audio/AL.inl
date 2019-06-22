#include <ML/Audio/AL.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	namespace AL
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		static constexpr Err Err_values[] = {
			Err::NoError,
			Err::InvalidName,
			Err::InvalidEnum,
			Err::InvalidValue,
			Err::InvalidOperation,
			Err::OutOfMemory
		};

		static constexpr CString Err_names[] = {
			"No Error",
			"Invalid Name",
			"Invalid Enum",
			"Invalid Value",
			"Invalid Operation",
			"Out Of Memory"
		};

		static constexpr CString Err_descriptions[] = {
			"No description"
			"An unacceptable value has been specified for a name argument.",
			"An unacceptable value has been specified for an enumerated argument.",
			"A numeric argument is out of range.",
			"The specified operation is not allowed in the current state.",
			"There is not enough memory left to execute the command.",
		};

		static constexpr bool valueAt(const int32_t i, Err & value)
		{
			return alg::value_at(i, value, Err_values);
		}

		static constexpr int32_t indexOf(const Err value)
		{
			return alg::index_of(value, Err_values);
		}

		static constexpr CString nameOf(const Err value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Err_names[i] : "";
		}

		static constexpr CString descOf(const Err value)
		{
			const int32_t i = indexOf(value);
			return (i >= 0) ? Err_descriptions[i] : "";
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}

	/* * * * * * * * * * * * * * * * * * * * */
}