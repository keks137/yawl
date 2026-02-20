#ifndef INCLUDE_SRC_LOADOPENGL_H_
#define INCLUDE_SRC_LOADOPENGL_H_

#include "yawl.h"
#include <GL/glcorearb.h>
#include <stdbool.h>

struct GLFuncs {
	PFNGLVERTEXATTRIBIPOINTERPROC VertexAttribIPointer;
	PFNGLUNIFORM1FPROC Uniform1f;
	PFNGLUNIFORM3FVPROC Uniform3fv;
	PFNGLBUFFERSUBDATAPROC BufferSubData;
	PFNGLTEXPARAMETERFVPROC TexParameterfv;
	PFNGLDELETEVERTEXARRAYSPROC DeleteVertexArrays;
	PFNGLDELETEBUFFERSPROC DeleteBuffers;
	PFNGLVIEWPORTPROC Viewport;
	PFNGLCLEARCOLORPROC ClearColor;
	PFNGLCLEARPROC Clear;
	PFNGLGENBUFFERSPROC GenBuffers;
	PFNGLBINDBUFFERPROC BindBuffer;
	PFNGLBUFFERDATAPROC BufferData;
	PFNGLCREATESHADERPROC CreateShader;
	PFNGLSHADERSOURCEPROC ShaderSource;
	PFNGLCOMPILESHADERPROC CompileShader;
	PFNGLGETSHADERIVPROC GetShaderiv;
	PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
	PFNGLCREATEPROGRAMPROC CreateProgram;
	PFNGLATTACHSHADERPROC AttachShader;
	PFNGLLINKPROGRAMPROC LinkProgram;
	PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
	PFNGLGETPROGRAMIVPROC GetProgramiv;
	PFNGLUSEPROGRAMPROC UseProgram;
	PFNGLDELETESHADERPROC DeleteShader;
	PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
	PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
	PFNGLGENVERTEXARRAYSPROC GenVertexArrays;
	PFNGLBINDVERTEXARRAYPROC BindVertexArray;
	PFNGLDRAWARRAYSPROC DrawArrays;
	PFNGLDRAWELEMENTSPROC DrawElements;
	PFNGLPOLYGONMODEPROC PolygonMode;
	PFNGLGENTEXTURESPROC GenTextures;
	PFNGLBINDTEXTUREPROC BindTexture;
	PFNGLTEXIMAGE2DPROC TexImage2D;
	PFNGLGENERATEMIPMAPPROC GenerateMipmap;
	PFNGLTEXPARAMETERIPROC TexParameteri;
	PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
	PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
	PFNGLGETERRORPROC GetError;
	PFNGLUNIFORM1IPROC Uniform1i;
	PFNGLACTIVETEXTUREPROC ActiveTexture;
	PFNGLENABLEPROC Enable;
	PFNGLCULLFACEPROC CullFace;
	PFNGLUNIFORM3FPROC Uniform3f;
	PFNGLUNIFORM2FPROC Uniform2f;
};
bool load_gl_functions(YwState *s);

#ifdef LOADOPENGL_IMPLEMENTATION
#include "loadopengl.h"
#define YW_LOAD_GL_FUNC(dest, name)                                    \
	do {                                                           \
		if (!YwGLLoadProc(s, (void **)&dest, #name)) {         \
			fprintf(stderr, "Failed to load %s\n", #name); \
			return false;                                  \
		}                                                      \
	} while (0)

struct GLFuncs gl = { 0 };

bool load_gl_functions(YwState *s)
{
	YW_LOAD_GL_FUNC(gl.Uniform3f, glUniform3f);
	YW_LOAD_GL_FUNC(gl.Uniform2f, glUniform2f);
	YW_LOAD_GL_FUNC(gl.Enable, glEnable);
	YW_LOAD_GL_FUNC(gl.CullFace, glCullFace);
	YW_LOAD_GL_FUNC(gl.ActiveTexture, glActiveTexture);
	YW_LOAD_GL_FUNC(gl.Uniform1i, glUniform1i);
	YW_LOAD_GL_FUNC(gl.GetError, glGetError);
	YW_LOAD_GL_FUNC(gl.UniformMatrix4fv, glUniformMatrix4fv);
	YW_LOAD_GL_FUNC(gl.GetUniformLocation, glGetUniformLocation);
	YW_LOAD_GL_FUNC(gl.TexParameteri, glTexParameteri);
	YW_LOAD_GL_FUNC(gl.GenerateMipmap, glGenerateMipmap);
	YW_LOAD_GL_FUNC(gl.TexImage2D, glTexImage2D);
	YW_LOAD_GL_FUNC(gl.BindTexture, glBindTexture);
	YW_LOAD_GL_FUNC(gl.GenTextures, glGenTextures);
	YW_LOAD_GL_FUNC(gl.PolygonMode, glPolygonMode);
	YW_LOAD_GL_FUNC(gl.DrawElements, glDrawElements);
	YW_LOAD_GL_FUNC(gl.DrawArrays, glDrawArrays);
	YW_LOAD_GL_FUNC(gl.BindVertexArray, glBindVertexArray);
	YW_LOAD_GL_FUNC(gl.GenVertexArrays, glGenVertexArrays);
	YW_LOAD_GL_FUNC(gl.EnableVertexAttribArray, glEnableVertexAttribArray);
	YW_LOAD_GL_FUNC(gl.VertexAttribPointer, glVertexAttribPointer);
	YW_LOAD_GL_FUNC(gl.DeleteShader, glDeleteShader);
	YW_LOAD_GL_FUNC(gl.UseProgram, glUseProgram);
	YW_LOAD_GL_FUNC(gl.GetProgramiv, glGetProgramiv);
	YW_LOAD_GL_FUNC(gl.GetProgramInfoLog, glGetProgramInfoLog);
	YW_LOAD_GL_FUNC(gl.LinkProgram, glLinkProgram);
	YW_LOAD_GL_FUNC(gl.AttachShader, glAttachShader);
	YW_LOAD_GL_FUNC(gl.CreateProgram, glCreateProgram);
	YW_LOAD_GL_FUNC(gl.GetShaderInfoLog, glGetShaderInfoLog);
	YW_LOAD_GL_FUNC(gl.GetShaderiv, glGetShaderiv);
	YW_LOAD_GL_FUNC(gl.CompileShader, glCompileShader);
	YW_LOAD_GL_FUNC(gl.ShaderSource, glShaderSource);
	YW_LOAD_GL_FUNC(gl.CreateShader, glCreateShader);
	YW_LOAD_GL_FUNC(gl.BufferData, glBufferData);
	YW_LOAD_GL_FUNC(gl.BindBuffer, glBindBuffer);
	YW_LOAD_GL_FUNC(gl.GenBuffers, glGenBuffers);
	YW_LOAD_GL_FUNC(gl.Clear, glClear);
	YW_LOAD_GL_FUNC(gl.ClearColor, glClearColor);
	YW_LOAD_GL_FUNC(gl.Viewport, glViewport);
	YW_LOAD_GL_FUNC(gl.DeleteBuffers, glDeleteBuffers);
	YW_LOAD_GL_FUNC(gl.DeleteVertexArrays, glDeleteVertexArrays);
	YW_LOAD_GL_FUNC(gl.TexParameterfv, glTexParameterfv);
	YW_LOAD_GL_FUNC(gl.BufferSubData, glBufferSubData);
	YW_LOAD_GL_FUNC(gl.Uniform3fv, glUniform3fv);
	YW_LOAD_GL_FUNC(gl.Uniform1f, glUniform1f);
	YW_LOAD_GL_FUNC(gl.VertexAttribIPointer, glVertexAttribIPointer);

	return true;
}

#endif // LOADOPENGL_IMPLEMENTATION
#endif // INCLUDE_SRC_LOADOPENGL_H_
