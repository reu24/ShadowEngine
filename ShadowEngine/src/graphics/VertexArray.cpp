#include "VertexArray.h"


VertexArray::VertexArray(int vertexBufferSize, const void* vertexBufferData, bool vertexBufferDynamic,
	int indexBufferSize, unsigned int indexBufferData[], bool indexBufferDynamic,
	ShaderLayout& layout)
{
	fillVertexBuffer(vertexBufferSize, vertexBufferData, vertexBufferDynamic);
	fillIndexBuffer(indexBufferSize, indexBufferData, indexBufferDynamic);
	vertexBufferLayout(layout);
}


VertexArray::~VertexArray()
{
	if (mVertexArray)
		glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::fillVertexBuffer(int size, const void* data, bool dynamic)
{
	if (!mVertexArray)
		glGenVertexArrays(1, &mVertexArray);

	glBindVertexArray(mVertexArray);
	if (mVertexBuffer == 0)
	{
		glGenBuffers(1, &mVertexBuffer);
	}
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	GLenum usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

void VertexArray::fillIndexBuffer(int size, unsigned int data[], bool dynamic)
{
	if (!mVertexArray)
		glGenVertexArrays(1, &mVertexArray);

	glBindVertexArray(mVertexArray);
	if (mIndexBuffer == 0)
	{
		glGenBuffers(1, &mIndexBuffer);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	GLenum usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
	glBindVertexArray(0);

	mVertexCount = size / sizeof(unsigned int);
}

void VertexArray::vertexBufferLayout(ShaderLayout& layout)
{
	if (!mVertexArray)
		glGenVertexArrays(1, &mVertexArray);

	glBindVertexArray(mVertexArray);
	layout.setData();
	glBindVertexArray(0);
}

void VertexArray::setVertexCount(int vertexCount)
{
	mVertexCount = vertexCount;
}

void VertexArray::setDefaultShader(Shader& shader)
{
	mDefaultShader = &shader;
}

Shader& VertexArray::getDefaultShader() const
{
	return *mDefaultShader;
}

int VertexArray::getVertexCount() const
{
	return mVertexCount;
}
int VertexArray::id() const
{
	return mVertexArray;
}
bool VertexArray::hasIndicies()
{
	return mIndexBuffer != 0;
}
void VertexArray::bind()
{
	glBindVertexArray(mVertexArray);
}