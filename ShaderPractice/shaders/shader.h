#ifndef SHADER_H
#define SHADER_H

#include "../glad/glad.h" // include glad to get the required OpenGL headers
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	// the program ID
	unsigned int ID;
	// constructor reads and builds the shader
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		//Retrieve vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		//Ensure we can throw exceptions
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			//Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			//Read file's buffer contents into the streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			//Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			printf("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n");
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		//Compile shaders
		unsigned int vertex = compileShader(GL_VERTEX_SHADER, vShaderCode);
		unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fShaderCode);

		//Create a program
		ID = createShader(vertex, fragment);



	}


	// use/activate the shader
	void use()
	{
		glUseProgram(ID);
	}
	// utility uniform functions
	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

private:
	static unsigned int compileShader(GLuint type, const std::string& source)
	{
		unsigned int id = glCreateShader(type);
		const char* src = source.c_str();
		glShaderSource(id, 1, &src, NULL);
		glCompileShader(id);

		//Testing for success
		int result;
		glGetShaderiv(id, GL_COMPILE_STATUS, &result);
		if (result == GL_FALSE)
		{
			int length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			char* message = (char*)(alloca(length * sizeof(char)));
			glGetShaderInfoLog(id, length, &length, message);
			printf("Failed to compile %s shader\n%s\n", (type == GL_VERTEX_SHADER ? "vertex" : "fragment"), message);
			glDeleteShader(id);
			return 0;
		}
		return id;
	}

	//Function for reading shader files
	static unsigned int createShader(GLuint vs, GLuint fs)
	{
		//Prepare shaders for attaching to program
		GLuint program = glCreateProgram();
		//GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
		//GLuint fs = compileShader(GL_VERTEX_SHADER, fragmentShader);

		//Create and attach shaders to program
		glAttachShader(program, vs);
		glAttachShader(program, fs);
		glLinkProgram(program);
		int success;
		char infoLog[512];
		//glValidateProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infoLog);
			printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
		}
		//Free shaders
		glDeleteShader(vs);
		glDeleteShader(fs);

		return program;
	}
};
#endif
