#ifndef __INIT_H__
#define __INIT_H__


#include <stdlib.h>
#include <stdio.h>
#include "esUtil.h"

#define DELAY_MAX		1

typedef struct
{
	GLint textureId;
	int width;
	int height;
} TEXTURE;

typedef struct
{
	// Handle to a program object
	char* textureName;
	GLuint shaderDrawTexture;
	TEXTURE*  texture;
} UserData;


int Init ( ESContext *esContext );
void LoadTextures(ESContext *esContext);

void draw(TEXTURE* texture, GLint shader_id);

void UpdateScreenRect(ESContext *esContext);
void CreateScene();
void RenderScene ( ESContext *esContext );
void ShutDown ( ESContext *esContext );

#endif