#ifndef _ML_META_HPP_
#define _ML_META_HPP_

// https://www.youtube.com/watch?v=DUiUBt-fqEY&t=2050s

#include <ML/Core/String.hpp>

namespace ml
{
	namespace meta
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Type;
		struct Field;
		struct Method;
		struct Class;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Type
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			String name;
			size_t size;

			Type()
				: name{ "" }
				, size{ 0 }
			{
			}

			Type(const String & name, size_t size)
				: name{ name }
				, size{ size }
			{
			}

			Type(String && name, size_t size)
				: name{ std::move(name) }
				, size{ size }
			{
			}

			Type(const Type & copy)
				: name{ copy.name }
				, size{ copy.size }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Field
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			const Type * type;
			String name;
			size_t offset;

			Field()
				: name{ "" }
				, type{ nullptr }
				, offset{ 0 }
			{
			}

			Field(const Type * type, const String & name, size_t offset)
				: name{ name }
				, type{ type }
				, offset{ offset }
			{
			}

			Field(const Type * type, String && name, size_t offset)
				: name{ std::move(name) }
				, type{ type }
				, offset{ offset }
			{
			}

			Field(const Field & copy)
				: name{ copy.name }
				, type{ copy.type }
				, offset{ copy.offset }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Method
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			String name;
			Field retval;
			Field params;

			Method()
				: name{ "" }
				, retval{}
				, params{}
			{
			}

			Method(String && name, Field && retval, Field && params)
				: name{ std::move(name) }
				, retval{ std::move(retval) }
				, params{ std::move(params) }
			{
			}

			Method(const String & name, const Field & retval, const Field & params)
				: name{ name }
				, retval{ retval }
				, params{ params }
			{
			}

			Method(const Method & copy)
				: name{ copy.name }
				, retval{ copy.retval }
				, params{ copy.params }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Class : public Type
		{
			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

			String name;
			List<Field> fields;
			List<Method> methods;

			Class()
				: name{ "" }
				, fields{}
				, methods{}
			{
			}

			Class(String && name, List<Field> && fields, List<Method> && methods)
				: name{ std::move(name) }
				, fields{ std::move(fields) }
				, methods{ std::move(methods) }
			{
			}

			Class(const String & name, const List<Field> & fields, const List<Method> & methods)
				: name{ name }
				, fields{ fields }
				, methods{ methods }
			{
			}

			Class(const Class & copy)
				: name{ copy.name }
				, fields{ copy.fields }
				, methods{ copy.methods }
			{
			}

			/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
		};

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		template <class T> static inline const Type * get_type()
		{
			static Type temp{ typeof<T>::name, sizeof(T) };
			return &temp;
		}

		template <> static inline const Type * get_type<void>()
		{
			static Type temp{ typeof<void>::name, 0 };
			return &temp;
		}

		template <class T> static inline const Class * get_class()
		{
			return nullptr;
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		struct Test
		{
			Test() = default;
			size_t id{ 0 };
			String name{};
			void print_name() { cout << name; }
		};

		template <> static inline const meta::Class * meta::get_class<Test>()
		{
			static meta::Class temp{ typeof<Test>::name,
				{
					{ meta::get_type<size_t>(), "id", 0 },
					{ meta::get_type<String>(), "name", 0 }
				},
				{
					{ "print_name",
					{ meta::get_type<void>(), "return", 0 },
					{ meta::get_type<void>(), "params", 0 }
					},
				}
			};
			return &temp;
		}

		static inline void TEST_META()
		{
			if (const auto * c{ meta::get_class<Test>() })
			{
				cout << "Class { " << c->name << endl;

				for (const auto & field : c->fields)
				{
					cout << "\t" << "Field { " << field.name << ", " << field.type->name << " }" << endl;
				}

				for (const auto & method : c->methods)
				{
					cout << "\t" << "Method { " << method.name << endl
						<< "\t\t" << method.retval.name << ", " << method.retval.type->name << endl
						<< "\t\t" << method.params.name << ", " << method.params.type->name << endl
						<< "\t}" << endl;
				}

				cout << "}" << endl;
			}
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}

#endif // !_ML_META_HPP_