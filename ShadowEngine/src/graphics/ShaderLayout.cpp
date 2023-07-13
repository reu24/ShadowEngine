#include "ShaderLayout.h"

void ShaderLayout::setData()
{
	int i = 0;
	for (const auto& layout : mLayoutData)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i++, layout.count, layout.type, layout.normalize, mStride, (const void*)layout.offset);
	}
}
