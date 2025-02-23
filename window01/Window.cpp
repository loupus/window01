#include <sstream>
#include "cException.hpp"
#include "Window.hpp"

bool cWindow::IsClsRegistered = false;


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

LRESULT CALLBACK cWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}	
}



cWindow::cWindow(HINSTANCE inst, int _width, int _height) : hInst(inst), width(_width), height(_height)
{

}

cWindow::~cWindow()
{
	// tek bir pencerede mi?
	UnregisterClass(wndClsName.c_str(), hInst);
}

void cWindow::Init()
{
	if (!IsClsRegistered)
		registerCls();

	RECT rc = { leftOffSet, topOffSet, width + leftOffSet, height + topOffSet }; // left, top, right, bottom
	
	 BOOL ret =   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
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
		nullptr,				//  HWND      hWndParent,
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


	SetWindowLongPtr((HWND)m_hwnd, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow((HWND)m_hwnd, SW_SHOW);
	UpdateWindow((HWND)m_hwnd);
}



void* cWindow::getHandle()
{
	return m_hwnd;
}

void cWindow::setTitle(const char* title)
{
	::SetWindowText((HWND)m_hwnd, title);
}

void cWindow::registerCls()
{
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = wndClsName.c_str();
	wc.lpfnWndProc = &WndProc;
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

