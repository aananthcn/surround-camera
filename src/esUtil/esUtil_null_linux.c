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
	esContext->eglDisplay = eglGetDisplay(0);
	if (esContext->eglDisplay == NULL)
	{
	  printf("%s","fbGetDisplay() failed.\n");
	  return GL_FALSE;
	}

	esContext->hWnd =	(NativeWindowType)00;

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
