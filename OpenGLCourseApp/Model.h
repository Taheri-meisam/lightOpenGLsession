#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "Texture.h"
#include <vector>
#include <string>

class Model
{
public:
	Model();
	void LoadModel(const std::string& fileName);
	void RenderModel();
	void ClearModel();
	void LoadNode(aiNode* nodes,const aiScene* scene);
	void LoadMesh(aiMesh* mesh,const aiScene* scene);
	void LoadMaterials(const aiScene* scene);
	~Model();
private:

	std::vector<Mesh*> MeshList;
	std::vector<Texture*> TextureList;
	std::vector<int> MeshToTexture;
	const aiScene* ModelScene;
};


