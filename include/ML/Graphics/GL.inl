#include <ML/Graphics/GL.hpp>
#include <ML/Core/Preprocessor.hpp>

// Generate Bit Mask Operators
#define ML_GENERATE_MASK_OPERATORS(NAME, TYPE) \
inline NAME operator&(const NAME a, const NAME b) { return (NAME)((TYPE)a & (TYPE)b); } \
inline NAME operator|(const NAME a, const NAME b) { return (NAME)((TYPE)a | (TYPE)b); } \
inline NAME & operator &=(NAME & a, const NAME b) { return (a = (a & b)); } \
inline NAME & operator |=(NAME & a, const NAME b) { return (a = (a | b)); }


// Generate Iterator Operators
#define ML_GENERATE_ITER_OPERATORS(NAME, TYPE, ITER, MIN, MAX) \
template <class T> inline NAME operator+(const NAME a, const T b) \
{ \
	return (NAME)(ML_CLAMP(((TYPE)a + (TYPE)b), MIN, MAX)); \
} \
template <class T> inline NAME operator-(const NAME a, const T b) { return (a + (-b)); } \
template <class T> inline NAME & operator+=(NAME & a, const T b) { return (a = (a + b)); } \
template <class T> inline NAME & operator-=(NAME & a, const T b) { return (a = (a - b)); } \
inline NAME operator++(NAME & a)		{ return (a += 1); } \
inline NAME operator--(NAME & a)		{ return (a -= 1); } \
inline NAME operator++(NAME & a, ITER)	{ NAME c = a; a += 1; return c; } \
inline NAME operator--(NAME & a, ITER)	{ NAME c = a; a -= 1; return c; }


// Operators
/* * * * * * * * * * * * * * * * * * * * */
namespace ml
{
	// GL::Mask
	ML_GENERATE_MASK_OPERATORS(
		GL::Mask, uint32_t
	);

	// GL::Attachment
	ML_GENERATE_ITER_OPERATORS(
		GL::Attachment, uint32_t, int32_t, GL::ColorAttachment0, GL::ColorAttachment9
	);

	// GL::TextureID
	ML_GENERATE_ITER_OPERATORS(
		GL::TextureID, uint32_t, int32_t, GL::Texture0, GL::Texture31
	);
}