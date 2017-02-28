#include "init.h"
#include "helper.h"
#include <stdbool.h>

int Init(ESContext *esContext)
{
	UserData *userData = (UserData *)esContext->userData;

	userData->shaderDrawTexture  = LoadShaderFromFile("./res/shaders/simple.vert.glsl", "./res/shaders/simple.frag.glsl");
	if (userData->shaderDrawTexture == -1) {
		return -1;
	}

	glClearColor ( 1.0f, 0.0f, 0.0f, 0.0f );

	// Set the viewport
	glViewport ( 0, 0, esContext->width, esContext->height );
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	LoadTextures(esContext);

	return true;
}


///
// Cleanup
//
void ShutDown ( ESContext *esContext )
{
   UserData *userData = (UserData *)esContext->userData;

   // Delete program object
   glDeleteProgram ( userData->shaderDrawTexture );
}
