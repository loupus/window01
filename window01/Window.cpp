#include <iostream>
#include <sstream>
#include "cException.hpp"
#include "Window.hpp"

bool cWindow::IsClsRegistered = false;
int cWindow::RefCount = 0;


// ========================================================================


/*const char* cHrException::what() const noexcept
{
	std::ostringstream oss;
	oss << "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ")" << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}


HRESULT cHrException::GetErrorCode() const noexcept
{
	return hr;
}

std::string cHrException::GetErrorDescription() const noexcept
{
	return TranslateErrorCode(hr);
}*/

// ========================================================================




cWindow::cWindow(HINSTANCE inst, int _width, int _height) : hInst(inst), width(_width), height(_height)
{

}

cWindow::~cWindow()
{
	// tek bir pencerede mi?
	/*
	https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-destroywindow

	Destroys the specified window. The function sends WM_DESTROY and WM_NCDESTROY messages to the window to deactivate it and remove the keyboard focus from it. 
	The function also destroys the window's menu, destroys timers, removes clipboard ownership, and breaks the clipboard viewer chain (if the window is at the top of the viewer chain).
	If the specified window is a parent or owner window, DestroyWindow automatically destroys the associated child or owned windows when it destroys the parent or owner window. 
	The function first destroys child or owned windows, and then it destroys the parent or owner window.
	DestroyWindow also destroys modeless dialog boxes created by the CreateDialog function.
	*/
	BOOL back = DestroyWindow(m_hwnd);
	if (!back)
	{
		DWORD errCode = GetLastError();
		std::string ErrDesc = TranslateErrorCode(errCode);
		//throw cException(__LINE__, __FILE__, "DestroyWindow failed", errCode, ErrDesc.c_str());
		std::cout << __LINE__ << __FILE__ << "DestroyWindow failed" << " ErrCode:" << errCode << " ErrMsg:" << ErrDesc.c_str() << std::endl;
	}
	RefCount--;
	if (RefCount == 0)
	{
		UnregisterClass(wndClsName.c_str(), hInst);
		IsClsRegistered = false;
	}
		
}

void cWindow::Init()
{
	if (!IsClsRegistered)
		registerCls();

	RECT rc = { leftOffSet, topOffSet, width + leftOffSet, height + topOffSet }; // left, top, right, bottom
	
	
	 BOOL ret =   AdjustWindowRect(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, false);
	 if (!ret)
	 {
		 DWORD errCode = GetLastError();
		 std::string ErrDesc = TranslateErrorCode(errCode);
		 throw cException(__LINE__, __FILE__, "AdjustWindowRect failed", errCode, ErrDesc.c_str());
	 }

	 
	m_hwnd = CreateWindowEx
	(
		NULL,					// DWORD     dwExStyle,
		wndClsName.c_str(),		// LPCSTR    lpClassName,
		wndName.c_str(),		// LPCSTR    lpWindowName,
		WS_OVERLAPPEDWINDOW,	// DWORD     dwStyle,
		CW_USEDEFAULT,			//  X,
		CW_USEDEFAULT,			//  Y,
		rc.right - rc.left,		// width
		rc.bottom - rc.top,		// height
		nullptr,				// HWND      hWndParent,
		nullptr,				// HMENU     hMenu,
		hInst,					// HINSTANCE hInstance,
		(void*)this				// LPVOID    lpParam
	);

	if (!m_hwnd)
	{
		DWORD errCode = GetLastError();
		std::string ErrDesc = TranslateErrorCode(errCode);
		throw cException(__LINE__, __FILE__, "CreateWindowEx failed", errCode, ErrDesc.c_str());
	}
	RefCount++;

	ShowWindow((HWND)m_hwnd, SW_SHOW);

	/*https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-updatewindow*/
	UpdateWindow((HWND)m_hwnd);
}



LRESULT CALLBACK cWindow::MsgHandlerSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		cWindow* const pWnd = static_cast<cWindow*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&cWindow::MsgHandlerReDirect));
		// forward message to window instance handler
		return pWnd->WndProc(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK cWindow::MsgHandlerReDirect(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// retrieve ptr to window instance
	cWindow* const pWnd = reinterpret_cast<cWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->WndProc(hWnd, msg, wParam, lParam);
}

LRESULT cWindow::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
	
}


void* cWindow::getHandle()
{
	return m_hwnd;
}

void cWindow::setTitle(const std::string& title)
{

	BOOL back = SetWindowText(m_hwnd, title.c_str());
	if (!back)
	{
		DWORD errCode = GetLastError();
		std::string ErrDesc = TranslateErrorCode(errCode);
		throw cException(__LINE__, __FILE__, "SetWindowText failed", errCode, ErrDesc.c_str());
	}
}

void cWindow::registerCls()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = wndClsName.c_str();
	wc.lpfnWndProc = MsgHandlerSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = nullptr;
	wc.hCursor = nullptr;
	wc.lpszMenuName = nullptr;
	wc.hInstance = hInst;
	wc.style = CS_OWNDC;
	wc.hIcon = nullptr;
	wc.hIconSm = nullptr;

	WORD classId = RegisterClassEx(&wc);
	if (classId == 0)
	{
		DWORD errCode = GetLastError();
		std::string ErrDesc = TranslateErrorCode(errCode);
		throw cException(__LINE__, __FILE__, "RegisterClassEx failed", errCode, ErrDesc.c_str());
	}
	else
		IsClsRegistered = true;
}



cRefRect cWindow::getClientSize()
{
	RECT rc = {};
	::GetClientRect((HWND)this->m_hwnd, &rc);
	::ClientToScreen((HWND)this->m_hwnd, (LPPOINT)&rc.left);
	::ClientToScreen((HWND)this->m_hwnd, (LPPOINT)&rc.right);
	return cRefRect{ rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };
}

cRefRect cWindow::getScreenSize()
{
	RECT rc = {};
	rc.right = ::GetSystemMetrics(SM_CXSCREEN);
	rc.bottom = ::GetSystemMetrics(SM_CYSCREEN);
	return cRefRect{ rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top };
}

std::string cWindow::TranslateErrorCode(DWORD _errCode) 
{
	char* pMsgBuf = nullptr;
	// windows will allocate memory for err string and make our pointer point to it
	const DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		_errCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&pMsgBuf),
		0, 
		nullptr
	);
	// 0 string length returned indicates a failure
	if (nMsgLen == 0)
	{
		return "Unidentified error code";
	}
	// copy error string from windows-allocated buffer to std::string
	std::string errorString = pMsgBuf;
	// free windows buffer
	LocalFree(pMsgBuf);
	return errorString;
}

