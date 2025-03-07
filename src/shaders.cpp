#include "shaders.h"
#include <iostream>
#include <vector>
#include <fstream>

// helper method
std::string readFile(const char * filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}

Shaders::Shaders()
{
}

GLuint Shaders::loadShader(GLenum type, const char *source)
{
	//create a shader
	GLuint shader = glCreateShader(type);
	if (shader == 0)
	{
		std::cerr << "Error creating shader" << std::endl;
		return 0;
	}

	//load the shader source to the shader object and compile it
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	//check if the shader compiled successfully
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		for(auto val : errorLog)
		{
			std::cout << val;
		}
		
		std::cerr << "Shader compilation error" << std::endl;
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

GLuint Shaders::buildProgram(const char * vertexShader, const char * fragmentShader)
{
	GLuint vertID = loadShader(GL_VERTEX_SHADER, vertexShader);
	GLuint fragID = loadShader(GL_FRAGMENT_SHADER, fragmentShader);

	//create a GL program and link it
	GLuint programObject = glCreateProgram();
	glAttachShader(programObject, vertID);
	glAttachShader(programObject, fragID);
	glLinkProgram(programObject);

	//check if the program linked successfully
	GLint linked;
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		std::cerr << "Program link error" << std::endl;
		glDeleteProgram(programObject);
		return 0;
	}

	return programObject;
}

GLuint Shaders::buildProgramFromFiles(const char * vertexSourcePath, const char * fragSourcePath)
{
	std::string vert = readFile(vertexSourcePath);
	std::string frag = readFile(fragSourcePath);

	return buildProgram(vert.c_str(), frag.c_str());
}