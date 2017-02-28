#ifndef __HELPER_H__
#define __HELPER_H__


#include <stdlib.h>
#include <stdio.h>
#include "esUtil.h"

#include <GL/glu.h>


#define XRES 800
#define YRES 480

#define MIN(a,b) ((a) < (b) ? (a) : (b))

#define VP_SIZE() MIN(XRES, YRES)

GLuint LoadTGA (char *fileName, int* width, int* height);

GLuint LoadShaderFromFile(char* vertexShaderFile, char* fragShdaderFile);

#endif
