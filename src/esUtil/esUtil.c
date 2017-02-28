//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// ESUtil.c
//
//    A utility library for OpenGL ES.  This library provides a
//    basic common framework for the example applications in the
//    OpenGL ES 2.0 Programming Guide.
//

///
//  Includes
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include "esUtil.h"
#include "esUtil_win.h"




///
// CreateEGLContext()
//
//    Creates an EGL rendering context and all associated elements
//
EGLBoolean CreateEGLContext ( EGLNativeWindowType hWnd, EGLNativeDisplayType nativeDisplay, EGLDisplay* eglDisplay,
			      EGLContext* eglContext, EGLSurface* eglSurface,
			      EGLint attribList[])
{
	EGLint numConfigs;
	EGLint majorVersion;
	EGLint minorVersion;
	EGLDisplay display;
	EGLContext context;
	EGLSurface surface;
	EGLConfig config;
	EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

	// Get Display
	display = eglGetDisplay(nativeDisplay);
	if ( display == EGL_NO_DISPLAY )
	{
		return EGL_FALSE;
	}

	// Initialize EGL
	if ( !eglInitialize(display, &majorVersion, &minorVersion) )
	{
		return EGL_FALSE;
	}

	// Get configs
	if ( !eglGetConfigs(display, NULL, 0, &numConfigs) )
	{
		return EGL_FALSE;
	}

	// Choose config
	if ( !eglChooseConfig(display, attribList, &config, 1, &numConfigs) )
	{
		return EGL_FALSE;
	}

	// Create a surface
	surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
	if ( surface == EGL_NO_SURFACE )
	{
		return EGL_FALSE;
	}

	// Create a GL context
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs );
	if ( context == EGL_NO_CONTEXT )
	{
		return EGL_FALSE;
	}   

	// Make the context current
	if ( !eglMakeCurrent(display, surface, surface, context) )
	{
		return EGL_FALSE;
	}

	printf("**********************CreateEGLContext success!\n");

	*eglDisplay = display;
	*eglSurface = surface;
	*eglContext = context;
	return EGL_TRUE;
} 

//////////////////////////////////////////////////////////////////
//
//  Public Functions
//
//

///
//  esInitContext()
//
//      Initialize ES utility context.  This must be called before calling any other
//      functions.
//
void ESUTIL_API esInitContext(ESContext *esContext)
{
	if (esContext != NULL)
	{
		memset(esContext, 0, sizeof( ESContext));
	}
}

///
//  esCreateWindow()
//
//      title - name for title bar of window
//      width - width of window to create
//      height - height of window to create
//      flags  - bitwise or of window creation flags 
//          ES_WINDOW_ALPHA       - specifies that the framebuffer should have alpha
//          ES_WINDOW_DEPTH       - specifies that a depth buffer should be created
//          ES_WINDOW_STENCIL     - specifies that a stencil buffer should be created
//          ES_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
//
GLboolean ESUTIL_API esCreateWindow( ESContext *esContext, const char* title,
				      GLint width, GLint height, GLuint flags )
{
	//EGLint attribList[] = {
	//    EGL_RED_SIZE,       5,
	//    EGL_GREEN_SIZE,     6,
	//    EGL_BLUE_SIZE,      5,
	//    EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
	//    EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
	//    EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
	//    EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
	//    EGL_NONE
	//};
	//
	EGLint attribList[] = {
		EGL_RED_SIZE,       8,
		EGL_GREEN_SIZE,     8,
		EGL_BLUE_SIZE,      8,
		EGL_ALPHA_SIZE,     0,
		EGL_DEPTH_SIZE,     16,
		EGL_STENCIL_SIZE,   0,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_LEVEL,	    0,
		EGL_NONE // always terminate the list with EGL_NONE
	};

	EGLint value = 0;

	if (esContext == NULL) {
		return GL_FALSE;
	}

	esContext->width = width;
	esContext->height = height;

	if (!WinCreate(esContext, title)) {
		return GL_FALSE;
	}
	printf("Window created ...\n");


	if (!CreateEGLContext(esContext->hWnd, esContext->nativeDisplay,
			      &esContext->eglDisplay, &esContext->oglContext,
			      &esContext->oglSurface, attribList) ) {
		return GL_FALSE;
	}
	printf("EGL context created ...\n");

	if(eglQuerySurface(esContext->eglDisplay, esContext->oglSurface,
			   EGL_WIDTH, &value)) {
		esContext->width = value;
		printf("********************EGL_WIDTH = %d!\n",value);
	}
	else {
		printf("EGL_WIDTH query failed!\n");
	}

	if(eglQuerySurface(esContext->eglDisplay, esContext->oglSurface,
			   EGL_HEIGHT, &value) ) {
		esContext->height = value;
		printf("********************EGL_HEIGHT = %d!\n",value);
	}
	else {
		printf("EGL_HEIGHT query failed!\n");
	}

	return GL_TRUE;
}

///
//  esMainLoop()
//
//    Start the main loop for the OpenGL ES application
//
void ESUTIL_API esMainLoop ( ESContext *esContext )
{
	WinLoop(esContext);
}


///
//  esRegisterDrawFunc()
//
void ESUTIL_API esRegisterDrawFunc( ESContext *esContext,
				    void (ESCALLBACK *drawFunc) (ESContext* ) )
{
	esContext->drawFunc = (void (*)(void*))drawFunc;
}


///
//  esRegisterUpdateFunc()
//
void ESUTIL_API esRegisterUpdateFunc( ESContext *esContext,
			void (ESCALLBACK *updateFunc) ( ESContext*, float ) )
{
	esContext->updateFunc = (void (*)(void*, float))updateFunc;
}


///
//  esRegisterKeyFunc()
//
void ESUTIL_API esRegisterKeyFunc ( ESContext *esContext,
			 void (ESCALLBACK *keyFunc) (ESContext*, unsigned char, int, int ) )
{
	esContext->keyFunc = (void (*)(void*, unsigned char, int, int))keyFunc;
}


///
// esLoadTGA()
//
//    Loads a 24-bit TGA image from a file
//
char* ESUTIL_API esLoadTGA ( char *fileName, int *width, int *height )
{
	char *buffer;

	if ( WinTGALoad ( fileName, &buffer, width, height ) )
	{
		return buffer;
	}

	return NULL;
}
