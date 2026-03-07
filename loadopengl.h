#ifndef INCLUDE_SRC_LOADOPENGL_H_
#define INCLUDE_SRC_LOADOPENGL_H_

#include "yawl.h"
#ifdef YAWL_ANDROID
#include <GLES3/gl3.h>
#else
#include <GL/glcorearb.h>
#endif // YAWL_ANDROID
#include <stdbool.h>

PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLBUFFERSUBDATAPROC glBufferSubData;
PFNGLTEXPARAMETERFVPROC glTexParameterfv;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLVIEWPORTPROC glViewport;
PFNGLCLEARCOLORPROC glClearColor;
PFNGLCLEARPROC glClear;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLDRAWARRAYSPROC glDrawArrays;
PFNGLDRAWELEMENTSPROC glDrawElements;
PFNGLGENTEXTURESPROC glGenTextures;
PFNGLBINDTEXTUREPROC glBindTexture;
PFNGLTEXIMAGE2DPROC glTexImage2D;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLTEXPARAMETERIPROC glTexParameteri;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLGETERRORPROC glGetError;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLACTIVETEXTUREPROC glActiveTexture;
PFNGLENABLEPROC glEnable;
PFNGLCULLFACEPROC glCullFace;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORM2FPROC glUniform2f;
bool load_gl_functions(YwState *s);

#ifdef LOADOPENGL_IMPLEMENTATION
#include "loadopengl.h"
#define YW_LOAD_GL_FUNC(dest)                                          \
	do {                                                           \
		if (!YwGLLoadProc(s, (void **)&dest, #dest)) {         \
			fprintf(stderr, "Failed to load %s\n", #dest); \
			return false;                                  \
		}                                                      \
	} while (0)

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

	return true;
}

#endif // LOADOPENGL_IMPLEMENTATION
#endif // INCLUDE_SRC_LOADOPENGL_H_
