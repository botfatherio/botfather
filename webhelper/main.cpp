#include <include/cef_app.h>
#include <include/cef_client.h>

#include <iostream>

#include "helper_app.hpp"

#ifdef _WIN64
#include <windows.h>

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      PWSTR lpCmdLine, int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);
    CefMainArgs main_args(hInstance);
#else

int main(int argc, char* argv[]) {
    CefMainArgs main_args(argc, argv);
#endif

    CefRefPtr<HelperApp> helper_app = new HelperApp;
    return CefExecuteProcess(main_args, helper_app.get(), nullptr);
}
