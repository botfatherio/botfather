#include <iostream>
#include "include/cef_app.h"
#include "include/cef_client.h"

#if (defined (_WIN32) || defined (_WIN64))
#include <Windows.h>
#endif

#if defined (_WIN32)

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	CefMainArgs main_args(hInstance);
	return CefExecuteProcess(main_args, CefRefPtr<CefApp>(), nullptr);
}

#elif defined (_WIN64)

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	CefMainArgs main_args(hInstance);
	return CefExecuteProcess(main_args, CefRefPtr<CefApp>(), nullptr);
}

#else

int main(int argc, char* argv[])
{
	CefMainArgs main_args(argc, argv);
	return CefExecuteProcess(main_args, CefRefPtr<CefApp>(), nullptr);
}

#endif