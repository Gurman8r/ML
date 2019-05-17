#ifndef _ML_CHUNK_HPP_
#define _ML_CHUNK_HPP_

#include <ML/Core/Export.hpp>
#include <ML/Core/StandardLib.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_CORE_API Chunk final
	{
		size_t	size;	// Size of npos
		bool	free;	// Is free?
		Chunk *	prev;	// Prev chunk in list
		Chunk *	next;	// Next chunk in list
		uint8_t	npos[1];// End of metadata struct, pointer to npos

		Chunk()
			: size(NULL)
			, free(NULL)
			, prev(NULL)
			, next(NULL)
		{
		};
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_CHUNK_HPP_