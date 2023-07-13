#pragma once

#include <iostream>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "assimp/Importer.hpp"

#include <vector>
#include <string>
#include <graphics/Shader.h>

typedef unsigned int uint;
#define NUM_BONES_PER_VEREX 4

// INTERNAL DON'T USE!
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 textCoords;
};

// INTERNAL DON'T USE!
struct Texture
{
	GLuint id;
	std::string type;
	aiString path;
};

// INTERNAL DON'T USE!
struct BoneMatrix
{
	aiMatrix4x4 offsetMatrix;
	aiMatrix4x4 finalWorldTransform;
};

// INTERNAL DON'T USE!
struct VertexBoneData
{
	uint ids[NUM_BONES_PER_VEREX];   // we have 4 bone ids for EACH vertex & 4 weights for EACH vertex
	float weights[NUM_BONES_PER_VEREX];

	VertexBoneData()
	{
		memset(ids, 0, sizeof(ids));    // init all values in array = 0
		memset(weights, 0, sizeof(weights));
	}

	void addBoneData(uint bone_id, float weight);
};

// INTERNAL DON'T USE!
class Mesh
{
public:
	Mesh(std::vector<Vertex> vertic, std::vector<GLuint> ind, std::vector<Texture> textur, std::vector<VertexBoneData> boneIdWeights);
	Mesh() = default;

	// Render mesh
	void Draw(Shader& shader);

private:
	//Mesh data
	std::vector<Vertex> mVertices;
	std::vector<GLuint> mIndices;
	std::vector<Texture> mTextures;
	std::vector<VertexBoneData> mBonesIdWeightsForEachVertex;

	//buffers
	GLuint mVao = 0;
	GLuint mVboVertices = 0;
	GLuint mVboBones = 0;
	GLuint mEboIndices = 0;

	//inititalize buffers
	void SetupMesh();
};
