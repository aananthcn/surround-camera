//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

// ESShapes.c
//
//    Utility functions for generating shapes
//

///
//  Includes
//
#include "esUtil.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

///
// Defines
//
#define ES_PI  (3.14159265f)

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
/// \brief Generates geometry for a sphere.  Allocates memory for the vertex data and stores 
///        the results in the arrays.  Generate index list for a TRIANGLE_STRIP
/// \param numSlices The number of slices in the sphere
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLE_STRIP
//

Mesh* ESUTIL_API esGenSphere(int numSlices, float radius)
{
	int i;
	int j;
	int numParallels = numSlices / 2;
	int numVertices = ( numParallels + 1 ) * ( numSlices + 1 );
	int numIndices = numParallels * numSlices * 6;
	float angleStep = (2.0f * ES_PI) / ((float) numSlices);
	Mesh* mesh = (Mesh*)calloc(1, sizeof(Mesh));

	// Allocate memory for buffers
	mesh->nvertices = numVertices;
	mesh->vertices = (GLfloat*)malloc(sizeof(GLfloat) * 3 * numVertices);
	mesh->normals = (GLfloat*)malloc(sizeof(GLfloat) * 3 * numVertices);
	mesh->texCoords = (GLfloat*)malloc( sizeof(GLfloat) * 2 * numVertices);

	mesh->nindices = numIndices;
	mesh->indices =(GLuint*)malloc(sizeof(GLuint) * numIndices);

	for ( i = 0; i < numParallels + 1; i++ ) {
		for ( j = 0; j < numSlices + 1; j++ ) {
			int vertex = (i * (numSlices + 1) + j) * 3;

			if (mesh->vertices) {
				(mesh->vertices)[vertex + 0] = radius *
					sinf(angleStep * (float)i) *
					sinf(angleStep * (float)j);

				(mesh->vertices)[vertex + 1] = radius *
					cosf(angleStep * (float)i);

				(mesh->vertices)[vertex + 2] = radius *
					sinf(angleStep * (float)i) *
					cosf(angleStep * (float)j);
			}

			if (mesh->normals){
				(mesh->normals)[vertex + 0] =
					(mesh->vertices)[vertex + 0] / radius;
				(mesh->normals)[vertex + 1] =
					(mesh->vertices)[vertex + 1] / radius;
				(mesh->normals)[vertex + 2] =
					(mesh->vertices)[vertex + 2] / radius;
			}

			if (mesh->texCoords) {
				int texIndex = (i * (numSlices + 1) + j) * 2;
				(mesh->texCoords)[texIndex + 1] =
					(1.0f - (float)i) /
					(float)(numParallels - 1 );
				(mesh->texCoords)[texIndex + 0] = (float)j / (float)numSlices;
				i = i;
			}
		}
	}

	// Generate the indices
	if (mesh->indices != NULL) {
		GLuint *indexBuf = (mesh->indices);
		for ( i = 0; i < numParallels ; i++ ) {
			for ( j = 0; j < numSlices; j++ ) {
				*indexBuf++  = i * (numSlices + 1) + j;
				*indexBuf++ = (i+1) * (numSlices + 1) + j;
				*indexBuf++ = (i+1) * (numSlices + 1) + (j+1);

				*indexBuf++ = i * ( numSlices + 1 ) + j;
				*indexBuf++ = (i+1) * (numSlices + 1) + (j+1);
				*indexBuf++ = i * (numSlices + 1) + (j+1);
			}
		}
	}

	return mesh;
}


//
/// \brief Generates geometry for a cube.  Allocates memory for the vertex data and stores 
///        the results in the arrays.  Generate index list for a TRIANGLES
/// \param scale The size of the cube, use 1.0 for a unit cube.
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLE_STRIP
//
Mesh* ESUTIL_API esGenCube ( float scale)
{
	int i;
	int numVertices = 24;
	int numIndices = 36;

	Mesh* mesh = (Mesh*)calloc(1, sizeof(Mesh));

	GLfloat cubeVerts[] =
	{
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,
		-0.5f,  0.5f, 0.5f,
		0.5f,  0.5f, 0.5f, 
		0.5f, -0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		0.5f,  0.5f, -0.5f,
	};

	GLfloat cubeNormals[] =
	{
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
	};

	GLfloat cubeTex[] =
	{
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	mesh->nvertices = numVertices;

	mesh->nindices = numIndices;

	// Allocate memory for buffers   
	{
		mesh->vertices = (GLfloat*) malloc ( sizeof(GLfloat) * 3 * numVertices );
		memcpy( mesh->vertices, cubeVerts, sizeof( cubeVerts ) );
		for ( i = 0; i < numVertices; i++ )
		{
			(mesh->vertices)[i] *= scale;
		}
	}


	{
		mesh->normals = (GLfloat*)malloc ( sizeof(GLfloat) * 3 * numVertices );
		memcpy( mesh->normals, cubeNormals, sizeof( cubeNormals ) );
	}

	{
		mesh->texCoords = (GLfloat*)malloc ( sizeof(GLfloat) * 2 * numVertices );
		memcpy( mesh->texCoords, cubeTex, sizeof( cubeTex ) ) ;
	}


	// Generate the indices
	{
		GLuint cubeIndices[] =
		{
			0, 2, 1,
			0, 3, 2, 
			4, 5, 6,
			4, 6, 7,
			8, 9, 10,
			8, 10, 11, 
			12, 15, 14,
			12, 14, 13, 
			16, 17, 18,
			16, 18, 19, 
			20, 23, 22,
			20, 22, 21
		};

		mesh->indices = (GLuint*)malloc ( sizeof(GLuint) * numIndices );
		memcpy( mesh->indices, cubeIndices, sizeof( cubeIndices ) );
	}

	return mesh;
}
