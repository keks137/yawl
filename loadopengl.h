#ifndef INCLUDE_SRC_LOADOPENGL_H_
#define INCLUDE_SRC_LOADOPENGL_H_

#include "yawl.h"
bool load_gl_functions(YwState *s);
// #ifdef YAWL_ANDROID

// #include <GLES3/gl3.h>
// #else
#include <GL/glcorearb.h>
#include <stdbool.h>

extern PFNGLPIXELSTOREIPROC glPixelStorei;
extern PFNGLGETTEXLEVELPARAMETERIVPROC glGetTexLevelParameteriv;
extern PFNGLDISABLEPROC glDisable;
extern PFNGLISTEXTUREPROC glIsTexture;
extern PFNGLGETSTRINGIPROC glGetStringi;
extern PFNGLGETINTEGERVPROC glGetIntegerv;
extern PFNGLGETSTRINGPROC glGetString;
extern PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
extern PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
extern PFNGLUNIFORM1FPROC glUniform1f;
extern PFNGLUNIFORM3FVPROC glUniform3fv;
extern PFNGLBUFFERSUBDATAPROC glBufferSubData;
extern PFNGLTEXPARAMETERFVPROC glTexParameterfv;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;
extern PFNGLVIEWPORTPROC glViewport;
extern PFNGLCLEARCOLORPROC glClearColor;
extern PFNGLCLEARPROC glClear;
extern PFNGLBLENDFUNCPROC glBlendFunc;
extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLGETSHADERIVPROC glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
extern PFNGLGETPROGRAMIVPROC glGetProgramiv;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLDRAWARRAYSPROC glDrawArrays;
extern PFNGLDRAWELEMENTSPROC glDrawElements;
extern PFNGLGENTEXTURESPROC glGenTextures;
extern PFNGLBINDTEXTUREPROC glBindTexture;
extern PFNGLTEXIMAGE2DPROC glTexImage2D;
extern PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
extern PFNGLTEXPARAMETERIPROC glTexParameteri;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
extern PFNGLGETERRORPROC glGetError;
extern PFNGLUNIFORM1IPROC glUniform1i;
extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLENABLEPROC glEnable;
extern PFNGLCULLFACEPROC glCullFace;
extern PFNGLUNIFORM3FPROC glUniform3f;
extern PFNGLUNIFORM2FPROC glUniform2f;
extern PFNGLGETTEXPARAMETERIVPROC glGetTexParameteriv;
// #endif // YAWL_ANDROID

#ifdef LOADOPENGL_IMPLEMENTATION
// #ifdef YAWL_ANDROID
// #define YW_LOAD_GL_FUNC(dest) (void)0
// bool load_gl_functions(YwState *s)
// {
// 	return true; // NOTE: just link dynamically on android
// }
//
// #else
#define YW_LOAD_GL_FUNC(dest)                                          \
	do {                                                           \
		if (!YwGLLoadProc(s, (void **)&dest, #dest)) {         \
			fprintf(stderr, "Failed to load %s\n", #dest); \
			return false;                                  \
		}                                                      \
	} while (0)

PFNGLGETTEXLEVELPARAMETERIVPROC glGetTexLevelParameteriv = NULL;
PFNGLDISABLEPROC glDisable = NULL;
PFNGLISTEXTUREPROC glIsTexture = NULL;
PFNGLGETSTRINGIPROC glGetStringi = NULL;
PFNGLGETINTEGERVPROC glGetIntegerv = NULL;
PFNGLGETSTRINGPROC glGetString = NULL;
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback = NULL;
PFNGLGETTEXPARAMETERIVPROC glGetTexParameteriv = NULL;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer = NULL;
PFNGLUNIFORM1FPROC glUniform1f = NULL;
PFNGLUNIFORM3FVPROC glUniform3fv = NULL;
PFNGLBUFFERSUBDATAPROC glBufferSubData = NULL;
PFNGLTEXPARAMETERFVPROC glTexParameterfv = NULL;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = NULL;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = NULL;
PFNGLVIEWPORTPROC glViewport = NULL;
PFNGLCLEARCOLORPROC glClearColor = NULL;
PFNGLCLEARPROC glClear = NULL;
PFNGLBLENDFUNCPROC glBlendFunc = NULL;
PFNGLGENBUFFERSPROC glGenBuffers = NULL;
PFNGLBINDBUFFERPROC glBindBuffer = NULL;
PFNGLBUFFERDATAPROC glBufferData = NULL;
PFNGLCREATESHADERPROC glCreateShader = NULL;
PFNGLSHADERSOURCEPROC glShaderSource = NULL;
PFNGLCOMPILESHADERPROC glCompileShader = NULL;
PFNGLGETSHADERIVPROC glGetShaderiv = NULL;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = NULL;
PFNGLCREATEPROGRAMPROC glCreateProgram = NULL;
PFNGLATTACHSHADERPROC glAttachShader = NULL;
PFNGLLINKPROGRAMPROC glLinkProgram = NULL;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = NULL;
PFNGLGETPROGRAMIVPROC glGetProgramiv = NULL;
PFNGLUSEPROGRAMPROC glUseProgram = NULL;
PFNGLDELETESHADERPROC glDeleteShader = NULL;
PFNGLDELETEPROGRAMPROC glDeleteProgram = NULL;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer = NULL;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray = NULL;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays = NULL;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray = NULL;
PFNGLDRAWARRAYSPROC glDrawArrays = NULL;
PFNGLDRAWELEMENTSPROC glDrawElements = NULL;
PFNGLGENTEXTURESPROC glGenTextures = NULL;
PFNGLBINDTEXTUREPROC glBindTexture = NULL;
PFNGLTEXIMAGE2DPROC glTexImage2D = NULL;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = NULL;
PFNGLTEXPARAMETERIPROC glTexParameteri = NULL;
PFNGLPIXELSTOREIPROC glPixelStorei = NULL;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = NULL;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv = NULL;
PFNGLGETERRORPROC glGetError = NULL;
PFNGLUNIFORM1IPROC glUniform1i = NULL;
PFNGLACTIVETEXTUREPROC glActiveTexture = NULL;
PFNGLENABLEPROC glEnable = NULL;
PFNGLCULLFACEPROC glCullFace = NULL;
PFNGLUNIFORM3FPROC glUniform3f = NULL;
PFNGLUNIFORM2FPROC glUniform2f = NULL;

bool load_gl_functions(YwState *s)
{
	YW_LOAD_GL_FUNC(glUniform3f);
	YW_LOAD_GL_FUNC(glUniform2f);
	YW_LOAD_GL_FUNC(glEnable);
	YW_LOAD_GL_FUNC(glCullFace);
	YW_LOAD_GL_FUNC(glActiveTexture);
	YW_LOAD_GL_FUNC(glUniform1i);
	YW_LOAD_GL_FUNC(glGetError);
	YW_LOAD_GL_FUNC(glUniformMatrix4fv);
	YW_LOAD_GL_FUNC(glGetUniformLocation);
	YW_LOAD_GL_FUNC(glTexParameteri);
	YW_LOAD_GL_FUNC(glGenerateMipmap);
	YW_LOAD_GL_FUNC(glTexImage2D);
	YW_LOAD_GL_FUNC(glBindTexture);
	YW_LOAD_GL_FUNC(glGenTextures);
	YW_LOAD_GL_FUNC(glBlendFunc);
	YW_LOAD_GL_FUNC(glDrawElements);
	YW_LOAD_GL_FUNC(glDrawArrays);
	YW_LOAD_GL_FUNC(glBindVertexArray);
	YW_LOAD_GL_FUNC(glGenVertexArrays);
	YW_LOAD_GL_FUNC(glEnableVertexAttribArray);
	YW_LOAD_GL_FUNC(glVertexAttribPointer);
	YW_LOAD_GL_FUNC(glDeleteShader);
	YW_LOAD_GL_FUNC(glDeleteProgram);
	YW_LOAD_GL_FUNC(glUseProgram);
	YW_LOAD_GL_FUNC(glGetProgramiv);
	YW_LOAD_GL_FUNC(glGetProgramInfoLog);
	YW_LOAD_GL_FUNC(glLinkProgram);
	YW_LOAD_GL_FUNC(glAttachShader);
	YW_LOAD_GL_FUNC(glCreateProgram);
	YW_LOAD_GL_FUNC(glGetShaderInfoLog);
	YW_LOAD_GL_FUNC(glGetShaderiv);
	YW_LOAD_GL_FUNC(glCompileShader);
	YW_LOAD_GL_FUNC(glShaderSource);
	YW_LOAD_GL_FUNC(glCreateShader);
	YW_LOAD_GL_FUNC(glBufferData);
	YW_LOAD_GL_FUNC(glBindBuffer);
	YW_LOAD_GL_FUNC(glGenBuffers);
	YW_LOAD_GL_FUNC(glClear);
	YW_LOAD_GL_FUNC(glClearColor);
	YW_LOAD_GL_FUNC(glViewport);
	YW_LOAD_GL_FUNC(glDeleteBuffers);
	YW_LOAD_GL_FUNC(glDeleteVertexArrays);
	YW_LOAD_GL_FUNC(glTexParameterfv);
	YW_LOAD_GL_FUNC(glBufferSubData);
	YW_LOAD_GL_FUNC(glUniform3fv);
	YW_LOAD_GL_FUNC(glUniform1f);
	YW_LOAD_GL_FUNC(glVertexAttribIPointer);
	YW_LOAD_GL_FUNC(glGetTexParameteriv);
	YW_LOAD_GL_FUNC(glDebugMessageCallback);
	YW_LOAD_GL_FUNC(glGetString);
	YW_LOAD_GL_FUNC(glGetIntegerv);
	YW_LOAD_GL_FUNC(glGetStringi);
	YW_LOAD_GL_FUNC(glIsTexture);
	YW_LOAD_GL_FUNC(glDisable);
	YW_LOAD_GL_FUNC(glPixelStorei);
	YW_LOAD_GL_FUNC(glGetTexLevelParameteriv);

	return true;
}

// #endif // YAWL_ANDROID
#endif // LOADOPENGL_IMPLEMENTATION
#endif // INCLUDE_SRC_LOADOPENGL_H_
