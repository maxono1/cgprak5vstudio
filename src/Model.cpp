//
//  Model.cpp
//  ogl4
//
//  Created by Philipp Lensing on 21.09.16.
//  Copyright © 2016 Philipp Lensing. All rights reserved.
//

#include "Model.h"
#include "phongshader.h"
#include <list>

Model::Model() : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{

}
Model::Model(const char* ModelFile, bool FitSize) : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0)
{
	bool ret = load(ModelFile, FitSize);
	if (!ret)
		throw std::exception();
}
Model::~Model()
{
	// TODO: Add your code (Exercise 3)
	delete[] pMeshes;
	delete[] pMaterials;
	deleteNodes(&RootNode);
}

void Model::deleteNodes(Node* pNode)
{
	if (!pNode)
		return;
	for (unsigned int i = 0; i < pNode->ChildCount; ++i)
		deleteNodes(&(pNode->Children[i]));
	if (pNode->ChildCount > 0)
		delete[] pNode->Children;
	if (pNode->MeshCount > 0)
		delete[] pNode->Meshes;
}

bool Model::load(const char* ModelFile, bool FitSize)
{
	const aiScene* pScene = aiImportFile(ModelFile, aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords);

	if (pScene == NULL || pScene->mNumMeshes <= 0)
		return false;

	Filepath = ModelFile;
	Path = Filepath;
	size_t pos = Filepath.rfind('/');
	if (pos == std::string::npos)
		pos = Filepath.rfind('\\');
	if (pos != std::string::npos)
		Path.resize(pos + 1);

	loadMeshes(pScene, FitSize);
	loadMaterials(pScene);
	loadNodes(pScene);

	return true;
}

void Model::loadMeshes(const aiScene* pScene, bool FitSize)
{
	//in membervariable pmeshes rein
	//this->pMeshes = pScene->mMeshes;
	//das geht so nicht!!! ich muss erst die calc bounding box methode aufrufen!!!! und wissen wie groß das Model ist!

	MeshCount = pScene->mNumMeshes;


	//bounding box koordinaten, wir legen sie hier an

	AABB boundingBox = AABB();
	//calcBoundingBox(pScene, boundingBox);

	//float scaling = 0.25f;
	float scaling = 1.00f;
	float maxAusdehnungReal = calcBoundingBox(pScene, boundingBox);
	float gewolltAusdehnung = 4.0f;
	if (FitSize) {
		scaling = maxAusdehnungReal / gewolltAusdehnung;
	}

	pMeshes = new Mesh[MeshCount];
	std::cout << "Num Meshes: " << MeshCount << std::endl;



	for (size_t i = 0; i < MeshCount; i++) //iterate all Meshes
	{
		pMeshes[i].VB.begin();
		pMeshes[i].MaterialIdx = pScene->mMeshes[i]->mMaterialIndex;
		for (size_t j = 0; j < pScene->mMeshes[i]->mNumVertices; j++) //in the Mesh through all Vertices
		{
			//Vertex needs:
			//normal, position, textureCoord
			//Vector normal;
			//Vector position;
			//float oder int, für u und v

			//pScene->mMeshes[i]->mVertices[j];
			if (pScene->mMeshes[i]->HasNormals()) {
				aiVector3D normal = pScene->mMeshes[i]->mNormals[j];
				pMeshes[i].VB.addNormal(Vector(normal.x, normal.y, normal.z));
			}//else you dont need to add it to the vertexbuffer
			//wir gucken hier nur die erste textur, deswegen 0
			if (pScene->mMeshes[i]->HasTextureCoords(0)) {
				aiVector3D texCoords0 = pScene->mMeshes[i]->mTextureCoords[0][j];
				pMeshes[i].VB.addTexcoord0(texCoords0.x, -texCoords0.y);
			}
			aiVector3D vertices = pScene->mMeshes[i]->mVertices[j];
			pMeshes[i].VB.addVertex(Vector(vertices.x / scaling, vertices.y / scaling, vertices.z / scaling));
		}
		pMeshes[i].VB.end();
	}

	for (size_t i = 0; i < MeshCount; i++) {
		pMeshes[i].IB.begin();
		for (size_t k = 0; k < pScene->mMeshes[i]->mNumFaces; k++) {     //mFaces und mNumFaces für den Indexbuffer
			for (size_t l = 0; l < pScene->mMeshes[i]->mFaces[k].mNumIndices; l++) {
				pMeshes[i].IB.addIndex(pScene->mMeshes[i]->mFaces[k].mIndices[l]);
			}
		}
		pMeshes[i].IB.end();
	}

}
void Model::loadMaterials(const aiScene* pScene)
{
	if (pScene->HasMaterials()) {
		MaterialCount = pScene->mNumMaterials;
		pMaterials = new Material[MaterialCount];
		for (size_t i = 0; i < MaterialCount; i++) {

			//Quellen: https://cpp.hotexamples.com/examples/-/-/aiGetMaterialColor/cpp-aigetmaterialcolor-function-examples.html
			//http://assimp.sourceforge.net/lib_html/materials.html
			aiColor4D aiDiffuse;
			aiColor4D aiSpecular;
			aiColor4D aiAmbient;
			float shininess;
			float shininessStrength;
			unsigned int max = 1;
			aiString textureName;
			pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuse);
			pMaterials[i].DiffColor = Color(aiDiffuse.r, aiDiffuse.g, aiDiffuse.b);
			pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecular);
			pMaterials[i].SpecColor = Color(aiSpecular.r, aiSpecular.g, aiSpecular.b);
			pScene->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, aiAmbient);
			pMaterials[i].AmbColor = Color(aiAmbient.r, aiAmbient.g, aiAmbient.b);
			//shininess ist der phong specular exponnent :  http://assimp.sourceforge.net/lib_html/materials.html
			//aiGetMaterialFloatArray(pScene->mMaterials[i], AI_MATKEY_SHININESS, &shininess, &max);
			pScene->mMaterials[i]->Get(AI_MATKEY_SHININESS, shininess);
			pScene->mMaterials[i]->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength);
			//aiGetMaterialFloatArray(pScene->mMaterials[i], AI_MATKEY_SHININESS_STRENGTH, &shininessStrength, &max);
			//shininess ist die größe von dem kreis
			pMaterials[i].SpecExp = shininess;//shininess / 100.0f;
			std::cout << "shininess" << shininess << "\n";
			std::cout << "shininessStrength" << shininessStrength << "\n";
			std::cout << "product" << shininess * shininessStrength << "\n";


			//einmal testen wie die werte 
			std::cout << "Diffuse: " << pMaterials[i].DiffColor.R << " : " << pMaterials[i].DiffColor.G << " : "
				<< pMaterials[i].DiffColor.B << std::endl;
			std::cout << "Specular: " << pMaterials[i].SpecColor.R << " : " << pMaterials[i].SpecColor.G << " : "
				<< pMaterials[i].SpecColor.B << std::endl;
			std::cout << "Ambient: " << pMaterials[i].AmbColor.R << " : " << pMaterials[i].AmbColor.G << " : "
				<< pMaterials[i].AmbColor.B << std::endl;

			int numTexturesDiff = pScene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
			//for (size_t i = 0; i < numTexturesDiff; i++) {
			//wir holen nur die diffuse texturen, da unser material in der model.h nur das unterstützt
			if(numTexturesDiff > 0){
				aiString texturName;
				pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), texturName);
				//dieses .data ist der C string in dem aiString
				//http://assimp.sourceforge.net/lib_html/structai_string.html
				std::string textureFileName = Path + texturName.data;
				pMaterials[i].DiffTex = Texture::LoadShared(textureFileName.c_str());
			}/*
			int numTexturesSpec = pScene->mMaterials[i]->GetTextureCount(aiTextureType_SPECULAR);
			for (size_t i = 0; i < numTexturesSpec; i++) {
				aiString texturName;
				pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE(aiTextureType_SPECULAR, i), texturName);
				//dieses .data ist der C string in dem aiString
				//http://assimp.sourceforge.net/lib_html/structai_string.html
				std::string textureFileName = Path + texturName.data;
				pMaterials[i].DiffTex = Texture::LoadShared(textureFileName.c_str());
			}*/
		}
	}

}


float Model::calcBoundingBox(const aiScene* pScene, AABB& Box) {
	//für das min größtmögliche zahlen wählen, damit jeder wert es überschreibt
	float minX = std::numeric_limits<float>::max();
	float minY = std::numeric_limits<float>::max();
	float minZ = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::min();
	float maxY = std::numeric_limits<float>::min();
	float maxZ = std::numeric_limits<float>::min();

	float overallMin = std::numeric_limits<float>::max();
	float overallMax = std::numeric_limits<float>::min();

	for (int i = 0; i < MeshCount; i++) {
		for (int k = 0; k < pScene->mMeshes[i]->mNumVertices; k++) {
			//wenn der x wert größer, dann wird maxX überschrieben
			if (pScene->mMeshes[i]->mVertices[k].x > maxX) {
				maxX = pScene->mMeshes[i]->mVertices[k].x;
			}
			if (pScene->mMeshes[i]->mVertices[k].x > overallMax) {
				overallMax = pScene->mMeshes[i]->mVertices[k].x;
			}

			if (pScene->mMeshes[i]->mVertices[k].y > maxY) {
				maxY = pScene->mMeshes[i]->mVertices[k].y;
			}
			if (pScene->mMeshes[i]->mVertices[k].y > overallMax) {
				overallMax = pScene->mMeshes[i]->mVertices[k].y;
			}

			if (pScene->mMeshes[i]->mVertices[k].z > maxZ) {
				maxZ = pScene->mMeshes[i]->mVertices[k].z;
			}
			if (pScene->mMeshes[i]->mVertices[k].z > overallMax) {
				overallMax = pScene->mMeshes[i]->mVertices[k].z;
			}

			if (pScene->mMeshes[i]->mVertices[k].x < minX) {
				minX = pScene->mMeshes[i]->mVertices[k].x;
			}
			if (pScene->mMeshes[i]->mVertices[k].x < overallMin) {
				overallMin = pScene->mMeshes[i]->mVertices[k].x;
			}

			if (pScene->mMeshes[i]->mVertices[k].y < minY) {
				minY = pScene->mMeshes[i]->mVertices[k].y;
			}
			if (pScene->mMeshes[i]->mVertices[k].y < overallMin) {
				overallMin = pScene->mMeshes[i]->mVertices[k].y;
			}

			if (pScene->mMeshes[i]->mVertices[k].z < minZ) {
				minZ = pScene->mMeshes[i]->mVertices[k].z;
			}
			if (pScene->mMeshes[i]->mVertices[k].z < overallMin) {
				overallMin = pScene->mMeshes[i]->mVertices[k].z;
			}
		}

	}
	//wenn iwie die vertexe nich existen, fehler ausgeben
	if (minX == std::numeric_limits<float>::max() || minY == std::numeric_limits<float>::max() || minZ == std::numeric_limits<float>::max()
		|| maxX == std::numeric_limits<float>::min() || maxY == std::numeric_limits<float>::min() == maxZ == std::numeric_limits<float>::min()) {
		std::cerr << "bounding box calculation fehler\n" << minX << "-minX \t" << minY << "-minY \t" << minZ << "-minZ \t" << maxX << "-maxX \t" << maxY << "-maxY \t" << maxZ << "-maxZ \t";

	}

	Vector min = Vector(minX, minY, minZ);
	Vector max = Vector(maxX, maxY, maxZ);

	Box.Min = min;
	Box.Max = max;


	//furthest away from the center of the coordinate plane!
	if (fabs(overallMin) > overallMax) {
		return overallMin * -1;
	}
	else {
		return overallMax;
	}
}

void Model::loadNodes(const aiScene* pScene)
{
	deleteNodes(&RootNode);
	copyNodesRecursive(pScene->mRootNode, &RootNode);
}

void Model::copyNodesRecursive(const aiNode* paiNode, Node* pNode)
{
	pNode->Name = paiNode->mName.C_Str();
	pNode->Trans = convert(paiNode->mTransformation);

	if (paiNode->mNumMeshes > 0)
	{
		pNode->MeshCount = paiNode->mNumMeshes;
		pNode->Meshes = new int[pNode->MeshCount];
		for (unsigned int i = 0; i < pNode->MeshCount; ++i)
			pNode->Meshes[i] = (int)paiNode->mMeshes[i];
	}

	if (paiNode->mNumChildren <= 0)
		return;

	pNode->ChildCount = paiNode->mNumChildren;
	pNode->Children = new Node[pNode->ChildCount];
	for (unsigned int i = 0; i < paiNode->mNumChildren; ++i)
	{
		copyNodesRecursive(paiNode->mChildren[i], &(pNode->Children[i]));
		pNode->Children[i].Parent = pNode;
	}
}

void Model::applyMaterial(unsigned int index)
{
	if (index >= MaterialCount)
		return;

	PhongShader* pPhong = dynamic_cast<PhongShader*>(shader());
	if (!pPhong) {
		std::cout << "Model::applyMaterial(): WARNING Invalid shader-type. Please apply PhongShader for rendering models.\n";
		return;
	}

	Material* pMat = &pMaterials[index];
	pPhong->ambientColor(pMat->AmbColor);
	pPhong->diffuseColor(pMat->DiffColor);
	pPhong->specularExp(pMat->SpecExp);
	pPhong->specularColor(pMat->SpecColor);
	pPhong->diffuseTexture(pMat->DiffTex);
	
}

void Model::draw(const BaseCamera& Cam)
{
	if (!pShader) {
		std::cout << "BaseModel::draw() no shader found" << std::endl;
		return;
	}
	pShader->modelTransform(transform());

	std::list<Node*> DrawNodes;
	DrawNodes.push_back(&RootNode);

	while (!DrawNodes.empty())
	{
		Node* pNode = DrawNodes.front();
		Matrix GlobalTransform;

		if (pNode->Parent != NULL)
			pNode->GlobalTrans = pNode->Parent->GlobalTrans * pNode->Trans;
		else
			pNode->GlobalTrans = transform() * pNode->Trans;

		pShader->modelTransform(pNode->GlobalTrans);

		for (unsigned int i = 0; i < pNode->MeshCount; ++i)
		{
			Mesh& mesh = pMeshes[pNode->Meshes[i]];
			mesh.VB.activate();
			mesh.IB.activate();
			applyMaterial(mesh.MaterialIdx);
			pShader->activate(Cam);
			glDrawElements(GL_TRIANGLES, mesh.IB.indexCount(), mesh.IB.indexFormat(), 0);
			mesh.IB.deactivate();
			mesh.VB.deactivate();
		}
		for (unsigned int i = 0; i < pNode->ChildCount; ++i)
			DrawNodes.push_back(&(pNode->Children[i]));

		DrawNodes.pop_front();
	}
}

Matrix Model::convert(const aiMatrix4x4& m)
{
	return Matrix(m.a1, m.a2, m.a3, m.a4,
		m.b1, m.b2, m.b3, m.b4,
		m.c1, m.c2, m.c3, m.c4,
		m.d1, m.d2, m.d3, m.d4);
}



