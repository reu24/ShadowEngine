#include "Mesh.h"
#include <graphics\Shadow.h>


Mesh::Mesh(std::vector<Vertex> vertic, std::vector<GLuint> ind, std::vector<Texture> textur, std::vector<VertexBoneData> boneIdWeights)
{
	mVertices = vertic;
	mIndices = ind;
	mTextures = textur;
	mBonesIdWeightsForEachVertex = boneIdWeights;

	// Now that we have all the required data, set the vertex buffers and its attribute pointers.
	SetupMesh();
}

void VertexBoneData::addBoneData(uint boneId, float weight)
{
	for (uint i = 0; i < NUM_BONES_PER_VEREX; i++)
	{
		if (weights[i] == 0.0)
		{
			ids[i] = boneId;
			weights[i] = weight;
			return;
		}
	}
}


void Mesh::Draw(Shader& shadersProgram)
{
	int diffuseNr = 1;
	int specularNr = 1;

	for (int i = 0; i < mTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = mTextures[i].type;
		if (name == "texture_diffuse")
		{
			number = std::to_string(diffuseNr++);

		}
		else if (name == "texture_specular")
		{
			number = std::to_string(specularNr++);
		}

		glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
		shadersProgram.addTexture(i, "material." + name + number);
	}

	shadersProgram.addUniform("material.usesDiffuse", diffuseNr > 1);
	shadersProgram.addUniform("material.usesSpecularMap", specularNr > 1);

	//Draw
	glBindVertexArray(mVao);
	glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	for (int i = 0; i < mTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Mesh::SetupMesh()
{
	//vertices data
	glGenBuffers(1, &mVboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
	glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(mVertices[0]), &mVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//bones data
	glGenBuffers(1, &mVboBones);
	glBindBuffer(GL_ARRAY_BUFFER, mVboBones);
	glBufferData(GL_ARRAY_BUFFER, mBonesIdWeightsForEachVertex.size() * sizeof(mBonesIdWeightsForEachVertex[0]), &mBonesIdWeightsForEachVertex[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//numbers for sequence indices
	glGenBuffers(1, &mEboIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(GLuint), &mIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// create VAO and binding data from buffers to shaders
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, mVboVertices);
	//vertex position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1); // offsetof(Vertex, normal) = returns the byte offset of that variable from the start of the struct
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, textCoords));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//bones
	glBindBuffer(GL_ARRAY_BUFFER, mVboBones);
	glEnableVertexAttribArray(3);
	glVertexAttribIPointer(3, 4, GL_INT, sizeof(VertexBoneData), (GLvoid*)0); // for INT Ipointer
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (GLvoid*)offsetof(VertexBoneData, weights));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEboIndices);
	glBindVertexArray(0);
}