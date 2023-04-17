#include "model.h"

Model::Model()
{
}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	ModelScene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!ModelScene) {
		printf("Loading of the model failed %s, %s", fileName, importer.GetErrorString());
		return;
	}
	else {
		LoadNode(ModelScene->mRootNode, ModelScene);
	}
	LoadMaterials(ModelScene);
}

void Model::RenderModel()
{
	for (int i = 0; i < MeshList.size(); i++) {
		unsigned int materialIndex = MeshToTexture[i];
		if (materialIndex < TextureList.size() && TextureList[materialIndex]) {
			TextureList[materialIndex]->useTexture();
		}
		MeshList[i]->RenderMesh();
	}

}

void Model::ClearModel()
{
	for (int i = 0; i < MeshList.size(); i++) {
		if (MeshList[i])
		{
			delete MeshList[i];
			MeshList[i] = nullptr;

		}
	}

	for (int i = 0; i < TextureList.size(); i++) {
		if (TextureList[i])
		{
			delete TextureList[i];
			TextureList[i] = nullptr;

		}
	}


}

void Model::LoadNode(aiNode* nodes, const aiScene* scene)
{
	for (int i = 0; i < nodes->mNumMeshes; i++) 
	{
		LoadMesh(ModelScene->mMeshes[nodes->mMeshes[i]], ModelScene);
	}
	for (int i = 0; i < nodes->mNumChildren; i++)
	{
		LoadNode(nodes->mChildren[i], ModelScene);

	}
}

void Model::LoadMesh(aiMesh* mesh,const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;
	for (int i = 0; i <mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x,mesh->mVertices[i].y,mesh->mVertices[i].z }); // adding vertices x,y,z
		if (mesh->mTextureCoords[0]) { // check if there is any texture coordinates 
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x,mesh->mTextureCoords[0][i].y}); // adding texturecoordinates  x,y // U and V
		}
		else {
			vertices.insert(vertices.end(), {0.0f,0.0f}); 	
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x,-mesh->mNormals[i].y,-mesh->mNormals[i].z }); //
	}

	for (int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
	Mesh* modelMesh = new Mesh();
	modelMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	MeshList.push_back(modelMesh);
	MeshToTexture.push_back(mesh->mMaterialIndex);
	


}

void Model::LoadMaterials(const aiScene* scene)
{
	TextureList.resize(scene->mNumMaterials);
	for (int i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* material = scene->mMaterials[i];
		TextureList[i] = nullptr;
		if (material->GetTextureCount(aiTextureType_DIFFUSE)) {
			aiString path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
				int index = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(index + 1);
				std::string texPath = std::string("Texture/") + filename;
				TextureList[i] = new Texture(texPath.c_str());
				if (!TextureList[i]->LoadTextureM()) {
					printf("failed to load texture : %s", texPath);
					delete TextureList[i];
					TextureList[i] = nullptr;
				}
			}
		}
		if (!TextureList[i]) {
			TextureList[i] = new Texture("Texture/1.jpg");
			TextureList[i]->loadTexture();
		}
	}


}

Model::~Model()
{
}
