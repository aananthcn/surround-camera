//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// ESShader.c
//
//    Utility functions for loading shaders and creating program objects.
//

///
//  Includes
//
#include "esUtil.h"
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////
//
//  Private Functions
//
//



//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

//
///
/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param shaderSrc Shader source string
/// \return A new shader object on success, 0 on failure
//
GLuint ESUTIL_API esLoadShader(GLenum type, const char *shaderSrc)
{
	GLuint shader;
	GLint compiled;

	// Filter for valid shader types
	if ((type != GL_VERTEX_SHADER) && (type != GL_FRAGMENT_SHADER)) {
		printf("%s(): invalid shader type = %d\n", __func__, type);
		return 0; //failure
	}

	// Create the shader object
	shader = glCreateShader(type);
	if (shader == 0) {
		printf("glCreateShader() failed for type = %d\n",type);
		return 0; //failure
	}

	// Load the shader source
	glShaderSource(shader, 1, &shaderSrc, NULL);
	// Compile the shader
	glCompileShader(shader);
	// Check the compile status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1)
		{
			char* infoLog = malloc(sizeof(char) * infoLen);

			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			if(type == GL_VERTEX_SHADER)
				printf("Error compiling vertex shader:\n%s\n",
				       infoLog );
			else
				printf("Error compiling fragment shader:\n%s\n",
				       infoLog );

			printf("the shader is: \n %s\n",shaderSrc);
			free(infoLog);
		}

		glDeleteShader(shader);
		return 0; //failure
	}

	return shader;
}


//
///
/// \brief Load a vertex and fragment shader, create a program object, link program.
//         Errors output to log.
/// \param vertShaderSrc Vertex shader source code
/// \param fragShaderSrc Fragment shader source code
/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure
//
GLuint ESUTIL_API esLoadProgram( const char *vertShaderSrc,
				 const char *fragShaderSrc )
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	/*
	   const char* vert = 
	   "attribute vec4 a_position;\n"  
	   "void main()\n"
	   "{\n"
	   "gl_Position = a_position;\n"
	   "}\n"
	   ; 

	   const char* frag =   
	   "precision mediump float;\n"
	   "void main()\n"
	   "{\n"
	   "	gl_FragColor =  vec4(1.0);\n"
	   "}\n"
	   ;
	*/

	// Load the vertex/fragment shaders
	vertexShader = esLoadShader(GL_VERTEX_SHADER, vertShaderSrc);
	if (vertexShader == 0) {
		printf("vertex shader loading failed!\n");
		return -5;
	}

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fragShaderSrc);
	if (fragmentShader == 0) {
		printf("fragment shader loading failed!\n");
		glDeleteShader( vertexShader );
		return -6;
	}

	// Create the program object
	programObject = glCreateProgram();

	if ( programObject == 0 )
		return -7;

	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);

	// Link the program
	glLinkProgram(programObject);

	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);

	if ( !linked ) {
		GLint infoLen = 0;

		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if ( infoLen > 1 ) {
			char* infoLog = malloc (sizeof(char) * infoLen );

			glGetProgramInfoLog(programObject, infoLen, NULL,
					   infoLog );
			printf ( "Error linking program:\n%s\n", infoLog );
			free ( infoLog );
		}

		glDeleteProgram ( programObject );
		return 0;
	}

	// Free up no longer needed shader resources
	glDeleteShader ( vertexShader );
	glDeleteShader ( fragmentShader );

	return programObject;
}
