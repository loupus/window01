#include <iostream>
#include "cApplication.hpp"

int main()
{
	std::cout << "program begin" << std::endl;
	cApplication app;
	cWindow* mainWnd = app.NewWindow();
	app.Run();


	return 0;
}