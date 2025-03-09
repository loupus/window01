#include "Graphics.hpp"

#pragma comment (lib,"d3d11.lib")

cGraphics::cGraphics(HWND hWnd)
{
    //https://learn.microsoft.com/en-us/windows/win32/api/dxgi/ns-dxgi-dxgi_swap_chain_desc
    /*
       typedef struct DXGI_SWAP_CHAIN_DESC {
                      DXGI_MODE_DESC   BufferDesc;
                      DXGI_SAMPLE_DESC SampleDesc;
                      DXGI_USAGE       BufferUsage;
                      UINT             BufferCount;
                      HWND             OutputWindow;
                      BOOL             Windowed;
                      DXGI_SWAP_EFFECT SwapEffect;
                      UINT             Flags;
                    } DXGI_SWAP_CHAIN_DESC;
    */
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 0;
    sd.BufferDesc.RefreshRate.Denominator = 0;
    sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.BufferCount = 1;
    sd.OutputWindow = hWnd;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = 0;
}

cGraphics::~cGraphics()
{   
    if (pContext != nullptr) pContext->Release(); 
    if (pSwap != nullptr) pSwap->Release();
    if (pDevice != nullptr) pDevice->Release();
}

void cGraphics::Init()
{
	// https://learn.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdeviceandswapchain
	/*
	HRESULT D3D11CreateDeviceAndSwapChain(
  [in, optional]  IDXGIAdapter               *pAdapter,
                  D3D_DRIVER_TYPE            DriverType,
                  HMODULE                    Software,
                  UINT                       Flags,
  [in, optional]  const D3D_FEATURE_LEVEL    *pFeatureLevels,
                  UINT                       FeatureLevels,
                  UINT                       SDKVersion,
  [in, optional]  const DXGI_SWAP_CHAIN_DESC *pSwapChainDesc,
  [out, optional] IDXGISwapChain             **ppSwapChain,
  [out, optional] ID3D11Device               **ppDevice,
  [out, optional] D3D_FEATURE_LEVEL          *pFeatureLevel,
  [out, optional] ID3D11DeviceContext        **ppImmediateContext
);
	*/

    HRESULT hr = D3D11CreateDeviceAndSwapChain
    (
        nullptr
        , D3D_DRIVER_TYPE_HARDWARE
        , nullptr
        , 0
        , nullptr
        , 0
        , D3D11_SDK_VERSION
        , &sd
        , &pSwap
        , &pDevice
        , nullptr
        , &pContext
    );
}

void cGraphics::Present()
{
    HRESULT hr = pSwap->Present(1u, 0u);
}
