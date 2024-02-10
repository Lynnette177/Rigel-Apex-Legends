#pragma once
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <memory>
#include <string_view>
#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <ctime>
#include <random>
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3d9types.h>
#include <Windows.h>
#include <iostream>
#include <filesystem>
#include <winhttp.h>
#include <fstream>
#include <string>
#include <direct.h>
#include <TlHelp32.h>
#include <vector>
#include <process.h>
#include <thread>
#include <bitset>
#include <mutex>
#include <array>
#include <dwmapi.h>

/* D3D SDK */
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3d9types.h>

/* Overlay & Imgui */
#include "Fucking ImGui/imgui.h"
#include "Fucking ImGui/imgui_impl_dx11.h"
#include "Fucking ImGui/imgui_impl_win32.h"
#include "Fucking ImGui/imgui_internal.h""

/* String Encrypted */
#include "skStr.h"

/* Offset & Address */

/* Config */
#include "Config.hpp"

/* Render */
#include "Vector.hpp"
#include "Renderer.hpp"

/* Lib */
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "winhttp.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dwmapi.lib")


#define M_PI        3.14159265358979323846f
#define M_RADPI        57.295779513082f
#define M_PI_F        ((float)(M_PI))
#define RAD2DEG( x  )  ( (float)(x) * (float)(180.f / M_PI_F) )
#define DEG2RAD( x  )  ( (float)(x) * (float)(M_PI_F / 180.f) )
#define ANGLE2SHORT(x) ((int)((x)*65536/360) & 65535)
#define SHORT2ANGLE(x)    ((x)*(360.0/65536))

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

inline void CleanupRenderTarget()
{
	if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = NULL; }
}

inline void CleanupDeviceD3D()
{
	CleanupRenderTarget();
	if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = NULL; }
	if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = NULL; }
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
}

inline void CreateRenderTarget()
{
	ID3D11Texture2D* pBackBuffer;
	g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	g_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &g_mainRenderTargetView);
	pBackBuffer->Release();
}

inline bool CreateDeviceD3D(HWND hWnd)
{
	// Setup swap chain
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT createDeviceFlags = 0;
	//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	if (D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext) != S_OK)
		return false;

	CreateRenderTarget();
	return true;
}

// Win32 message handler
inline LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		if (g_pd3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			CleanupRenderTarget();
			g_pSwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
			CreateRenderTarget();
		}
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}
	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}
inline float MeterToGameUnits(float Meters) {
	return 39.37007874 * Meters;
}
inline float GameUnitsToMeter(float GameUnits) {
	return  GameUnits/39.37007874;
}
namespace id {
	int head = 8;
	int neck = 7;
	int body = 5;
	int hips = 0;
}
const char* keys = ("X2");
const char* key_type[] = { ("L"), ("R"),  ("M"),  ("X1"),  ("X2") };
const char* keys2 = ("L");
const char* key_type2[] = { ("L"), ("R"),  ("M"),  ("X1"),  ("X2") };
const char* bones = ("Body");
const char* bones_type[] = { ("Head"), ("Neck"),  ("Body"),  ("Hips")};