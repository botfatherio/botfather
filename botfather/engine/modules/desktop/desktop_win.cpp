#include <Windows.h>

#include <QPixmap>
#include <QSize>
#include <QtWin>

#include "desktop.hpp"
#include "keymap.hpp"

class DesktopPrivate {
   public:
    QSize getDesktopSize() {
        HWND desktop_hwnd = GetDesktopWindow();
        RECT desktop_size;
        GetClientRect(desktop_hwnd, &desktop_size);
        return QSize(desktop_size.right, desktop_size.bottom);
    }

    void sendKeyboardEvent(WORD virtual_key_code, bool keyup) {
        // https://msdn.microsoft.com/de-de/library/windows/desktop/dd375731(v=vs.85).aspx
        INPUT input_event;
        input_event.type = INPUT_KEYBOARD;
        input_event.ki.wVk = virtual_key_code;
        input_event.ki.dwFlags = keyup ? KEYEVENTF_KEYUP : 0;
        SendInput(1, &input_event, sizeof(INPUT));
    }

    void sendMouseEvent(WORD flags) {
        // https://msdn.microsoft.com/de-de/library/windows/desktop/ms646273(v=vs.85).aspx
        INPUT input_event;
        input_event.type = INPUT_MOUSE;
        input_event.mi.dwFlags = flags;
        SendInput(1, &input_event, sizeof(INPUT));
    }
};

Desktop::Desktop(QObject *parent)
    : QObject(parent), pimpl(new DesktopPrivate) {}

Desktop::~Desktop() {}

QImage Desktop::takeScreenshot() {
    HWND hwnd = GetDesktopWindow();
    HDC hwindow_dc = GetDC(hwnd);
    HDC hwindow_compatible_dc = CreateCompatibleDC(hwindow_dc);
    SetStretchBltMode(hwindow_compatible_dc, COLORONCOLOR);

    RECT window_size;
    GetClientRect(hwnd, &window_size);

    int window_width = window_size.right;
    int window_height = window_size.bottom;

    // Create a bitmap
    HBITMAP window_hbitmap =
        CreateCompatibleBitmap(hwindow_dc, window_width, window_height);
    BITMAPINFOHEADER bitmap_info_header;
    bitmap_info_header.biSize = sizeof(bitmap_info_header);
    bitmap_info_header.biWidth = window_width;
    bitmap_info_header.biHeight =
        -window_height;  // online sources claim that having this pos draws the
                         // image upside down.
    bitmap_info_header.biPlanes = 1;
    bitmap_info_header.biBitCount = 32;
    bitmap_info_header.biCompression = BI_RGB;
    bitmap_info_header.biSizeImage = 0;
    bitmap_info_header.biXPelsPerMeter = 0;
    bitmap_info_header.biYPelsPerMeter = 0;
    bitmap_info_header.biClrUsed = 0;
    bitmap_info_header.biClrImportant = 0;

    SelectObject(hwindow_compatible_dc, window_hbitmap);
    StretchBlt(hwindow_compatible_dc, 0, 0, window_width, window_height,
               hwindow_dc, 0, 0, window_width, window_height, SRCCOPY);

    QImage image(QtWin::fromHBITMAP(window_hbitmap).toImage());
    image = image.convertToFormat(QImage::Format_RGB888);

    // Free objects to prevent memory leaks.
    DeleteObject(window_hbitmap);
    DeleteDC(hwindow_compatible_dc);
    DeleteDC(hwindow_dc);
    return image;
}

QSize Desktop::getSize() { return pimpl->getDesktopSize(); }

void Desktop::leftClick(const QPoint &position) {
    warpCursor(position);
    pimpl->sendMouseEvent(MOUSEEVENTF_LEFTDOWN);
    pimpl->sendMouseEvent(MOUSEEVENTF_LEFTUP);
}

void Desktop::middleClick(const QPoint &position) {
    warpCursor(position);
    pimpl->sendMouseEvent(MOUSEEVENTF_MIDDLEDOWN);
    pimpl->sendMouseEvent(MOUSEEVENTF_MIDDLEUP);
}

void Desktop::rightClick(const QPoint &position) {
    warpCursor(position);
    pimpl->sendMouseEvent(MOUSEEVENTF_RIGHTDOWN);
    pimpl->sendMouseEvent(MOUSEEVENTF_RIGHTUP);
}

void Desktop::pressMouse(Desktop::MouseButtons button) {
    holdMouse(button);
    releaseMouse(button);
}

void Desktop::holdMouse(Desktop::MouseButtons button) {
    switch (button) {
        case Desktop::MouseButtons::left:
            pimpl->sendMouseEvent(MOUSEEVENTF_LEFTDOWN);
            break;
        case Desktop::MouseButtons::middle:
            pimpl->sendMouseEvent(MOUSEEVENTF_MIDDLEDOWN);
            break;
        case Desktop::MouseButtons::right:
            pimpl->sendMouseEvent(MOUSEEVENTF_RIGHTDOWN);
            break;
    }
}

void Desktop::releaseMouse(Desktop::MouseButtons button) {
    switch (button) {
        case Desktop::MouseButtons::left:
            pimpl->sendMouseEvent(MOUSEEVENTF_LEFTUP);
            break;
        case Desktop::MouseButtons::middle:
            pimpl->sendMouseEvent(MOUSEEVENTF_MIDDLEUP);
            break;
        case Desktop::MouseButtons::right:
            pimpl->sendMouseEvent(MOUSEEVENTF_RIGHTUP);
            break;
    }
}

void Desktop::pressKey(const QString &key) {
    holdKey(key);
    releaseKey(key);
}

void Desktop::holdKey(const QString &key) {
    pimpl->sendKeyboardEvent(KEYMAP[key.toLower()], false);
}

void Desktop::releaseKey(const QString &key) {
    pimpl->sendKeyboardEvent(KEYMAP[key.toLower()], true);
}

void Desktop::warpCursor(const QPoint &position) {
    SetCursorPos(position.x(), position.y());
}

bool Desktop::getCursorPosition(int *x, int *y) {
    POINT cursor_position;
    if (GetCursorPos(&cursor_position)) {
        *x = static_cast<int>(cursor_position.x);
        *y = static_cast<int>(cursor_position.y);
        return true;
    }
    return false;
}

bool Desktop::keyExists(const QString &key) { return KEYMAP[key.toLower()]; }
