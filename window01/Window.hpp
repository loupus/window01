#pragma once
#include "hWindows.hpp"
#include <string>
#include <memory>
#include "RefRect.hpp"
#include "cException.hpp"
#include "Graphics.hpp"



class cWindow
{

private:
	
	const std::string wndName = "Hayrettin";
	const std::string wndClsName = "myWindowClass";
	static bool IsClsRegistered;
	static int RefCount;
	int width = 0;
	int height = 0;
	int leftOffSet = 100;
	int topOffSet = 100;
	std::unique_ptr<cGraphics> pGfx = nullptr;

protected:
	HWND  m_hwnd = nullptr;
	HINSTANCE hInst = nullptr;

public:
	cWindow(HINSTANCE ins, int _width, int _height);
	~cWindow();

	// delete copy
	cWindow(const cWindow&) = delete;
	cWindow& operator=(const cWindow&) = delete;


	void OnSize(int width, int height);


	void Init();
	void* getHandle();
	void setTitle(const std::string& title);
	void registerCls();
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MsgHandlerSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MsgHandlerReDirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	std::string TranslateErrorCode(DWORD _errCode);
	cGraphics& Gfx();

	cRefRect getClientSize();
	cRefRect getScreenSize();
};

