#pragma once
#include "ShaderLayout.h"
#include "Shader.h"
#include "Texture2D.h"

// INTERNAL DON'T USE!
class VertexArray
{
public:
	VertexArray()
	{
	}
	VertexArray(int vertexBufferSize, const void* vertexBufferData, bool vertexBufferDynamic,
		int indexBufferSize, unsigned int indexBufferData[], bool indexBufferDynamic,
		ShaderLayout& layout);
	~VertexArray();

	void fillVertexBuffer(int size, const void* data, bool dynamic = true);
	void fillIndexBuffer(int size, unsigned int data[], bool dynamic = true);
	void vertexBufferLayout(ShaderLayout& layout);

	void setVertexCount(int vertexCount);

	void setDefaultShader(Shader& shader);
	Shader& getDefaultShader() const;
	int getVertexCount() const;
	int id() const;
	bool hasIndicies();
	void bind();

private:
	unsigned int mVertexArray = 0;
	unsigned int mVertexBuffer = 0;
	unsigned int mIndexBuffer = 0;

	int mVertexCount = 0;
	Shader* mDefaultShader = nullptr;
};