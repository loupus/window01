#include "cApplication.hpp"

cApplication::cApplication()
{
	hInst = GetModuleHandle(NULL);
}

cApplication::~cApplication()
{
	hInst = nullptr;
}

int cApplication::Run()
{
	MSG msg;   
	ZeroMemory(&msg, sizeof(MSG));    

	while (true)   
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)    
				break;    

			TranslateMessage(&msg);   
			DispatchMessage(&msg);
		}

		else 
		{   
			//todo do your stuff
		}


	}

	return (int)msg.wParam;        
}

cWindow* cApplication::NewWindow()
{
	cWindow* nw = new cWindow(hInst,720,576);
	nw->Init();
	return nw;
}
