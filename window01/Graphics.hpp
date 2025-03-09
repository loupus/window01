#pragma once
#include "hWindows.hpp"
#include <d3d11.h>

class cGraphics
{
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	DXGI_SWAP_CHAIN_DESC sd = {};

public:
	cGraphics(HWND hWnd);
	~cGraphics();
	cGraphics(const cGraphics&) = delete;
	cGraphics& operator=(const cGraphics&) = delete;
	void Init();
	void Present();
};

