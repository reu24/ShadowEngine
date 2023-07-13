#include "Shader.h"

#include <GL/glew.h>
#include <fstream>
#include <iostream>
#include <core/Log.h>

std::string readFile(const std::string& filePath)
{
	// create filestream
	std::ifstream file(filePath, std::ios::binary);
	// error checking
	if (file.fail())
	{
		std::cout << std::string("Failed to load content from file " + filePath + ':');
		perror(filePath.c_str());
		std::cout << '\n';
		return "";
	}

	// get file size
	file.seekg(0, std::ios::end);
	unsigned long fileSize = (unsigned long)file.tellg();
	file.seekg(0, std::ios::beg);

	std::string data(fileSize, ' ');
	file.read(&data[0], fileSize);
	file.close();
	return data;
}

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource)
{
	initSource(vertexSource, fragmentSource);
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
	// vertex shader
	const std::string vertexShaderSource = readFile(vertexPath);
	const char* vertexShaderSourceCStr = vertexShaderSource.c_str();
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, NULL);
	glCompileShader(vertexShader);
	// error checking
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "Failed to compile vertex shader! Error log: " << infoLog << "\nFile: " << vertexPath;
		system("PAUSE");
		exit(-1);
	}
	// fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const std::string fragmentShaderSource = readFile(fragmentPath);
	const char* fragmentShaderSourceCStr = fragmentShaderSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, nullptr);
	glCompileShader(fragmentShader);
	// error checking
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "Failed to compile fragment shader! Error log: " << infoLog << "\nFile: " << fragmentPath;
		system("PAUSE");
		exit(-1);
	}
	// geometry shader
	unsigned int geometryShader;
	geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

	const std::string geometryShaderSource = readFile(fragmentPath);
	const char* geometryShaderSourceCStr = fragmentShaderSource.c_str();
	glShaderSource(geometryShader, 1, &fragmentShaderSourceCStr, nullptr);
	glCompileShader(geometryShader);
	// error checking
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(geometryShader, 512, nullptr, infoLog);
		std::cout << "Failed to compile fragment shader! Error log: " << infoLog << "\nFile: " << fragmentPath;
		system("PAUSE");
		exit(-1);
	}
	// shader program
	mShader = glCreateProgram();
	glAttachShader(mShader, vertexShader);
	glAttachShader(mShader, fragmentShader);
	glAttachShader(mShader, geometryShader);
	glLinkProgram(mShader);
	// error checking
	glGetShaderiv(mShader, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(mShader, 512, nullptr, infoLog);
		std::cout << "Failed to link shaders! Error log: " << infoLog;
		system("PAUSE");
		exit(-1);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);
}

Shader::~Shader()
{
	glDeleteProgram(mShader);
}

void Shader::initSource(const std::string& vertexSource, const std::string& fragmentSource)
{
	// vertex shader
	const char* vertexShaderSourceCStr = vertexSource.c_str();
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSourceCStr, NULL);
	glCompileShader(vertexShader);
	// error checking
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::string error = "Failed to compile vertex shader! Error log: ";
		error += infoLog;
		LOG_ERROR(error, false);
		system("PAUSE");
		exit(-1);
	}
	// fragment shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	const char* fragmentShaderSourceCStr = fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, nullptr);
	glCompileShader(fragmentShader);
	// error checking
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::string error = "Failed to compile fragment shader! Error log: ";
		error += infoLog;
		LOG_ERROR(error, false);
		system("PAUSE");
		exit(-1);
	}
	// shader program
	mShader = glCreateProgram();
	glAttachShader(mShader, vertexShader);
	glAttachShader(mShader, fragmentShader);
	glLinkProgram(mShader);
	// error checking
	glGetShaderiv(mShader, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[512];
		glGetShaderInfoLog(mShader, 512, nullptr, infoLog);
		std::string error = "Failed to failed to link shaders! Error log: ";
		error += infoLog;
		LOG_ERROR(error, false);
		system("PAUSE");
		exit(-1);
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


void Shader::bind()
{
	glUseProgram(mShader);
}

void Shader::addTexture(int location, std::string inShaderName)
{
	glUniform1i(glGetUniformLocation(mShader, inShaderName.c_str()), location);
}

unsigned int Shader::id() const
{
	return mShader;
}
