#include <ML/Graphics/ModelRenderer.hpp>
#include <ML/Graphics/RenderTarget.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/scene.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	using tex_t = ModelRenderer::tex_t;
	using mesh_t = ModelRenderer::mesh_t;

	static inline mesh_t * processMesh(aiMesh * mesh, const aiScene *scene)
	{
		if (!mesh || !scene)
			return nullptr;

		Vertices		vertices;
		List<uint32_t>	indices;
		List<tex_t>		textures;

		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			vertices.push_back(Vertex {
				mesh->mVertices
					? vec3 { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z }
					: vec3 { uninit },
				mesh->mNormals
					? vec4 { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z, 1 }
					: vec4 { uninit },
				mesh->mTextureCoords[0]
					? vec2 { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y }
					: vec2 { uninit }
			});
		}

		for (uint32_t i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (uint32_t j = 0; j < face.mNumIndices; j++)
			{
				indices.push_back(face.mIndices[j]);
			}
		}

		return new mesh_t { vertices, indices, textures };
	}

	static inline void processNode(List<mesh_t *> & meshes, aiNode * node, const aiScene * scene)
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

	void ModelRenderer::mesh_t::setup()
	{
		if (vertices && indices)
		{
			m_vao.create(GL::Triangles).bind();
			m_vbo.create(GL::StaticDraw).bind().bufferData(vertices.contiguous());
			m_ibo.create(GL::StaticDraw, GL::UnsignedInt).bind().bufferData(indices);
			BufferLayout::Default.bind();
			m_ibo.unbind();
			m_vbo.unbind();
			m_vao.unbind();
		}
		else if (vertices)
		{
			m_vao.create(GL::Triangles).bind();
			m_vbo.create(GL::StaticDraw).bind().bufferData(vertices.contiguous());
			BufferLayout::Default.bind();
			m_vbo.unbind();
			m_vao.unbind();
		}
	}

	void ModelRenderer::mesh_t::draw(RenderTarget & target, RenderBatch batch) const
	{
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
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ml
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ModelRenderer::ModelRenderer()
		: m_states		{ RenderStates::Default }
		, m_material	{ nullptr }
		, m_meshes		{}
	{
	}

	ModelRenderer::~ModelRenderer()
	{
		for (auto *& elem : m_meshes)
			delete elem;
	}
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool ModelRenderer::loadFromFile(const String & filename)
	{
		if (const aiScene * scene = aiImportFile(filename.c_str(), aiProcess_Triangulate))
		{
			if (!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) && scene->mRootNode)
			{
				processNode(m_meshes, scene->mRootNode, scene);

				return !m_meshes.empty();
			}
		}
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

			for (const auto & elem : m_meshes)
			{
				target.draw(elem);
			}

			m_material->unbind();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}