#ifndef _ML_DOCUMENT_HPP_
#define _ML_DOCUMENT_HPP_

#include <ML/Editor/Export.hpp>
#include <ML/Core/File.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	struct ML_EDITOR_API Document
		: public ITrackable
		, public IComparable<Document>
	{
		/* * * * * * * * * * * * * * * * * * * * */

		enum : size_t 
		{ 
			NameSize = 64,
			DataSize = 1024,
		};

		/* * * * * * * * * * * * * * * * * * * * */

		using Name = typename char[NameSize];
		using Data = typename char[DataSize];

		/* * * * * * * * * * * * * * * * * * * * */

		using iterator				= typename std::_Array_iterator<char, DataSize>;
		using const_iterator		= typename std::_Array_const_iterator<char, DataSize>;
		using reverse_iterator		= typename std::reverse_iterator<iterator>;
		using const_reverse_iterator= typename std::reverse_iterator<const_iterator>;

		/* * * * * * * * * * * * * * * * * * * * */
		
		Name	name;
		Data	data;
		bool	open;
		bool	dirty;

		/* * * * * * * * * * * * * * * * * * * * */

		Document(CString name);
		Document(CString name, const char value);
		Document(CString name, const String & value);
		Document(CString name, CString value, const size_t size);
		Document(CString name, CString value, const size_t size, const size_t first);
		Document(const Document & copy);

		/* * * * * * * * * * * * * * * * * * * * */

		size_t sizeUsed() const;
		size_t sizeMax() const;

		/* * * * * * * * * * * * * * * * * * * * */

		inline iterator					begin()				{ return iterator(data, 0); }
		inline iterator					end()				{ return iterator(data, DataSize); }
		inline const_iterator			begin()		const	{ return const_iterator(data, 0); }
		inline const_iterator			end()		const	{ return const_iterator(data, DataSize); }
		inline const_iterator			cbegin()	const	{ return begin(); }
		inline const_iterator			cend()		const	{ return end(); }
		inline reverse_iterator			rbegin()			{ return reverse_iterator(end()); }
		inline reverse_iterator			rend()				{ return reverse_iterator(begin()); }
		inline const_reverse_iterator	rbegin()	const	{ return const_reverse_iterator(end()); }
		inline const_reverse_iterator	rend()		const	{ return const_reverse_iterator(begin()); }
		inline const_reverse_iterator	crbegin()	const	{ return rbegin(); }
		inline const_reverse_iterator	crend()		const	{ return rend(); }

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class _InIt,
			typename _Elem,
			class _OutIt = const_iterator
		> inline _OutIt find(_InIt first, const _InIt last, const _Elem & value) const
		{
			return std::find(first, last, value);
		}

		template <
			typename _Elem,
			class _OutIt = const_iterator
		> inline _OutIt find(const _Elem & value) const
		{
			return this->find<_OutIt>(begin(), end(), value);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		template <
			class _InIt,
			typename _Elem,
			class _OutIt = iterator
		> inline _OutIt find(_InIt first, const _InIt last, const _Elem & value)
		{
			return std::find(first, last, value);
		}

		template <
			typename _Elem,
			class _OutIt = iterator
		> inline _OutIt find(const _Elem & value)
		{
			return this->find<_OutIt>(begin(), end(), value);
		}

		/* * * * * * * * * * * * * * * * * * * * */

		inline bool equals(const Document & other) const override
		{
			return 
				(this->sizeUsed() == other.sizeUsed()) &&
				(String(this->data) == String(other.data));
		}

		inline bool lessThan(const Document & other) const override
		{
			return 
				(this->sizeUsed() < other.sizeUsed()) ||
				(String(this->data) < String(other.data));
		}

		/* * * * * * * * * * * * * * * * * * * * */
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_DOCUMENT_HPP_