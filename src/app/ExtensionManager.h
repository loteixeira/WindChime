// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#ifndef EXTENSIONMANAGER_H_
#define EXTENSIONMANAGER_H_

#include "../Config.h"

class ExtensionManager
{
public:
#ifdef WIN32_ENV
	static bool checkMultisample(int* multisamplePixelformat, unsigned short depth, int samples);
#endif /* WIN32_ENV */

	static void initialize();
	static bool check(string extensionName);

	static bool anisotropyAvailable();
	static bool shadersAvailable();
	static bool multisampleAvailable();

	static float getMaxAnisotropy();
	static void disableShaders();

	static void genPrograms(GLsizei n, GLuint* programs);
	static void bindProgram(GLenum target, GLuint program);
	static void programString(GLenum target, GLenum format, GLsizei len, const GLvoid* string);
	static void deletePrograms(GLsizei n, const GLuint* programs);
	static void programLocalParameter4fv(GLenum n, GLuint i, const GLfloat* f);
	static GLuint createShaderObject(GLenum shader_type);
	static void shaderSource(GLuint shader, GLint num_of_strings, const char** strings, GLint* len_of_strings);
	static void compileShader(GLuint shader);
	static GLuint createProgramObject();
	static void attachObject(GLuint program, GLuint shader);
	static void linkProgram(GLuint program);
	static void useProgramObject(GLuint program);
	static void deleteObject(GLuint object);
	static void getInfoLog(GLuint object, GLsizei max_length, GLsizei* length, char* infolog);
	static void getObjectParameteriv(GLuint object, GLenum type, GLint* param);
	static GLint getUniformLocation(GLuint program, const char* name);
	static void uniform1iv(GLint location, GLsizei count, const GLint* v);
	static void uniform1fv(GLint location, GLsizei count, const GLfloat* v);
	static void uniform2fv(GLint location, GLsizei count, const GLfloat* v);
	static void uniform3fv(GLint location, GLsizei count, const GLfloat* v);
	static void uniform4fv(GLint location, GLsizei count, const GLfloat* v);
	static void uniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* v);
	static void uniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* v);
	static void uniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* v);
	static void getActiveUniform(GLuint program, GLuint index, GLsizei max_length, GLsizei* length, GLint* size, GLenum* type, char* name);
};

#endif /* EXTENSIONMANAGER_H_ */
