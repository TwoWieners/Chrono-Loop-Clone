#include "stdafx.h"
#include "Rendering\SystemInitializer.h"
#include "Rendering\renderer.h"
#include "Rendering\InputLayoutManager.h"
#include "VRInputManager.h"
#include <openvr.h>
#include <iostream>
#include <ctime>
#include <chrono>
#include "Messager\Messager.h"

#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h> 



HWND hwnd;
LPCTSTR WndClassName = L"ChronoWindow";
HINSTANCE hInst;
Messager msger = Messager::Instance();

bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed);
std::chrono::steady_clock::time_point lastTime = std::chrono::steady_clock::now();
static float timeFrame = 0.0f;
static float deltaTime;
TimeManager* TManager; 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
template<class... Args>
void Update();
void UpdateTime();

int APIENTRY wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	if (!InitializeWindow(hInstance, nCmdShow, 800, 600, true)) {
		MessageBox(NULL, L"Kablamo.", L"The window broked.", MB_ICONERROR | MB_OK);
	}


	// Initialize Rendering systems and VR
	vr::HmdError pError;
	vr::IVRSystem *vrsys = vr::VR_Init(&pError, vr::VRApplication_Scene);
	if (pError != vr::HmdError::VRInitError_None) {
		std::cout << "Could not initialize OpenVR for reasons!" << std::endl;
	}

	//vrsys = nullptr;
	
	if (vrsys)
	{
		VRInputManager::Instance();
	}
	if (!RenderEngine::InitializeSystems(hwnd, 1512, 1680, false, 90, false, 1000, 0.1f, vrsys)) {
		return 1;
	} 
	TManager = TimeManager::Instance();
	
	// Update everything
	Update();

	//delete PlsGitRidOfThis;
	// Cleanup
	RenderEngine::ShutdownSystems();

#if _DEBUG || CONSOLE_OVERRIDE
	FreeConsole();
#endif

	return 0;
}


bool InitializeWindow(HINSTANCE hInstance, int ShowWnd, int width, int height, bool windowed) {
#if _DEBUG || CONSOLE_OVERRIDE
	if (AllocConsole()) {
		FILE* pCout;
		freopen_s(&pCout, "CONOUT$", "w", stdout);
		SetConsoleTitle(L"Chrono::Loop - Debug Console");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	}
#endif

	WNDCLASSEX wc;

	HINSTANCE hDll;
	hDll = LoadLibrary(L"SHELL32.dll");

	wc.cbSize = sizeof(WNDCLASSEX);    //Size of our windows class
	wc.style = (CS_HREDRAW | CS_VREDRAW);    //class styles
	wc.lpfnWndProc = WndProc;    //Default windows procedure function
	wc.cbClsExtra = NULL;    //Extra bytes after our wc structure
	wc.cbWndExtra = NULL;    //Extra bytes after our windows instance
	wc.hInstance = hInstance;    //Instance to current application
	wc.hIcon = LoadIcon(hDll, MAKEINTRESOURCE(512));    //Title bar Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);    //Default mouse Icon
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);    //Window bg color
	wc.lpszMenuName = NULL;    //Name of the menu attached to our window
	wc.lpszClassName = WndClassName;    //Name of our windows class
	wc.hIconSm = LoadIcon(hDll, MAKEINTRESOURCE(512)); //Icon in your taskbar

	if (!RegisterClassEx(&wc))    //Register our windows class
	{
		//if registration failed, display error
		DWORD code = GetLastError();
		MessageBox(NULL, L"Error registering class", L"Error", MB_OK | MB_ICONERROR);
		return false;
	}

	hwnd = CreateWindowEx(                                     //Create our Extended Window
												NULL,                                //Extended style
												WndClassName,                        //Name of our windows class
												L"Chrono::Loop",                     //Name in the title bar of our window
												WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, //style of our window
												600, 150,                            //Top left corner of window
												width,                               //Width of our window
												height,                              //Height of our window
												NULL,                                //Handle to parent window
												NULL,                                //Handle to a Menu
												hInstance,                           //Specifies instance of current program
												NULL                                 //used for an MDI client window
	);
	if (!hwnd) {
		MessageBox(NULL, L"Error creating window", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	ShowWindow(hwnd, ShowWnd);
	UpdateWindow(hwnd);

	return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

template<class... Args>
void Update() {
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	Message<> * _msg = new Message<>(messageTypes::SoundEngine, soundMessages::INITAILIZE_Audio, 0, false);
	msger.SendInMessage((void*)_msg);
	Message<Listener*, const char*> *_msg1 = new Message<Listener*, const char*>(messageTypes::SoundEngine, soundMessages::ADD_Listener, 0, false, new Listener(), nullptr);
	msger.SendInMessage((void*)_msg1);

	while (true) {

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// Handle windows message.
			if (msg.message == WM_QUIT) {
				
				break;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		} else {
			if (GetAsyncKeyState(VK_ESCAPE)) {
				break;
			}

			UpdateTime();
			// Input.Update(float deltaTime);
			VRInputManager::Instance().update();
			// Logic.Update(float deltaTime);
			TManager->Instance()->Update(deltaTime);
			RenderEngine::Renderer::Instance()->Render();
		}
	}
}
void UpdateTime()
{
	deltaTime = (float)(std::chrono::steady_clock::now().time_since_epoch().count() - lastTime.time_since_epoch().count()) / 1000.0f / 1000.0f / 1000.0f;
	lastTime = std::chrono::steady_clock::now();
}



