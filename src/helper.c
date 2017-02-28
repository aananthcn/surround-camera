#include "helper.h"


#ifdef QNX
#include <sys/stat.h>
#include <sys/types.h>
#endif

GLuint LoadTGA (char *fileName, int* width, int* height)
{
#ifdef QNX
	struct stat statbuf;
	paddr_t freemem;
#endif

#ifdef QNX
	stat( "/proc", &statbuf ); 
	freemem = (paddr_t)statbuf.st_size; 
	printf( "Current Free system memory is %d MB\n", (freemem / ( 1024 * 1024 ))); 
#endif
	printf("About to read TGA File into system memory. Press Enter to continue..\n");
	//cin.get();

	char *buffer = esLoadTGA ( fileName, width, height );
	GLuint texId;
	int a;

	if ( buffer == NULL )
	{
		printf ( "Error loading (%s) image.\n", fileName );
		return 0;
	}

#ifdef QNX
	stat( "/proc", &statbuf ); 
	freemem = (paddr_t)statbuf.st_size; 
	printf( "Current Free system memory is %d MB\n", (freemem / ( 1024 * 1024 ))); 
#endif  
	printf("TGA Image loaded to system memory. About to call glTexImage2D(). Press Enter to continue..\n");
	//cin.get();

	glGenTextures ( 1, &texId );
	glBindTexture ( GL_TEXTURE_2D, texId );

	glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, *width, *height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

#ifdef QNX
	stat( "/proc", &statbuf ); 
	freemem = (paddr_t)statbuf.st_size; 
	printf( "Current Free system memory is %d MB\n", (freemem / ( 1024 * 1024 ))); 
#endif   
	printf("glTexImage2D() called. About to free the TGA image in system memory. Press Enter to continue..\n");
	//cin.get();

	free ( buffer );

#ifdef QNX
	stat( "/proc", &statbuf ); 
	freemem = (paddr_t)statbuf.st_size; 
	printf( "Current Free system memory is %d MB\n", (freemem / ( 1024 * 1024 ))); 
#endif   
	printf("TGA Image loaded to system memory has been released. About to show the texture. Press Enter to continue..\n");
	//cin.get();

	return texId;

}


unsigned long getFileLength(const char *file)
{
	int size;
	FILE *fp;

	fp = fopen(file, "r");
	if(fp == NULL) {
		printf("Can't open \"%s\"\n", file);
		return -1;
	}

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fclose(fp);

	return size;
}

int loadshader(char* filename, char** ShaderSource, unsigned long* len)
{
	void *ptr; //FIXME: possible memory leak as this is a local pointer
	unsigned int i=0;
	FILE *fp;

	/* check the input image file size */
	*len = getFileLength(filename);
	if (*len == 0) {
		printf("Shader %s is empty file\n", filename);
		return -2;   // Error: Empty File 
	}
	printf("%s(): Shader size = %d\n", __func__, (int)*len);
	fp = fopen(filename, "rb");

	/* allocate shader memory */
	ptr = malloc(*len+1);
	if (ptr == NULL) {
		printf("unable to reserve memory for Shader %s \n", filename);
		return -3;   // can't reserve memory
	}
	*ShaderSource = (char*) ptr;

	// len isn't always strlen cause some characters are stripped in ascii read...
	// it is important to 0-terminate the real length later, len is just max possible value... 
	(*ShaderSource)[*len] = 0;


	for (i = 0; i < *len; i++) {
		fread((*ShaderSource)+i, 1, 1, fp);       // get character from file.
		if (-1 == feof(fp)) {
			printf("%s(): unexpected EOF reached!\n", __func__);
			break;
		}
	}

	//(*ShaderSource)[i] = '\0';  // 0-terminate it at the correct position
	*((*ShaderSource)+i-1) = '\0';  // 0-terminate it at the correct position

	fclose(fp);
	*len = i;

	return 0; // No Error
}


int unloadshader(GLubyte** ShaderSource)
{
	if (*ShaderSource != 0)
		free(*ShaderSource);
	*ShaderSource = 0;

	return 0;
}


GLuint LoadShaderFromFile(char* vertexShaderFile, char* fragShdaderFile)
{
	char *vertexShaderSrc, *fragShaderSrc;
	unsigned long vertexShaderLen, fragShaderLen;
	int rc;

	vertexShaderSrc = NULL;
	rc = loadshader(vertexShaderFile, &vertexShaderSrc, &vertexShaderLen);
	if ((rc != 0) || (vertexShaderSrc == NULL)) {
		printf("%s(): loadshader(vertex) failed!\n", __func__);
		return -1;
	}

	fragShaderSrc = NULL;
	rc = loadshader(fragShdaderFile, &fragShaderSrc, &fragShaderLen);
	if ((rc != 0) || (fragShaderSrc == NULL)) {
		printf("%s(): loadshader(fragment) failed!\n", __func__);
		return -1;
	}

	GLuint ret = esLoadProgram(vertexShaderSrc, fragShaderSrc);
	if (ret < 0) {
		printf("%s(): esLoadProgram() failed!!\n", __func__);
		printf("%s(): return code: %d\n", __func__, ret);
		return -1;
	}

	return ret;
}
