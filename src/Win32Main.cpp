// Copyright 2010 Lucas Teixeira
// This software is distribuited under the terms of the GNU Lesser Public License.
// See license.txt for more information.
// Dependencies: zlib and libpng.
//
// Auhtor: Lucas Teixeira
// Email: loteixeira at gmail dot com


#include "Config.h"
#include "app/App.h"
#include "app/ConfigFile.h"
#include "app/ExtensionManager.h"


//
// Win32 Main File
//
#ifdef WIN32_ENV


#define WINDOW_CLASS "WindChimeWindow"


ConfigFile* configFile;
HWND hWnd;
HDC hDC;
HGLRC hRC;
bool multisample;
int pixelformat;


bool createWindow(HINSTANCE hInstance);
bool startOpenGL();
void stopOpenGL();
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


//
// WinMain
//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	configFile = new ConfigFile(CONFIG_FILE_NAME);

	if (configFile->getMultisample() > 0)
		multisample = ExtensionManager::checkMultisample(&pixelformat, configFile->getWindowColorDepth(), configFile->getMultisample());
	else
		multisample = false;

	if (!createWindow(hInstance))
		return EXIT_FAILURE;

	if (!startOpenGL())
		return EXIT_FAILURE;

	App* app = new App(configFile);
	MSG msg;

	do
	{
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
        	TranslateMessage(&msg);
        	DispatchMessage(&msg);
        }
		else
		{
			SetWindowText(hWnd, app->getWindowCaption().c_str());
			app->run();
			SwapBuffers(hDC);
		}
	}
	while (msg.message != WM_QUIT);

	delete app;
	delete configFile;

	stopOpenGL();

	return EXIT_SUCCESS;
}


//
// CreateWindow
//
bool createWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, NULL);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS;
	wc.hIconSm = LoadIcon(hInstance, NULL);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Unable to register window class", "Error", MB_ICONERROR | MB_OK);
		return false;
	}

	hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, WINDOW_CLASS, "", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, configFile->getWindowWidth(), configFile->getWindowHeight(), NULL, NULL, hInstance, NULL);

	if (hWnd == NULL)
	{
		MessageBox(NULL, "Unable to create application window", "Error", MB_ICONERROR | MB_OK);
		return false;
	}

	if (configFile->getFullScreenWindow())
	{
	    DEVMODE screenSettings;
	    ZeroMemory(&screenSettings, sizeof(screenSettings));

	    screenSettings.dmSize = sizeof(screenSettings);
	    screenSettings.dmPelsWidth = configFile->getWindowWidth();
	    screenSettings.dmPelsHeight = configFile->getWindowHeight();
	    screenSettings.dmBitsPerPel = configFile->getWindowColorDepth();
	    screenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	    if (ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	    {
	    	char text[256];
	    	sprintf(text, "Unable to change desktop resolution to %ux%ux%u", configFile->getWindowWidth(), configFile->getWindowHeight(), configFile->getWindowColorDepth());

			MessageBox(NULL, text, "Error", MB_ICONERROR | MB_OK);
			return false;
	    }

		SetWindowLong(hWnd, GWL_EXSTYLE, (LONG) WS_EX_APPWINDOW);
		SetWindowLong(hWnd, GWL_STYLE, (LONG) WS_POPUP);

	    ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	}
	else
	{
	SetWindowLong(hWnd, GWL_STYLE, (LONG) WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = configFile->getWindowWidth();
		rect.bottom = configFile->getWindowHeight();

		AdjustWindowRectEx(&rect, GetWindowLong(hWnd, GWL_STYLE), FALSE, GetWindowLong(hWnd, GWL_EXSTYLE));
		SetWindowPos(hWnd, 0, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOMOVE);

		ShowWindow(hWnd, SW_SHOW);
	}

	return true;
}


//
// StartOpenGL
//
bool startOpenGL()
{
	PIXELFORMATDESCRIPTOR pfd;
    int iFormat;

    hDC = GetDC(hWnd);

	if (hDC == NULL)
	{
		MessageBox(NULL, "Unable to get device context", "Error", MB_ICONERROR | MB_OK);
		return false;
	}

    ZeroMemory(&pfd, sizeof(pfd));
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = configFile->getWindowColorDepth();
    pfd.cDepthBits = 16;
    pfd.iLayerType = PFD_MAIN_PLANE;

    if (multisample)
    {
    	iFormat = pixelformat;
    }
    else
    {
    	iFormat = ChoosePixelFormat(hDC, &pfd);

		if (iFormat == 0)
		{
			MessageBox(NULL, "Unable to choose pixel format", "Error", MB_ICONERROR | MB_OK);
			return false;
		}
    }

    if (SetPixelFormat(hDC, iFormat, &pfd) == FALSE)
	{
		MessageBox(NULL, "Unable to set pixel format", "Error", MB_ICONERROR | MB_OK);
		return false;
	}

    hRC = wglCreateContext(hDC);

	if (hRC == NULL)
	{
		MessageBox(NULL, "Unable to create opengl context", "Error", MB_ICONERROR | MB_OK);
		return false;
	}

    if (wglMakeCurrent(hDC, hRC) == FALSE)
	{
		MessageBox(NULL, "Unable to activate opengl context", "Error", MB_ICONERROR | MB_OK);
		return false;
	}

	return true;
}


//
// StopOpenGL
//
void stopOpenGL()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(hRC);
    ReleaseDC(hWnd, hDC);
}


//
// WndProc
//
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

    return 0;
}


//
// getTime
//
unsigned long getTime()
{
	return timeGetTime();
}

#endif /* WIN32_ENV */
