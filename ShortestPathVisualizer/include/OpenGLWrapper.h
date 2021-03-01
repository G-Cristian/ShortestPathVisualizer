#pragma once

#ifdef OPENGLWRAPPER_EXPORTS
#define OPENGLWRAPPER_API __declspec(dllexport)
#else
#define OPENGLWRAPPER_API __declspec(dllimport)
#endif // OPENGLWRAPPER_EXPORTS

#include <GL/glew.h>
#include <string>

/// <summary>
/// Gets OpenGL errors.
/// The returned value can be used to help reading the buffer
/// </summary>
/// <param name="errorsBuffer">Buffer for put errors in</param>
/// <param name="bufferSize">Size of the buffer (number of errors that can be returned in this call)</param>
/// <param name="moreErrors">Indicates that there are more errors that didn't fit in the buffer.</param>
/// <returns>Number of errors.</returns>
extern "C" OPENGLWRAPPER_API int getOpenGLError(int* errorsBuffer, int bufferSize, bool* moreErrors);

/// <summary>
/// Gets OpenGL errors. Empty string if there were no errors.
/// </summary>
/// <param name="numberOfErrorsToShow">Number of errors to show per call (0 for no limit)</param>
/// <returns>std::string containing OpenGL errors (empty string if there were no errors).</returns>
OPENGLWRAPPER_API std::string __stdcall getOpenGLError(int numberOfErrorsToShow);

/// <summary>
/// Gets shader log corresponding to 'shader'.
/// </summary>
/// <param name="shader">Shader for which to get the log.</param>
/// <param name="logBuffers">Buffers for holding log lines. Each line will go to one position of 'buffers'.</param>
/// <param name="logSize">The size of each element in 'buffers' (i.e. The max size that can be read per line)</param>
/// <param name="numberOfBuffers">Number of buffers</param>
/// <param name="moreToRead">True if there are more values to read.</param>
/// <returns>Number of logs read (or error code)</returns>
extern "C" OPENGLWRAPPER_API int getShaderLog(GLuint shader, char**logBuffers, int logSize, int numberOfBuffers, bool *moreToRead);

/// <summary>
/// Gets shader log corresponding to 'shader'.
/// </summary>
/// <param name="shader">Shader for which to get the log.</param>
/// <returns>std::string containing shader log.</returns>
OPENGLWRAPPER_API std::string __stdcall getShaderLog(GLuint shader);

/// <summary>
/// Gets program log corresponding to 'program'.
/// </summary>
/// <param name="program">Program to get the log for.</param>
/// <returns>std::string containing program log.</returns>
OPENGLWRAPPER_API std::string __stdcall getProgramLog(GLuint program);

/// <summary>
/// Reads a text file containing a shader source and gets its content.
/// </summary>
/// <param name="shaderSourcePath">Path of the text file.</param>
/// <param name="returnCode">Out variable for setting a return code (1 if OK).</param>
/// <returns>std::string with source file content</returns>
OPENGLWRAPPER_API std::string __stdcall readShaderSource(const char* shaderSourcePath, int& returnCode);

/// <summary>
/// Creates and compiles a shader from a text file containing its content.
/// </summary>
/// <param name="shaderPath">Path of shader file.</param>
/// <param name="shaderType">OpenGL enum indicating the type of shader to create (examples: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, etc.)</param>
/// <param name="returnCode">Out variable for setting a return code (1 if OK).</param>
/// <param name="returnMsg">Out string for setting a return message.</param>
/// <returns>GLuint for a shader</returns>
OPENGLWRAPPER_API GLuint __stdcall createAndCompileShader(const char* shaderPath, GLenum shaderType, int& returnCode, std::string& returnMsg);

/// <summary>
/// Creates and compiles a vertex shader from a text file containing its content.
/// </summary>
/// <param name="shaderPath">Path of shader file.</param>
/// <param name="returnCode">Out variable for setting a return code (1 if OK).</param>
/// <param name="returnMsg">Out string for setting a return message.</param>
/// <returns>GLuint for a vertex shader</returns>
OPENGLWRAPPER_API GLuint __stdcall createAndCompileVertexShader(const char* shaderPath, int& returnCode, std::string& returnMsg);

/// <summary>
/// Creates and compiles a fragment shader from a text file containing its content.
/// </summary>
/// <param name="shaderPath">Path of shader file.</param>
/// <param name="returnCode">Out variable for setting a return code (1 if OK).</param>
/// <param name="returnMsg">Out string for setting a return message.</param>
/// <returns>GLuint for a fragment shader</returns>
OPENGLWRAPPER_API GLuint __stdcall createAndCompileFragmentShader(const char* shaderPath, int& returnCode, std::string& returnMsg);

/// <summary>
/// Creates a rendering program using a vertex and fragment shader files.
/// </summary>
/// <param name="vertexShaderFilePath">Vertex shader file path.</param>
/// <param name="fragmentShaderFilePath">Fragment shader file path.</param>
/// <param name="returnCode">Out variable for setting a return code (1 if OK).</param>
/// <param name="returnMsg">Out string for setting a return message.</param>
/// <returns>GLuint for rendering program</returns>
OPENGLWRAPPER_API GLuint __stdcall createRenderingProgram(const char* vertexShaderFilePath, const char* fragmentShaderFilePath, int& returnCode, std::string& returnMsg);

/// <summary>
/// Inits glew (calls glewInit())
/// Returns GLEW_OK if success
/// </summary>
/// <returns>Code returned by glewInit()</returns>
extern "C" OPENGLWRAPPER_API int initGlew();

/// <summary>
/// Clears color buffer (GL_COLOR_BUFFER_BIT)
/// </summary>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void clearColorBuffer();

/// <summary>
/// Clears depth buffer (GL_DEPTH_BUFFER_BIT)
/// </summary>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void clearDepthBuffer();

/// <summary>
/// Wrapper for glEnable
/// </summary>
/// <param name="attribute">Attribute to enable with glEnable (e.g. GL_CULL_FACE, GL_DEPTH_TEST)</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void enableGLAttribute(GLenum attribute);

/// <summary>
/// Enables cull face (glEnable(GL_CULL_FACE))
/// </summary>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void enableCullFace();

/// <summary>
/// Enables depth test (glEnable(GL_DEPTH_TEST))
/// </summary>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void enableDepthTest();

/// <summary>
/// Sets depth function to use (calls glDepthFunc)
/// </summary>
/// <param name="depthFunction">Depth function to use (e.g. GL_LEQUAL)</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void setDepthFunction(GLenum depthFunction);

/// <summary>
/// Sets depth function to GL_LEQUAL (glDepthFunc(GL_LEQUAL))
/// </summary>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void setDepthFunctionLessEqual();

/// <summary>
/// Sets clockwise (GL_CW) or counterclockwise (GL_CCW) by calling glFrontFace
/// </summary>
/// <param name="frontFace">GLenum for glFrontFace (GL_CW or GL_CCW)</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void setFrontFace(GLenum frontFace);

/// <summary>
/// Sets front face to clockwise (glFrontFace(GL_CW))
/// </summary>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void setFrontFaceClockwise();

/// <summary>
/// Sets front face to counter clockwise (glFrontFace(GL_CCW))
/// </summary>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void setFrontFaceCounterClockwise();

/// <summary>
/// Creates and binds a vertex array object.
/// </summary>
/// <returns>The vertex array object (VAO).</returns>
extern "C" OPENGLWRAPPER_API GLuint createAndBindVAO();

/// <summary>
/// Creates an array buffer (GL_ARRAY_BUFFER)
/// </summary>
/// <param name="attributes">Array of attributes.</param>
/// <param name="numberOfAttributes">Number of attributes.</param>
/// <returns>The vertex buffer object (VBO)</returns>
extern "C" OPENGLWRAPPER_API GLuint createArrayBuffer(const float* attributes, GLsizeiptr numberOfAttributes);

/// <summary>
/// Creates an element array buffer (GL_ELEMENT_ARRAY_BUFFER)
/// </summary>
/// <param name="attributes">Array of attributes.</param>
/// <param name="numberOfAttributes">Number of attributes.</param>
/// <returns>The vertex buffer object (VBO)</returns>
extern "C" OPENGLWRAPPER_API GLuint createElementArrayBuffer(const int* attributes, GLsizeiptr numberOfAttributes);

/// <summary>
/// Loads a texture and creates the buffer object
/// if there were no errors *returnCode equals 1
/// </summary>
/// <param name="textureImagePath">Path of the texture to load</param>
/// <param name="returnCode">Out variable used for setting a return value (errors, etc.)</param>
/// <returns>Buffer object for the texture</returns>
extern "C" OPENGLWRAPPER_API GLuint loadAndCreateTexture(const char* textureImagePath, int* returnCode);

/// <summary>
/// Gets the location for a uniform.
/// </summary>
/// <param name="renderingProgram">Renderign program</param>
/// <param name="attributeName">The name of the attribute in the shader</param>
/// <returns>GLint with the location.</returns>
extern "C" OPENGLWRAPPER_API GLint getUniformLocation(GLuint renderingProgram, const char* attributeName);

/// <summary>
/// Sends a matrix to a shader
/// </summary>
/// <param name="matrixLocation">Location in the shader</param>
/// <param name="matrix">Pointer of matrix to send</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void setUniformMatrix4fv(GLint matrixLocation, GLfloat* matrix);

/// <summary>
/// Sends int value to the rendering program in the specified location
/// </summary>
/// <param name="renderingProgram">Rendering program</param>
/// <param name="location">Location</param>
/// <param name="value">Int value to send</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void programUniform1i(GLuint renderingProgram, GLint location, GLint value);

/// <summary>
/// Sends float value to the rendering program in the specified location
/// </summary>
/// <param name="renderingProgram">Rendering program</param>
/// <param name="location">Location</param>
/// <param name="value">Float value to send</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void programUniform1f(GLuint renderingProgram, GLint location, GLfloat value);

/// <summary>
/// Sends an array of two elements to the rendering program in the specified location
/// </summary>
/// <param name="renderingProgram">Rendering program</param>
/// <param name="location">Location</param>
/// <param name="value">Array to send</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void programUniform2fv(GLuint renderingProgram, GLint location, const GLfloat * value);

/// <summary>
/// Sends an array of three elements to the rendering program in the specified location
/// </summary>
/// <param name="renderingProgram">Rendering program</param>
/// <param name="location">Location</param>
/// <param name="value">Array to send</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void programUniform3fv(GLuint renderingProgram, GLint location, const GLfloat * value);

/// <summary>
/// Sends an array of four elements to the rendering program in the specified location
/// </summary>
/// <param name="renderingProgram">Rendering program</param>
/// <param name="location">Location</param>
/// <param name="value">Array to send</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void programUniform4fv(GLuint renderingProgram, GLint location, const GLfloat * value);

/// <summary>
/// Binds a vertex array and enables it in the shader
/// (The attribute size is 2)
/// </summary>
/// <param name="index">Location in the shader</param>
/// <param name="vbo">Vertex array to bind</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void activateArrayBuffer2f(GLuint index, GLuint vbo);

/// <summary>
/// Binds a vertex array and enables it in the shader
/// (The attribute size is 3)
/// </summary>
/// <param name="index">Location in the shader</param>
/// <param name="vbo">Vertex array to bind</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void activateArrayBuffer3f(GLuint index, GLuint vbo);

/// <summary>
/// Activates a 2D texture and binds the vbo
/// </summary>
/// <param name="texture">Texture to activate (for example: GL_TEXTURE0)</param>
/// <param name="textureVBO">VBO to bind, corresponding to a texture.</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void activateTexture2D(GLenum texture, GLuint textureVBO);

/// <summary>
/// Activates texture 0 (GL_TEXTURE0) and binds the vbo which corresponds to a 2D texture (GL_TEXTURE_2D)
/// </summary>
/// <param name="textureVBO">VBO to bind, corresponding to a texture.</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void activateTexture02D(GLuint textureVBO);

/// <summary>
/// Draws triangles using vertices (uses glDrawArrays)
/// </summary>
/// <param name="numberOfVertices">Total number of vertices</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void drawTrianglesUsingVertices(GLsizei numberOfVertices);

/// <summary>
/// Binds an element array containing indices and draws triangles using them (uses glDrawElements)
/// </summary>
/// <param name="indices">VBO of the indices</param>
/// <param name="numberOfIndices">Total number of indices</param>
/// <returns></returns>
extern "C" OPENGLWRAPPER_API void drawTrianglesUsingIndices(GLuint indices, GLsizei numberOfIndices);
