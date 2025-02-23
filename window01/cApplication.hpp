#pragma once
#include "Window.hpp"

class cApplication
{
private:
	HINSTANCE hInst = nullptr;

public:
	cApplication();
	~cApplication();
	int Run();
	cWindow* NewWindow();
};

