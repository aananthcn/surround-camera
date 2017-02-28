//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// esUtil_win32.c
//
//    This file contains the Win32 implementation of the windowing functions. 


///
// Includes
//
#include "esUtil.h"
#include <stdio.h>
#include <sys/time.h>
#include <screen/screen.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/neutrino.h>

#define NULL (0)
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

///
//  WinCreate()
//
//      Create window
//
GLboolean WinCreate ( ESContext *esContext, const char *title )
{/*
	esContext->eglDisplay = fbGetDisplayByIndex(0);
	if (esContext->eglDisplay == NULL)
	{
	  printf("%s","fbGetDisplay() failed.\n");
	  return GL_FALSE;
	}

	esContext->hWnd =	fbCreateWindow(esContext->eglDisplay, 0, 0, 1920, 720);
	if (esContext->hWnd == NULL)
	{
	  printf("%s", "fbCreateWindow() failed.\n");
	  return GL_FALSE;
	}
*/

	screen_context_t screen_ctx;         /* connection to screen windowing system */
	screen_window_t screen_win;          /* native handle for our window */
	screen_event_t screen_ev;            /* handle used to pop events from our queue */
	EGLDisplay egl_disp;                 /* abstract display on which graphics are drawn */
	EGLConfig egl_conf;                  /* describes the color and ancillary buffers */
	EGLSurface egl_surf;                 /* refers to our window's rendering surface */
	EGLContext egl_ctx;                  /* a handle to a rendering context */
	int usage = SCREEN_USAGE_OPENGL_ES2; /* we will use OpenGL ES 2.X to do our rendering */
	int size[2] = { -1, -1 };            /* width and height of our window */
	int pos[2] = { 0, 0 };               /* x,y position of our window */
	int nbuffers = 2;                    /* number of buffers backing the window */
	int format = SCREEN_FORMAT_RGBA8888; /* native visual type / screen format */
	int val;                             /* a generic variable used to set/get window properties */
	EGLint interval = 1;                 /* EGL swap interval */
	int verbose = EGL_FALSE;             /* EGL_TRUE if the verbose option was set */
	int vis = 1;                         /* boolean that indicates if our window is visible */
	int pause = 0;                       /* EGL_TRUE if rendering is frozen */
	const char *conf_str = NULL;         /* configuration string */
	const char *tok;                     /* used to process command-line arguments */
	int rc;                              /* return value from functions */

	printf("**********************WinCreate enter!\n");
	
	rc = screen_create_context(&screen_ctx, 0);
	if (rc) {
		perror("screen_context_create");
		goto fail3;
	}

	rc = screen_create_window(&screen_win, screen_ctx);
	if (rc) {
		perror("screen_create_window");
		goto fail4;
	}

	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_FORMAT, &format);
	if (rc) {
		perror("screen_set_window_property_iv(SCREEN_PROPERTY_FORMAT)");
		goto fail5;
	}

	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &usage);
	if (rc) {
		perror("screen_set_window_property_iv(SCREEN_PROPERTY_USAGE)");
		goto fail5;
	}

	rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SWAP_INTERVAL, &interval);
	if (rc) {
		perror("screen_set_window_property_iv(SCREEN_PROPERTY_SWAP_INTERVAL)");
		goto fail5;
	}

	if (size[0] > 0 && size[1] > 0) {
		rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SIZE, size);
		if (rc) {
			perror("screen_set_window_property_iv(SCREEN_PROPERTY_SIZE)");
			goto fail5;
		}
	} else {
		rc = screen_get_window_property_iv(screen_win, SCREEN_PROPERTY_SIZE, size);
		if (rc) {
			perror("screen_get_window_property_iv(SCREEN_PROPERTY_SIZE)");
			goto fail5;
		}
	}

	if (pos[0] != 0 || pos[1] != 0) {
		rc = screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_POSITION, pos);
		if (rc) {
			perror("screen_set_window_property_iv(SCREEN_PROPERTY_POSITION)");
			goto fail5;
		}
	}

	rc = screen_create_window_buffers(screen_win, nbuffers);
	if (rc) {
		perror("screen_create_window_buffers");
		goto fail5;
	}

	rc = screen_create_event(&screen_ev);
	if (rc) {
		perror("screen_create_event");
		goto fail5;
	}

	printf("**********************WinCreate finish!\n");
	
	esContext->eglDisplay = EGL_DEFAULT_DISPLAY;
	
	esContext->hWnd =	screen_win;
	if (esContext->hWnd == NULL)
	{
	  printf("%s", "QNX Screen window is null.\n");
	  return GL_FALSE;
	}	
	
	printf("**********************WinCreate success!\n");
	
   return GL_TRUE;
   
fail5:
	screen_destroy_window(screen_win);
fail4:
	screen_destroy_context(screen_ctx);   
fail3:
	
	printf("**********************WinCreate failed!\n");
	
	return GL_FALSE;	
}

///
//  winLoop()
//
//      Start main windows loop
//
void WinLoop ( ESContext *esContext )
{
    struct timeval t1, t2;
    struct timezone tz;
    float deltatime;

    gettimeofday ( &t1 , &tz );

    while(GL_TRUE)
    {
        gettimeofday(&t2, &tz);
        deltatime = (float)(t2.tv_sec - t1.tv_sec + (t2.tv_usec - t1.tv_usec) * 1e-6);
        t1 = t2;

        if (esContext->updateFunc != NULL)
            esContext->updateFunc(esContext, deltatime);
        if (esContext->drawFunc != NULL)
            esContext->drawFunc(esContext);
    }
}

unsigned int ESUTIL_API esGetCurrentMillisecond()
{
    struct timeval t1, t2;

    gettimeofday ( &t1 , NULL );
	
	unsigned long time = 0;
	time = t1.tv_sec * 1000; // secons to milliseconds
	time += (t1.tv_usec / 1000); // microseconds to milliseconds
	return time;
}