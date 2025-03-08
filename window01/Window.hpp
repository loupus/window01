#pragma once
#define WIN32_LEAN_AND_MEAN // todo windows header hazırla
#include <string>
#include <Windows.h>
#include "RefRect.hpp"
#include "cException.hpp"



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

protected:
	HWND  m_hwnd = nullptr;
	HINSTANCE hInst = nullptr;

public:
	cWindow(HINSTANCE ins, int _width, int _height);
	~cWindow();

	// delete copy
	cWindow(const cWindow&) = delete;
	cWindow& operator=(const cWindow&) = delete;


	virtual void onCreate() {}
	virtual void onUpdate() {}
	virtual void onDestroy() {}
	virtual void onSize(const  cRefRect& size) {}

	void Init();
	void* getHandle();
	void setTitle(const std::string& title);
	void registerCls();
	LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MsgHandlerSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK MsgHandlerReDirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	std::string TranslateErrorCode(DWORD _errCode);
	

	cRefRect getClientSize();
	cRefRect getScreenSize();
};

