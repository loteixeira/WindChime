// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "ShaderProgram.h"
#include "ExtensionManager.h"

void ShaderProgram::disable()
{
	ExtensionManager::useProgramObject(0);
}

ShaderProgram::ShaderProgram()
{
	infoLog = NULL;
	programId = ExtensionManager::createProgramObject();
}

ShaderProgram::~ShaderProgram()
{
	if (infoLog != NULL)
		delete infoLog;

	ExtensionManager::deleteObject(programId);
}

bool ShaderProgram::addFragShader(string fileName)
{
	return addShader(fileName, GL_FRAGMENT_SHADER_ARB);
}

bool ShaderProgram::addVertexShader(string fileName)
{
	return addShader(fileName, GL_VERTEX_SHADER_ARB);
}

bool ShaderProgram::link()
{
	ExtensionManager::linkProgram(programId);
	updateInfoLog(programId);

	GLint result;
	ExtensionManager::getObjectParameteriv(programId, GL_OBJECT_LINK_STATUS_ARB, &result);

	if (result == GL_TRUE)
	{
		uniforms.clear();
		GLint uniform_count;
		ExtensionManager::getObjectParameteriv(programId, GL_OBJECT_ACTIVE_UNIFORMS_ARB, &uniform_count);

		if (uniform_count > 0)
		{
			GLint max_length;
			ExtensionManager::getObjectParameteriv(programId, GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB, &max_length);

			if (max_length > 0)
			{
				char* buffer = new char[max_length];

				for (GLint i = 0; i < uniform_count; i++)
				{
					GLenum type;
					GLint size;
					memset(buffer, 0, max_length);
					ExtensionManager::getActiveUniform(programId, i, max_length, 0, &size, &type, buffer);
					uniforms[buffer] = type;
				}

				delete buffer;
			}
		}

		return true;
	}

	return false;
}

string ShaderProgram::getInfoLog()
{
	if (infoLog == NULL)
		return "";

	return infoLog;
}

void ShaderProgram::enable()
{
	ExtensionManager::useProgramObject(programId);
}

void ShaderProgram::setUniform(const char* name, float value)
{
	GLuint location = ExtensionManager::getUniformLocation(programId, name);
	ExtensionManager::uniform1fv(location, 1, &value);
}

void ShaderProgram::setUniform(const char* name, const Vector3& vec)
{
	GLuint location = ExtensionManager::getUniformLocation(programId, name);
	GLfloat buffer[4] = { vec.x, vec.y, vec.z, 0.0f };
	ExtensionManager::uniform3fv(location, 1, buffer);
}

void ShaderProgram::setUniform(const char* name, const float* param, size_t length)
{
	GLuint location = ExtensionManager::getUniformLocation(programId, name);
	GLenum type = uniforms[name];

	switch (type)
	{
	case GL_FLOAT:
		ExtensionManager::uniform1fv(location, length, param);
		break;

	case GL_FLOAT_VEC2_ARB:
		ExtensionManager::uniform2fv(location, length / 2, param);
		break;

	case GL_FLOAT_VEC3_ARB:
		ExtensionManager::uniform3fv(location, length / 3, param);
		break;

	case GL_FLOAT_VEC4_ARB:
		ExtensionManager::uniform4fv(location, length / 4, param);
		break;

	case GL_FLOAT_MAT2_ARB:
		ExtensionManager::uniformMatrix2fv(location, length / 4, false, param);
		break;

	case GL_FLOAT_MAT3_ARB:
		ExtensionManager::uniformMatrix3fv(location, length / 9, false, param);
		break;

	case GL_FLOAT_MAT4_ARB:
		ExtensionManager::uniformMatrix4fv(location, length / 16, false, param);
		break;
	}
}

bool ShaderProgram::loadTextFile(string fileName, char** sourceCode, size_t* sourceLength)
{
	FILE* file;

	file = fopen(fileName.c_str(), "rb");

	if (file == NULL)
		return false;

	fseek(file, 0, SEEK_END);
	*sourceLength = ftell(file);
	rewind(file);

	*sourceCode = new char[*sourceLength + 1];
	fread(*sourceCode, *sourceLength, 1, file);
	fclose(file);

	(*sourceCode)[*sourceLength] = 0;

	return true;
}

bool ShaderProgram::addShader(string fileName, GLenum shaderType)
{
	char* sourceCode;
	size_t sourceLength;

	if (!loadTextFile(fileName, &sourceCode, &sourceLength))
		return false;

	GLuint shaderId = ExtensionManager::createShaderObject(shaderType);
	ExtensionManager::shaderSource(shaderId, 1, (const char**) &sourceCode, NULL);
	ExtensionManager::compileShader(shaderId);
	ExtensionManager::attachObject(programId, shaderId);

	updateInfoLog(shaderId);

	delete sourceCode;

	return infoLog == NULL;
}

void ShaderProgram::updateInfoLog(GLuint objectId)
{
	GLint size;

	if (infoLog != NULL)
		delete infoLog;

	ExtensionManager::getObjectParameteriv(objectId, GL_OBJECT_INFO_LOG_LENGTH_ARB, &size);

	if (size > 1)
	{
		GLint total;
		infoLog = new char[size];
		ExtensionManager::getInfoLog(objectId, size, &total, infoLog);
	}
	else
	{
		infoLog = NULL;
	}
}
