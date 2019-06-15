#include <iostream>
#include <include/cef_app.h>
#include <include/cef_client.h>

#ifdef _WIN64
#include <windows.h>

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);
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
