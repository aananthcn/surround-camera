#include "math.h"
#include "init.h"
#include "helper.h"


Mesh* cube = NULL;
Mesh* sphere = NULL;
Camera cam;

void LoadTextures(ESContext *esContext)
{
   UserData *userData = (UserData *)esContext->userData;

   TEXTURE* texture = (TEXTURE*) malloc(sizeof(TEXTURE));
   texture->textureId = LoadTGA(userData->textureName,
				&texture->width, &texture->height);
   userData->texture = texture;
}

void render_mesh(ESContext *esContext, Mesh* mesh, GLint shader_id)
{
	UserData *userData = (UserData *)esContext->userData;
	GLint positionLoc = -1, texCoordLoc = -1, loc = -1;
	GLint activeShaderProgram = -1;

	glUseProgram(shader_id);

	//vertex
	positionLoc = glGetAttribLocation ( shader_id, "a_position" );
	if(positionLoc != -1) {
		if(mesh->vertices == NULL) {
			//cout << "Data for \"a_position\" not found!"  << endl;
			printf("Data for \"a_position\" not found!");
			return;
		}
		glVertexAttribPointer(positionLoc, 3, GL_FLOAT,
				      GL_FALSE, 0, mesh->vertices);
		glEnableVertexAttribArray ( positionLoc );
	}


	//tex coord
	texCoordLoc = glGetAttribLocation(shader_id, "a_texCoord");
	if(texCoordLoc != -1) {
		if(mesh->texCoords == NULL) {
			//cout << "Data for \"a_texCoord\" not found!"  << endl;
			printf("Data for \"a_texCoord\" not found!");
			return;
		}
		glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT,
				      GL_FALSE, 0, mesh->texCoords);
		glEnableVertexAttribArray(texCoordLoc);
	}

        loc = glGetUniformLocation(shader_id, "mvpMatrix");
        if (loc != -1)
        {
                glUniformMatrix4fv(loc, 1, GL_FALSE, &cam.transformation.m[0][0]);
        }


	loc = glGetUniformLocation(shader_id, "s_texture");
	if (loc != -1) {
		glUniform1i(loc, 0);
		glActiveTexture ( GL_TEXTURE0 );
		glBindTexture ( GL_TEXTURE_2D, userData->texture->textureId);
	}

	{
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glCullFace(GL_BACK);
		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawElements(GL_TRIANGLES, mesh->nindices, GL_UNSIGNED_INT,
			       mesh->indices );
	}
}

void CreateScene()
{
	float angle = 90.0;

	//cube = esGenCube(0.4); commented by Aananth
	sphere = esGenSphere(32, 1);

	esMatrixLoadIdentity(&cam.transformation);
	esPerspective(&cam.transformation, 270.0, 1.0, 0.1, 10.0); // field of view in degrees, aspect ratio, near plane, far plane

	// Initially camera will be in z-axis (perpendicular to screen)
	// we are rotating it by 90 around x-axis to align in y-axis
	esRotate(&cam.transformation, angle, 1.0, 0.0, 0.0);
	esTranslate(&cam.transformation, 0.0, 0.1, 0.0);
}


void RenderScene(ESContext *esContext)
{
	UserData *userData = (UserData *)esContext->userData;
	static int x = 100;
	static float angle = 0.05;

	//x++;
	//if (x > VP_SIZE())
	//	x = 100;

	//glViewport(XRES/2-VP_SIZE()/2, 0, VP_SIZE(), VP_SIZE());
	glViewport(0, 0, XRES, YRES);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	esRotate(&cam.transformation, angle, 0.0, 1.0, 0.0);

	if(userData->texture != NULL)
	{
		glClearColor ( 0.0f, 0.0f, 0.3f, 1.0f );
		glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		render_mesh(esContext, sphere, userData->shaderDrawTexture);
	}
	eglSwapBuffers(esContext->eglDisplay, esContext->oglSurface);
}
