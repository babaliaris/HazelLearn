#include "pch.h"
#include "OpenGLShader.h"
#include <core/logger.h>
#include "glcall.h"
#include <glad/glad.h>


enum class ShaderType
{
	NONE = -1,
	VERTEX = 0,
	FRAGMENT = 1
};

struct ShaderSources
{
	std::string vertex_src;
	std::string fragment_src;
};


ShaderSources LoadShaderFromFile(const std::string& path);
unsigned int CompileShader(GLenum shader_type, const std::string& src, const std::string& path);




Amen::OpenGLShader::OpenGLShader(const std::string& filePath) : m_id(0)
{
	//Load the sources from a file.
	ShaderSources sources = LoadShaderFromFile(filePath);

	//The sources are invalid.
	if (sources.fragment_src.empty() && sources.vertex_src.empty())
		return;


	//Compile both shaders.
	unsigned int vertex_shader = CompileShader(GL_VERTEX_SHADER, sources.vertex_src, filePath);
	unsigned int fragment_shader = CompileShader(GL_FRAGMENT_SHADER, sources.fragment_src, filePath);

	//One of the shaders failed to compile.
	if (vertex_shader == 0 || fragment_shader == 0)
	{
		//Try deleting the vertex_shader.
		if (vertex_shader > 0)
		{
			GLCall(glDeleteShader(vertex_shader));
		}

		//Try deleting the fragment shader.
		if (fragment_shader > 0)
		{
			GLCall(glDeleteShader(fragment_shader));
		}

		return;
	}

	//Create the program and attach the shaders.
	GLCall(m_id = glCreateProgram());
	GLCall(glAttachShader(m_id, vertex_shader));
	GLCall(glAttachShader(m_id, fragment_shader));

	//Link the program.
	GLCall(glLinkProgram(m_id));

	//Delete the shaders.
	GLCall(glDeleteShader(vertex_shader));
	GLCall(glDeleteShader(fragment_shader));

	//Some variables to be user below.
	int link_success, validate_success, length;

	//Get required info for linking status.
	GLCall(glGetProgramiv(m_id, GL_LINK_STATUS, &link_success));
	GLCall(glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length));

	//Linking failed.
	if (!link_success)
	{
		//Allocate memory for log message.
		char* message = new char[length];

		//Retrieve the log message.
		GLCall(glGetProgramInfoLog(m_id, length, NULL, message));

		//Log a warning.
		AMEN_ERROR("Shader linkage failed: %s\n%s", filePath.c_str(), message);

		//Delete the message.
		delete[] message;

		//Delete the program.
		GLCall(glDeleteProgram(m_id));

		return;
	}


	//Validate the Program.
	GLCall(glValidateProgram(m_id));

	//Get required info for validation status.
	GLCall(glGetProgramiv(m_id, GL_VALIDATE_STATUS, &validate_success));
	GLCall(glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length));

	//Validation failed.
	if (!validate_success)
	{
		//Allocate memory for log message.
		char* message = new char[length];

		//Retrieve the log message.
		GLCall(glGetProgramInfoLog(m_id, length, NULL, message));

		//Log a warning.
		AMEN_ERROR("Shader validation failed: %s\n%s", filePath.c_str(), message);

		//Delete the message.
		delete[] message;

		//Delete the program.
		GLCall(glDeleteProgram(m_id));

		return;
	}

	//The program has been linked and validated successfully.
	m_isValid = true;

	AMEN_INFO("Shader created!!!");
}





Amen::OpenGLShader::~OpenGLShader()
{
	AMEN_INFO("Calling ~OpenGLShader()");
	GLCall(glDeleteProgram(m_id));
}





void Amen::OpenGLShader::Bind()
{
	GLCall(glUseProgram(m_id));
}





void Amen::OpenGLShader::UnBind()
{
	GLCall(glUseProgram(0));
}




//Load a shader from a file.
ShaderSources LoadShaderFromFile(const std::string& path)
{
	//Open the file.
	std::ifstream file(path);

	//Create source object.
	ShaderSources src;

	//Create shader type variable.
	ShaderType type = ShaderType::NONE;

	//Create two streams.
	std::stringstream stream[2];

	//The file could not be opened.
	if (!file.is_open())
	{
		AMEN_ERROR("File: %s, could not be opened.", path.c_str());
	}

	//Read the file line by line.
	std::string line;
	while (std::getline(file, line))
	{
		//Shader Decoration found.
		if (line.find("#shader") != std::string::npos)
		{
			//Indicates the start of a Vertex Shader.
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;

			//Indicates the start of a Fragment Shader.
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;

			//Shader decoration is invalid.
			else
			{
				AMEN_ERROR("Shader: %s, has an invalid #shader decoration.", path.c_str());
				return src;
			}
		}

		//This is normal shader code.
		else
		{
			//The type must not be none at this stage.
			if (type == ShaderType::NONE)
			{
				AMEN_ERROR("Shader: %s, has invalid #shader decorations.", path.c_str());
				return src;
			}

			//Add this line of code to the appropriate string stream.
			stream[(int)type] << line << std::endl;
		}
	}

	//Set the strings.
	src.vertex_src = stream[(int)ShaderType::VERTEX].str();
	src.fragment_src = stream[(int)ShaderType::FRAGMENT].str();

	//Return the source object.
	return src;
}



unsigned int CompileShader(GLenum shader_type, const std::string& src, const std::string& path)
{
	//Get the c_str() pointer of the string.
	const char* src_ptr = src.c_str();

	//Create a new shader object.
	GLCall(unsigned int shader_id = glCreateShader(shader_type));

	//Assign the source code.
	GLCall(glShaderSource(shader_id, 1, &src_ptr, NULL));

	//Compile the source code.
	GLCall(glCompileShader(shader_id));

	//Get some values.
	int success, length;
	GLCall(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success));
	GLCall(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length));

	//Compilation failed.
	if (!success)
	{
		//Allocate memory for the log message.
		char* message = new char[length];

		//Ge the log message.
		GLCall(glGetShaderInfoLog(shader_id, length, NULL, message));

		//Log an error.
		AMEN_ERROR("Shader compilation failed: %s\n%s", path.c_str(), message);

		//Delete the shader.
		GLCall(glDeleteShader(shader_id));

		//Delete message string.
		delete[] message;

		return 0;
	}

	return shader_id;
}