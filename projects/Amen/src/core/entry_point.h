#pragma once
#include <core/app.h>

#ifdef AMEN_ENTRYPOINT

/**
* The entry point for Windows and Linux.
*
*/
int main(int argc, char **argv)
{
	Hazel::App* app = Hazel::CreateApp();

	app->Run();

	delete app;

	return 0;
}
#endif