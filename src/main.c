#include "init.h"
#include "helper.h"

void MainInit();


int main(int argc, char *argv[])
{
	ESContext esContext;
	UserData  userData;

	esInitContext(&esContext);
	userData.textureName = "./res/ExportedFont.tga";
	esContext.userData = &userData;

	esCreateWindow(&esContext, "Surround Camera",
		       XRES, YRES, ES_WINDOW_RGB );

	if(-1 == Init(&esContext)) {
		printf("Init() failed!!\n");
		return -1;
	}

	CreateScene();

	esRegisterDrawFunc(&esContext, RenderScene);

	esMainLoop(&esContext);

	ShutDown(&esContext);

	return 0;
}
