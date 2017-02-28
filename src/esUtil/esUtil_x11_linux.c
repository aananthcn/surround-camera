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
#include <string.h>
#include <sys/time.h>

#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>

//#define NULL (0)
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
GLboolean WinCreate(ESContext *esContext, const char *title )
{
	Display    *x_display;
	Window      win, root;
	Atom  wm_state;
	XWMHints hints;

	int   one = 1;

	XSetWindowAttributes  swa;
	XSetWindowAttributes  xattr;

	//X11 stuffs added by Aananth
	x_display = XOpenDisplay(NULL);   // open the standard display (the primary screen)
	if (x_display == NULL) {
		printf("%s(): cannot connect to X server\n", __func__);
		return 1;
	}

	root = DefaultRootWindow(x_display);   // get the root window (usually the whole screen)

	swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;
	win = XCreateWindow( // create a window with the provided parameters
			     x_display, root,
			     0, 0, esContext->width, esContext->height,   0,
			     CopyFromParent, InputOutput,
			     CopyFromParent, CWEventMask,
			     &swa );


	xattr.override_redirect = False;
	XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

	hints.input = TRUE;
	hints.flags = InputHint;
	XSetWMHints(x_display, win, &hints);

	//make the window visible on the screen
	XMapWindow(x_display, win);
	XStoreName(x_display, win, title);

	// get identifiers for the provided atom name strings
	wm_state = XInternAtom(x_display, "_NET_WM_STATE", FALSE);

	XEvent xev;
	memset(&xev, 0, sizeof(xev));

	xev.type                 = ClientMessage;
	xev.xclient.window       = win;
	xev.xclient.message_type = wm_state;
	xev.xclient.format       = 32;
	xev.xclient.data.l[0]    = 1;
	xev.xclient.data.l[1]    = FALSE;
	XSendEvent(// send an event mask to the X-server
		   x_display,
		   DefaultRootWindow ( x_display ),
		   FALSE,
		   SubstructureNotifyMask,
		   &xev );

	//EGL stuffs sent by SankarGanesh
	esContext->eglDisplay = eglGetDisplay((EGLNativeDisplayType)x_display);
	if (esContext->eglDisplay == NULL)
	{
		printf("%s","eglGetDisplay(EGLNative..) failed.\n");
		return GL_FALSE;
	}

	esContext->hWnd =	(NativeWindowType)win;

	return GL_TRUE;
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
	float deltatime, time;

	gettimeofday(&t1 , &tz);
	time = 0;

	while(GL_TRUE)
	{
		gettimeofday(&t2, &tz);
		deltatime = (float)(t2.tv_sec - t1.tv_sec +
				    (t2.tv_usec - t1.tv_usec) * 1e-6);
		t1 = t2;

		if (esContext->updateFunc != NULL)
			esContext->updateFunc(esContext, deltatime);
		if (esContext->drawFunc != NULL)
			esContext->drawFunc(esContext);

		time += deltatime;
		if (time > 10.0)
			break;
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
