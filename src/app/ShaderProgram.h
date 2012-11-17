// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include "../Config.h"
#include "Vector3.h"

class ShaderProgram
{
public:
	static void disable();

	ShaderProgram();
	~ShaderProgram();

	bool addFragShader(string fileName);
	bool addVertexShader(string fileName);
	bool link();

	string getInfoLog();

	void enable();

	void setUniform(const char* name, float value);
	void setUniform(const char* name, const Vector3& vec);
	void setUniform(const char* name, const float* param, size_t length);

private:
	GLuint programId;
	char* infoLog;

	map<string, GLenum> uniforms;

	bool loadTextFile(string fileName, char** sourceCode, size_t* sourceLength);
	bool addShader(string fileName, GLenum shaderType);
	void updateInfoLog(GLuint objectId);
};

#endif /* SHADERPROGRAM_H_ */
