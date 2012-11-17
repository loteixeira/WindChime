// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "ExtensionManager.h"


//
// static members
//
static bool anisotropy, shaders, multisample;
static float maxAnisotropy;


//
// windows extension pointers
//
#ifdef WIN32_ENV
static PFNGLGENPROGRAMSARBPROC glGenProgramsARB;
static PFNGLBINDPROGRAMARBPROC glBindProgramARB;
static PFNGLPROGRAMSTRINGARBPROC glProgramStringARB;
static PFNGLDELETEPROGRAMSNVPROC glDeleteProgramsARB;
static PFNGLPROGRAMLOCALPARAMETER4FVARBPROC glProgramLocalParameter4fvARB;
static PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB;
static PFNGLSHADERSOURCEARBPROC glShaderSourceARB;
static PFNGLCOMPILESHADERARBPROC glCompileShaderARB;
static PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB;
static PFNGLATTACHOBJECTARBPROC glAttachObjectARB;
static PFNGLLINKPROGRAMARBPROC glLinkProgramARB;
static PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB;
static PFNGLDELETEOBJECTARBPROC glDeleteObjectARB;
static PFNGLGETINFOLOGARBPROC glGetInfoLogARB;
static PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
static PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB;
static PFNGLUNIFORM1IVARBPROC glUniform1ivARB;
static PFNGLUNIFORM1FVARBPROC glUniform1fvARB;
static PFNGLUNIFORM2FVARBPROC glUniform2fvARB;
static PFNGLUNIFORM3FVARBPROC glUniform3fvARB;
static PFNGLUNIFORM4FVARBPROC glUniform4fvARB;
static PFNGLUNIFORMMATRIX2FVARBPROC glUniformMatrix2fvARB;
static PFNGLUNIFORMMATRIX3FVARBPROC glUniformMatrix3fvARB;
static PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB;
static PFNGLGETACTIVEUNIFORMARBPROC glGetActiveUniformARB;
#endif /* WIN32_ENV */


//
// methods
//

// multisample checking (OS specific code!)
#ifdef WIN32_ENV
LRESULT CALLBACK multisample_window_proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

bool ExtensionManager::checkMultisample(int* multisamplePixelformat, unsigned short depth, int samples)
{
	// create test window
	WNDCLASSEX wc;
	HWND hWnd;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = multisample_window_proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon = LoadIcon(NULL, NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "extensionmanager_multisample_test";
	wc.hIconSm = LoadIcon(NULL, NULL);

	if (!RegisterClassEx(&wc))
		return false;

	hWnd = CreateWindowEx(0, "extensionmanager_multisample_test", "", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1, 1, NULL, NULL, NULL, NULL);

	if (hWnd == NULL)
		return false;

	HDC hDC = GetDC(hWnd);
	PIXELFORMATDESCRIPTOR pfd;
	int pixel_format;

	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = depth;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pixel_format = ChoosePixelFormat(hDC, &pfd);

	if (pixel_format == 0)
		return false;

	if (SetPixelFormat(hDC, pixel_format, &pfd) == FALSE)
		return false;

	HGLRC hRC = wglCreateContext(hDC);

	if (hRC == NULL)
		return false;

    if (wglMakeCurrent(hDC, hRC) == FALSE)
		return false;

    // check if multisample extension is available
    if (!check("WGL_ARB_multisample"))
    	return false;

    // choose multisample pixel format
	PFNWGLCHOOSEPIXELFORMATARBPROC __wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");

	if (__wglChoosePixelFormatARB == NULL)
		return false;

	UINT num_formats;
	float f_attributes[] = { 0, 0 };

	int i_attributes[] = { WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
		WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
		WGL_COLOR_BITS_ARB, 24,
		WGL_ALPHA_BITS_ARB, 8,
		WGL_DEPTH_BITS_ARB, 16,
		WGL_STENCIL_BITS_ARB, 0,
		WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
		WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
		WGL_SAMPLES_ARB, samples,
		0, 0};

	bool result = __wglChoosePixelFormatARB(hDC, i_attributes, f_attributes, 1, multisamplePixelformat, &num_formats) == TRUE;

	if (!result || num_formats < 1)
		return false;

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);

	ReleaseDC(hWnd, hDC);
	DestroyWindow(hWnd);

	return true;
}
#endif /* WIN32_ENV */

// Non-OS specific code!
void ExtensionManager::initialize()
{
	anisotropy = check("GL_EXT_texture_filter_anisotropic");
	shaders = check("GL_ARB_fragment_shader") && check("GL_ARB_vertex_shader");

#ifdef WIN32_ENV
	multisample = check("WGL_ARB_multisample");
#else
	multisample = false;
#endif /* WIN32_ENV */

	// extensions report
	cout << "OpenGL Extensions: ";
	cout << "anisotropy " << (anisotropy ? "available" : "not available") << ", ";
	cout << "shaders " << (shaders ? "available" : "not available") << ", ";
	cout << "multisample " << (multisample ? "available" : "not available") << " ";
	cout << endl;

	if (anisotropy)
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	else
		maxAnisotropy = 0.0f;

	// get windows extension pointers
#ifdef WIN32_ENV
	glGenProgramsARB = (PFNGLGENPROGRAMSARBPROC) wglGetProcAddress("glGenProgramsARB");
	glBindProgramARB = (PFNGLBINDPROGRAMARBPROC) wglGetProcAddress("glBindProgramARB");
	glProgramStringARB = (PFNGLPROGRAMSTRINGARBPROC) wglGetProcAddress("glProgramStringARB");
	glDeleteProgramsARB = (PFNGLDELETEPROGRAMSNVPROC) wglGetProcAddress("glDeleteProgramsARB");
	glProgramLocalParameter4fvARB = (PFNGLPROGRAMLOCALPARAMETER4FVARBPROC) wglGetProcAddress("glProgramLocalParameter4fvARB");
	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC) wglGetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) wglGetProcAddress("glShaderSourceARB");
	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC) wglGetProcAddress("glCompileShaderARB");
	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) wglGetProcAddress("glCreateProgramObjectARB");
	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC) wglGetProcAddress("glAttachObjectARB");
	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) wglGetProcAddress("glLinkProgramARB");
	glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC) wglGetProcAddress("glUseProgramObjectARB");
	glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC) wglGetProcAddress("glDeleteObjectARB");
	glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) wglGetProcAddress("glGetInfoLogARB");
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) wglGetProcAddress("glGetObjectParameterivARB");
	glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC) wglGetProcAddress("glGetUniformLocationARB");
	glUniform4fvARB = (PFNGLUNIFORM4FVARBPROC) wglGetProcAddress("glUniform4fvARB");
	glUniform1ivARB = (PFNGLUNIFORM1IVARBPROC) wglGetProcAddress("glUniform1ivARB");
	glUniform1fvARB = (PFNGLUNIFORM1FVARBPROC) wglGetProcAddress("glUniform1fvARB");
	glUniform2fvARB = (PFNGLUNIFORM2FVARBPROC) wglGetProcAddress("glUniform2fvARB");
	glUniform3fvARB = (PFNGLUNIFORM3FVARBPROC) wglGetProcAddress("glUniform3fvARB");
	glUniformMatrix2fvARB = (PFNGLUNIFORMMATRIX2FVARBPROC) wglGetProcAddress("glUniformMatrix2fvARB");
	glUniformMatrix3fvARB = (PFNGLUNIFORMMATRIX3FVARBPROC) wglGetProcAddress("glUniformMatrix3fvARB");
	glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC) wglGetProcAddress("glUniformMatrix4fvARB");
	glGetActiveUniformARB = (PFNGLGETACTIVEUNIFORMARBPROC) wglGetProcAddress("glGetActiveUniformARB");
#endif /* WIN32_ENV */
}

bool ExtensionManager::check(string extensionName)
{
#ifdef WIN32_ENV
	PROC wglGetExtensionsStringARB = wglGetProcAddress("wglGetExtensionsStringARB");

	if (wglGetExtensionsStringARB != NULL)
		if (strstr((const char*)  ((char*(__stdcall*)(HDC))wglGetExtensionsStringARB)(wglGetCurrentDC()), extensionName.c_str()) != NULL);
			return true;
#endif /* WIN32_ENV */

	return strstr((const char*) glGetString(GL_EXTENSIONS), extensionName.c_str()) != NULL;
}

bool ExtensionManager::anisotropyAvailable()
{
	return anisotropy;
}

bool ExtensionManager::shadersAvailable()
{
	return shaders;
}

bool ExtensionManager::multisampleAvailable()
{
	return multisample;
}

float ExtensionManager::getMaxAnisotropy()
{
	return maxAnisotropy;
}

void ExtensionManager::disableShaders()
{
	shaders = false;
}

//
// extensions functions
//
void ExtensionManager::genPrograms(GLsizei n, GLuint* programs)
{
	if (glGenProgramsARB == NULL)
		return;

	glGenProgramsARB(n, programs);
}

void ExtensionManager::bindProgram(GLenum target, GLuint program)
{
	if (glBindProgramARB == NULL)
		return;

	glBindProgramARB(target, program);
}

void ExtensionManager::programString(GLenum target, GLenum format, GLsizei len, const GLvoid* string)
{
	if (glProgramStringARB == NULL)
		return;

	glProgramStringARB(target, format, len, string);
}

void ExtensionManager::deletePrograms(GLsizei n, const GLuint* programs)
{
	if (glDeleteProgramsARB == NULL)
		return;

	glDeleteProgramsARB(n, programs);
}

void ExtensionManager::programLocalParameter4fv(GLenum n, GLuint i, const GLfloat* f)
{
	if (glProgramLocalParameter4fvARB == NULL)
		return;

	glProgramLocalParameter4fvARB(n, i, f);
}

GLuint ExtensionManager::createShaderObject(GLenum shader_type)
{
	if (glCreateShaderObjectARB == NULL)
		return 0;

	return glCreateShaderObjectARB(shader_type);
}

void ExtensionManager::shaderSource(GLuint shader, GLint num_of_strings, const char** strings, GLint* len_of_strings)
{
	if (glShaderSourceARB == NULL)
		return;

	glShaderSourceARB(shader, num_of_strings, strings, len_of_strings);
}

void ExtensionManager::compileShader(GLuint shader)
{
	if (glCompileShaderARB == NULL)
		return;

	glCompileShaderARB(shader);
}

GLuint ExtensionManager::createProgramObject()
{
	if (glCreateProgramObjectARB == NULL)
		return 0;

	return glCreateProgramObjectARB();
}

void ExtensionManager::attachObject(GLuint program, GLuint shader)
{
	if (glAttachObjectARB == NULL)
		return;

	glAttachObjectARB(program, shader);
}

void ExtensionManager::linkProgram(GLuint program)
{
	if (glLinkProgramARB == NULL)
		return;

	glLinkProgramARB(program);
}

void ExtensionManager::useProgramObject(GLuint program)
{
	if (glUseProgramObjectARB == NULL)
		return;

	glUseProgramObjectARB(program);
}

void ExtensionManager::deleteObject(GLuint object)
{
	if (glDeleteObjectARB == NULL)
		return;

	glDeleteObjectARB(object);
}

void ExtensionManager::getInfoLog(GLuint object, GLsizei max_length, GLsizei* length, char* infolog)
{
	if (glGetInfoLogARB == NULL)
		return;

	glGetInfoLogARB(object, max_length, length, infolog);
}

void ExtensionManager::getObjectParameteriv(GLuint object, GLenum type, GLint* param)
{
	if (glGetObjectParameterivARB == NULL)
		return;

	glGetObjectParameterivARB(object, type, param);
}

GLint ExtensionManager::getUniformLocation(GLuint program, const char* name)
{
	if (glGetUniformLocationARB == NULL)
		return 0;

	return glGetUniformLocationARB(program, name);
}

void ExtensionManager::uniform1iv(GLint location, GLsizei count, const GLint* v)
{
	if (glUniform1ivARB == NULL)
		return;

	glUniform1ivARB(location, count, v);
}

void ExtensionManager::uniform1fv(GLint location, GLsizei count, const GLfloat* v)
{
	if (glUniform1fvARB == NULL)
		return;

	glUniform1fvARB(location, count, v);
}

void ExtensionManager::uniform2fv(GLint location, GLsizei count, const GLfloat* v)
{
	if (glUniform2fvARB == NULL)
		return;

	glUniform2fvARB(location, count, v);
}

void ExtensionManager::uniform3fv(GLint location, GLsizei count, const GLfloat* v)
{
	if (glUniform3fvARB == NULL)
		return;

	glUniform3fvARB(location, count, v);
}

void ExtensionManager::uniform4fv(GLint location, GLsizei count, const GLfloat* v)
{
	if (glUniform4fvARB == NULL)
		return;

	glUniform4fvARB(location, count, v);
}

void ExtensionManager::uniformMatrix2fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* v)
{
	if (glUniformMatrix2fvARB == NULL)
		return;

	glUniformMatrix2fvARB(location, count, transpose, v);
}

void ExtensionManager::uniformMatrix3fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* v)
{
	if (glUniformMatrix3fvARB == NULL)
		return;

	glUniformMatrix3fvARB(location, count, transpose, v);
}

void ExtensionManager::uniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat* v)
{
	if (glUniformMatrix4fvARB == NULL)
		return;

	glUniformMatrix4fvARB(location, count, transpose, v);
}

void ExtensionManager::getActiveUniform(GLuint program, GLuint index, GLsizei max_length, GLsizei* length, GLint* size, GLenum* type, char* name)
{
	if (glGetActiveUniformARB == NULL)
		return;

	glGetActiveUniformARB(program, index, max_length, length, size, type, name);
}
