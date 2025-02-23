#pragma once
#include <string>
#include <Windows.h>
#include "RefRect.hpp"
#include "cException.hpp"

//class cHrException : public cException
//{
//private:
//	
//	 HRESULT hr;
//
//public:
//	cHrException(int _line, const char* _file, HRESULT _hr)
//		:cException(_line,_file,""), hr(_hr)
//	{
//		
//	}
//	cHrException(int _line, const char* _file, const char* _msg, HRESULT _hr)
//		:cException(_line, _file, _msg), hr(_hr)
//	{
//
//	}
//	const char* what() const noexcept;
//	HRESULT GetErrorCode() const noexcept;
//	std::string GetErrorDescription() noexcept;
//};


class cWindow
{
private:
	const std::string wndClsName = "myWindowClass";
	const std::string wndName = "Hayrettin";
	static bool IsClsRegistered;
	int width = 0;
	int height = 0;
	int leftOffSet = 100;
	int topOffSet = 100;

protected:
	void* m_hwnd = nullptr;
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
	void setTitle(const char* title);
	void registerCls();
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	std::string TranslateErrorCode(DWORD _errCode);
	

	cRefRect getClientSize();
	cRefRect getScreenSize();
};

