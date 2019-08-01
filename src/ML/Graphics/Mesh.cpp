#include <ML/Graphics/Mesh.hpp>
#include <ML/Core/FileSystem.hpp>
#include <ML/Core/Debug.hpp>

/* * * * * * * * * * * * * * * * * * * * */

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/* * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * */

	Mesh::Mesh()
		: Mesh(Vertices())
	{
		static Assimp::Importer ml_assimp_test;
	}

	Mesh::Mesh(const Vertices & vertices)
		: Mesh(vertices, Indices())
	{
	}

	Mesh::Mesh(const Vertices & vertices, const Indices & indices)
		: m_vertices	(vertices)
		, m_indices		(indices)
		, m_contiguous	(vertices.contiguous())
	{
	}

	Mesh::Mesh(const Mesh & copy)
		: m_vertices	(copy.m_vertices)
		, m_indices		(copy.m_indices)
		, m_contiguous	(copy.m_contiguous)
	{
	}

	Mesh::~Mesh() { dispose(); }

	/* * * * * * * * * * * * * * * * * * * * */
	
	bool Mesh::dispose()
	{
		m_vertices.clear();
		m_indices.clear();
		m_contiguous.clear();
		return true;
	}
	
	bool Mesh::loadFromFile(const String & filename)
	{
		/* * * * * * * * * * * * * * * * * * * * */

		auto parseLine = [](
			const String & line,
			const String & find,
			SStream & ss)
		{
			size_t i;
			if ((i = line.find(find)) != String::npos)
			{
				ss.str(line.substr((i + find.size()), (line.size() - find.size() - 1)));
				return true;
			}
			return false;
		};

		/* * * * * * * * * * * * * * * * * * * * */

		SStream in;
		if (ML_FS.getFileContents(filename, in))
		{
			/* * * * * * * * * * * * * * * * * * * * */

			List<vec3> vp;		// Positions
			List<vec2> vt;		// Texcoords
			List<vec3> vn;		// Normals
			List<Indices> vf;	// Faces

			String line;
			while (std::getline(in, line))
			{
				if (line.front() == '#')
					continue;

				SStream ss;
				if (parseLine(line, "v ", ss))
				{
					// Position
					vec3 temp;
					ss >> temp;
					vp.push_back(temp);
				}
				else if (parseLine(line, "vt ", ss))
				{
					// Texcoord
					vec2 temp;
					ss >> temp;
					vt.push_back(temp);
				}
				else if (parseLine(line, "vn ", ss))
				{
					// Normal
					vec3 temp;
					ss >> temp;
					vn.push_back(temp);
				}
				else if (parseLine(line, "f ", ss))
				{
					// Face
					vf.push_back(Indices());

					SStream lineStream(ss.str());
					while (lineStream.good())
					{
						lineStream >> line;

						SStream valueStream(line);
						while (std::getline(valueStream, line, '/'))
						{
							vf.back().push_back(std::stoi(line));
						}
					}
				}
			}

			/* * * * * * * * * * * * * * * * * * * * */

			Indices vi; // Position Indices
			Indices ti; // Texcoord Indices
			Indices ni; // Normal Indices

			for (size_t i = 0, imax = vf.size(); i < imax; i++)
			{
				vi.push_back(vf[i][0] - 1);
				ti.push_back(vf[i][1] - 1);
				ni.push_back(vf[i][2] - 1);

				vi.push_back(vf[i][3] - 1);
				ti.push_back(vf[i][4] - 1);
				ni.push_back(vf[i][5] - 1);

				vi.push_back(vf[i][6] - 1);
				ni.push_back(vf[i][8] - 1);
				ti.push_back(vf[i][7] - 1);
			}

			/* * * * * * * * * * * * * * * * * * * * */

			m_indices.clear(); // not using

			m_vertices.resize(vi.size());

			for (size_t i = 0, imax = m_vertices.size(); i < imax; i++)
			{
				m_vertices[i] = Vertex(
					vp[vi[i]],
					{ vn[vi[i]][0], vn[vi[i]][1], vn[vi[i]][2], 1.0f },
					vt[ti[i]]
				);
			}

			m_contiguous = m_vertices.contiguous();

			/* * * * * * * * * * * * * * * * * * * * */

			return true;
		}
		return false;
	}

	bool Mesh::loadFromMemory(const List<float_t> & vertices)
	{
		if (!vertices.empty())
		{
			m_indices.clear();
			m_vertices.clear();
			m_contiguous = vertices;
			return true;
		}
		return false;
	}

	bool Mesh::loadFromMemory(const List<float_t>& vertices, const Indices& indices)
	{
		if (!vertices.empty() && !indices.empty())
		{
			m_indices = indices;
			m_vertices.clear();
			m_contiguous = vertices;
			return true;
		}
		return false;
	}

	bool Mesh::loadFromMemory(const Vertices & vertices)
	{
		if (!vertices.empty())
		{
			m_indices.clear();
			m_vertices = vertices;
			m_contiguous = m_vertices.contiguous();
			return true;
		}
		return false;
	}

	bool Mesh::loadFromMemory(const Vertices & vertices, const Indices & indices)
	{
		if (!vertices.empty() && !indices.empty())
		{
			m_indices = indices;
			m_vertices = vertices;
			m_contiguous = m_vertices.contiguous();
			return true;
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * */
}