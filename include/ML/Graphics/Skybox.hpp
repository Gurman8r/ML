#ifndef _ML_SKYBOX_HPP_
#define _ML_SKYBOX_HPP_

#include <ML/Graphics/Texture.hpp>
#include <ML/Graphics/IDrawable.hpp>

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	// WIP
	class ML_GRAPHICS_API Skybox final
		: public IObject
		, public IDisposable
		, public IReadable
	{
	public:
		enum
		{
			Up, Down, Left, Right, Front, Back,
			MAX_FACE
		};

	public:
		Skybox();
		~Skybox();

	public:
		bool dispose() override;
		bool loadFromFile(const String & filename) override;

		void serialize(std::ostream & out) const override;

		static uint32_t loadCubemap(const List<String> & faces);

	public:
		inline const Texture * up()		const { return m_faces[Up];	   }
		inline const Texture * down()	const { return m_faces[Down];  }
		inline const Texture * left()	const { return m_faces[Left];  }
		inline const Texture * right()	const { return m_faces[Right]; }
		inline const Texture * front()	const { return m_faces[Front]; }
		inline const Texture * back()	const { return m_faces[Back];  }

	private:
		Texture * m_faces[MAX_FACE] { 0, 0, 0, 0, 0, 0 };
	};

	/* * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ML_SKYBOX_HPP_