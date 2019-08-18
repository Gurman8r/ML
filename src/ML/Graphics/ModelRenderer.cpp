#include <ML/Graphics/ModelRenderer.hpp>
#include <ML/Graphics/RenderTarget.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/scene.h>

namespace ml
{
	struct TempMesh
	{
		List<vec3> vertices;
	};

	static inline aiMesh * processMesh(aiMesh * mesh, const aiScene * scene)
	{
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			mesh->mVertices[i];
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			mesh->mFaces[i];
		}

		return nullptr;
	}

	static inline void processNode(List<aiMesh *> & meshes, aiNode * node, const aiScene * scene)
	{
		// process all of the node's meshes and then do the same for each of its children
		if (node && scene)
		{
			for (uint32_t i = 0; i < node->mNumMeshes; i++)
			{
				meshes.push_back(processMesh(
					scene->mMeshes[node->mMeshes[i]],
					scene
				));
			}

			for (uint32_t i = 0; i < node->mNumChildren; i++)
			{
				processNode(
					meshes,
					node->mChildren[i],
					scene
				);
			}
		}
	}

	static inline ModelRenderer * loadFromAssimp(const String & filename)
	{
		if (const aiScene * scene = aiImportFile(
			filename.c_str(),
			aiProcess_Triangulate | aiProcess_FlipUVs
		))
		{
			if (!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode)
			{
				List<aiMesh *> meshes;
				processNode(meshes, scene->mRootNode, scene);

			}
		}
		return nullptr;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ModelRenderer::ModelRenderer()
		: m_vao			{ }
		, m_vbo			{ }
		, m_ibo			{ }
		, m_layout		{ BufferLayout::Default }
		, m_states		{ RenderStates::Default }
		, m_material	{ nullptr }
	{
	}

	ModelRenderer::~ModelRenderer()
	{
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ModelRenderer::loadFromFile(const String & filename)
	{
		if (const aiScene * scene = aiImportFile(
			filename.c_str(),
			aiProcess_Triangulate | aiProcess_FlipUVs
		))
		{
			if (!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode)
			{
				List<aiMesh *> meshes;
				//processNode(meshes, scene->mRootNode, scene);

				return true;
			}
		}
		return false;
	}

	bool ModelRenderer::loadFromMemory(const Vertices & vertices)
	{
		return loadFromMemory(vertices.contiguous());
	}

	bool ModelRenderer::loadFromMemory(const List<float_t> & vertices)
	{
		m_vao.create(GL::Triangles).bind();
		m_vbo.create(GL::StaticDraw).bind().bufferData(vertices);

		m_layout.bind();

		m_vbo.unbind();
		m_vao.unbind();

		return (m_vao && m_vbo);
	}

	bool ModelRenderer::loadFromMemory(const Vertices & vertices, const List<uint32_t> & indices)
	{
		return loadFromMemory(vertices.contiguous(), indices);
	}

	bool ModelRenderer::loadFromMemory(const List<float_t> & vertices, const List<uint32_t> & indices)
	{
		m_vao.create(GL::Triangles).bind();
		m_vbo.create(GL::StaticDraw).bind().bufferData(vertices);
		m_ibo.create(GL::StaticDraw, GL::UnsignedInt).bind().bufferData(indices);

		m_layout.bind();

		m_ibo.unbind();
		m_vbo.unbind();
		m_vao.unbind();

		return (m_vao && m_vbo && m_ibo);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ModelRenderer & ModelRenderer::setLayout(const BufferLayout & value)
	{
		if (&value != &m_layout)
		{
			m_layout = value;
		}
		return (*this);
	}

	ModelRenderer & ModelRenderer::setMaterial(const Material * value)
	{
		if (value != m_material)
		{
			m_material = value;
		}
		return (*this);
	}

	ModelRenderer & ModelRenderer::setStates(const RenderStates & value)
	{
		if (&value != &m_states)
		{
			m_states = value;
		}
		return (*this);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void ModelRenderer::draw(RenderTarget & target, RenderBatch batch) const
	{
		if (m_material)
		{
			m_states.apply();

			m_material->bind();

			if (m_vao && m_vbo)
			{
				if (m_ibo)
				{
					target.draw(m_vao, m_vbo, m_ibo);
				}
				else
				{
					target.draw(m_vao, m_vbo);
				}
			}

			m_material->unbind();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}