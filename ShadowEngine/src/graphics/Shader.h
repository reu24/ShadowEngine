#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL\glew.h>
#include <string>
#include "core/Utils.h"

/*
	INTERNAL DON'T USE!
	Compiles a GLSL programm
*/
class Shader
{
public:
	Shader() = default;
	Shader(const std::string& vertexSource, const std::string& fragmentSource);
	Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath);
	~Shader();

	void initSource(const std::string& vertexSource, const std::string& fragmentSource);

	void bind();
	void addTexture(int location, std::string inShaderName);

	unsigned int id() const;

	// uniforms
	void addUniform(const std::string& name, bool value)
	{
		glUniform1i(getUniformLoc(name), (int)value);
	}
	void addUniform(const std::string& name, int value)
	{
		glUniform1i(getUniformLoc(name), value);
	}
	void addUniform(const std::string& name, float value)
	{
		glUniform1f(getUniformLoc(name), value);
	}
	void addUniform(const std::string& name, glm::vec2 value)
	{
		glUniform2fv(getUniformLoc(name), 1, &value[0]);
	}
	void addUniform(const std::string& name, glm::vec3 value)
	{
		glUniform3fv(getUniformLoc(name), 1, &value[0]);
	}
	void addUniform(const std::string& name, glm::vec4 value)
	{
		glUniform4fv(getUniformLoc(name), 1, &value[0]);
	}
	void addUniform(const std::string& name, glm::mat2 value)
	{
		glUniformMatrix2fv(getUniformLoc(name), 1, GL_FALSE, &value[0][0]);
	}
	void addUniform(const std::string& name, glm::mat3 value)
	{
		glUniformMatrix3fv(getUniformLoc(name), 1, GL_FALSE, &value[0][0]);
	}
	void addUniform(const std::string& name, glm::mat4 value)
	{
		glUniformMatrix4fv(getUniformLoc(name), 1, GL_FALSE, &value[0][0]);
	}

private:
	unsigned int getUniformLoc(const std::string& name)
	{
		return getMappedValue(mUniformLocs, name, glGetUniformLocation, mShader, name.c_str());
	}

	unsigned int mShader = 0;
	std::map<std::string, unsigned int> mUniformLocs;
};

extern std::string readFile(const std::string& filePath);