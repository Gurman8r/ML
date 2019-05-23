#ifndef _ML_I_METADATABLE_HPP_
#define _ML_I_METADATABLE_HPP_

namespace ml
{
	class Metadata;

	class IMetadatable
	{
	public:
		virtual Metadata & getMetadata(Metadata &) const = 0;
	};
}

#endif // !_ML_I_METADATABLE_HPP_