#pragma once
#include <vector>
#include <GL\glew.h>

// INTERNAL DON'T USE!
class ShaderLayout
{
	struct LayoutData
	{
		int type;
		int count;
		bool normalize;
		int offset;
	};

public:
	void setData();

	template <typename T>
	void push(int count, bool normalize = false)
	{
		Error("Layout Error: Can't use that type");
	}

	template <>
	void push<float>(int count, bool normalize)
	{
		mLayoutData.push_back(LayoutData {GL_FLOAT, count, normalize, mStride}); // last paramter is offset
		mStride += count * sizeof(float);
	}

	template <>
	void push<int>(int count, bool normalize)
	{
		mLayoutData.push_back(LayoutData {GL_INT, count, normalize, mStride}); // last paramter is offset
		mStride += count * sizeof(int);
	}

	template <>
	void push<bool>(int count, bool normalize)
	{
		mLayoutData.push_back(LayoutData {GL_BOOL, count, normalize, mStride}); // last paramter is offset
		mStride += count * sizeof(bool);
	}

	template <>
	void push<char>(int count, bool normalize)
	{
		mLayoutData.push_back(LayoutData {GL_BYTE, count, normalize, mStride}); // last paramter is offset
		mStride += count * sizeof(char);
	}

	template <>
	void push<double>(int count, bool normalize)
	{
		mLayoutData.push_back(LayoutData {GL_DOUBLE, count, normalize, mStride}); // last paramter is offset
		mStride += count * sizeof(double);
	}

private:
	std::vector<LayoutData> mLayoutData;
	int mStride = 0;
};