#include "desktop.h"
#include <windows.h>
#include <QSize>
#include "keymap.h"

class DesktopPrivate {
public:
    QSize getDesktopSize()
    {
        HWND desktop_hwnd = GetDesktopWindow();
        RECT desktop_size;
        GetClientRect(desktop_hwnd, &desktop_size);
        return QSize(desktop_size.right, desktop_size.bottom);
    }

    void sendKeyboardEvent(WORD virtual_key_code, bool keyup)
    {
        // https://msdn.microsoft.com/de-de/library/windows/desktop/dd375731(v=vs.85).aspx
        INPUT input_event;
        input_event.type = INPUT_KEYBOARD;
        input_event.ki.wVk = virtual_key_code;
        input_event.ki.dwFlags = keyup ? KEYEVENTF_KEYUP : 0;
        SendInput(1, &input_event, sizeof(INPUT));
    }

    void sendMouseEvent(WORD flags)
    {
        // https://msdn.microsoft.com/de-de/library/windows/desktop/ms646273(v=vs.85).aspx
        INPUT input_event;
        input_event.type = INPUT_MOUSE;
        input_event.mi.dwFlags = flags;
        SendInput(1, &input_event, sizeof(INPUT));
    }
};

Desktop::Desktop(QObject *parent) : QObject(parent), d_ptr(new DesktopPrivate)
{

}

Desktop::~Desktop()
{
	
}

#include <opencv2/highgui.hpp>

bool Desktop::takeScreenshot(cv::Mat &screenshot)
{
    HWND hwnd = GetDesktopWindow();
    HDC hwindow_dc = GetDC(hwnd);
    HDC hwindow_compatible_dc = CreateCompatibleDC(hwindow_dc);
    SetStretchBltMode(hwindow_compatible_dc, COLORONCOLOR);

    RECT window_size;
    GetClientRect(hwnd, &window_size);

    int window_width = window_size.right;
    int window_height = window_size.bottom;
    cv::Mat mat(window_height, window_width, CV_8UC4);

    // Create a bitmap
    HBITMAP window_hbitmap = CreateCompatibleBitmap(hwindow_dc, window_width, window_height);
    BITMAPINFOHEADER bitmap_info_header;
    bitmap_info_header.biSize = sizeof(bitmap_info_header);
    bitmap_info_header.biWidth = window_width;
    bitmap_info_header.biHeight = -window_height; // online sources claim that having this pos draws the image upside down.
    bitmap_info_header.biPlanes = 1;
    bitmap_info_header.biBitCount = 32;
    bitmap_info_header.biCompression = BI_RGB;
    bitmap_info_header.biSizeImage = 0;
    bitmap_info_header.biXPelsPerMeter = 0;
    bitmap_info_header.biYPelsPerMeter = 0;
    bitmap_info_header.biClrUsed = 0;
    bitmap_info_header.biClrImportant = 0;

    SelectObject(hwindow_compatible_dc, window_hbitmap);
    StretchBlt(hwindow_compatible_dc, 0, 0, window_width, window_height, hwindow_dc, 0, 0, window_width, window_height, SRCCOPY);
    GetDIBits(hwindow_compatible_dc, window_hbitmap, 0, window_height, mat.data, (BITMAPINFO *)&bitmap_info_header, DIB_RGB_COLORS);

    // Free objects to prevent memory leaks.
    DeleteObject(window_hbitmap);
    DeleteDC(hwindow_compatible_dc);
    DeleteDC(hwindow_dc);

    // Remove alpha channel
    cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);

    screenshot = mat;
    return true;
}

int Desktop::getWidth()
{
    return d_ptr->getDesktopSize().width();
}

int Desktop::getHeight()
{
    return d_ptr->getDesktopSize().height();
}

void Desktop::leftClick(int x, int y)
{
    warpCursor(x, y);
    d_ptr->sendMouseEvent(MOUSEEVENTF_LEFTDOWN);
    d_ptr->sendMouseEvent(MOUSEEVENTF_LEFTUP);
}

void Desktop::middleClick(int x, int y)
{
    warpCursor(x, y);
    d_ptr->sendMouseEvent(MOUSEEVENTF_MIDDLEDOWN);
    d_ptr->sendMouseEvent(MOUSEEVENTF_MIDDLEUP);
}

void Desktop::rightClick(int x, int y)
{
    warpCursor(x, y);
    d_ptr->sendMouseEvent(MOUSEEVENTF_RIGHTDOWN);
    d_ptr->sendMouseEvent(MOUSEEVENTF_RIGHTUP);
}

void Desktop::pressKey(QString key)
{
    holdKey(key);
    releaseKey(key);
}

void Desktop::holdKey(QString key)
{
    d_ptr->sendKeyboardEvent(KEYMAP[key.toLower()], false);
}

void Desktop::releaseKey(QString key)
{
    d_ptr->sendKeyboardEvent(KEYMAP[key.toLower()], true);
}

void Desktop::warpCursor(int x, int y)
{
    SetCursorPos(x, y);
}

bool Desktop::getCursorPosition(int *x, int *y)
{
	POINT cursor_position;
	if (GetCursorPos(&cursor_position)) {
        x = (int *)cursor_position.x;
        y = (int *)cursor_position.y;
		return true;
	}
	return false;
}

bool Desktop::keyExists(QString key)
{
    return KEYMAP[key.toLower()];
}
