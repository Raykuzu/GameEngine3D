#pragma once

#include <windows.h>
#include <windowsx.h>
#include <iostream>
#include <map>
#include "IWindow.hpp"

#define DEFAULT_WIN_NAME L"My Window"

typedef std::map<WPARAM, WindowInput> winInputMap;

class WinWindow : public IWindow {
    public:
        WinWindow() : IWindow(), _instance(nullptr), _winHandler(nullptr) {
        }

        ~WinWindow() override {
            if (_windowOpened) {
                CloseWindow(_winHandler);
                PostQuitMessage(0);
            }
        }

        void closeWindow() override {
            CloseWindow(_winHandler);
            PostQuitMessage(0);
            _windowOpened = false;
        }

        void create(WindowSettings settings) override {
            const wchar_t className[] = L"Sample Window Class";
            DWORD colors[] = { RGB(0, 0, 0), RGB(255, 255, 255) };
            _instance = GetModuleHandle(NULL);

            WNDCLASS wc = { };
            wc.lpfnWndProc = s_WndProc;
            wc.hInstance = _instance;
            wc.lpszClassName = className;
            RegisterClass(&wc);

            _winHandler = CreateWindow(
                className,
                DEFAULT_WIN_NAME,
                WS_OVERLAPPEDWINDOW,
                settings.x,
                settings.y,
                settings.width,
                settings.height,
                NULL,
                NULL,
                _instance,
                this
            );

            if (_winHandler == nullptr) {
                fprintf(stderr, "ERROR : Display not opened");
                exit(84);
            }

            int aElements[1] = { COLOR_WINDOW };
            DWORD aNewColors[1];

            aNewColors[0] = colors[settings.windowColor];
            SetSysColors(1, aElements, aNewColors);

            if (_fullScreen) {
                setFullScreen();
            } else {
                MONITORINFO mi = { sizeof(mi) };
                GetMonitorInfo(MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY), &mi);

                setWindowSizePos((mi.rcMonitor.right - settings.width) / 2, (mi.rcMonitor.bottom - settings.height) / 2, settings.width, settings.height);
            }
            _windowOpened = true;
        }

        void setWindowSizePos(int x, int y, unsigned int width, unsigned int height) override {
            DWORD dwStyle = GetWindowLong(_winHandler, GWL_STYLE);
            SetWindowLong(_winHandler, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
            SetWindowPos(
                _winHandler,
                HWND_TOP,
                x,
                y,
                width,
                height,
                SWP_NOOWNERZORDER
            );
            _xCoord = x;
            _yCoord = y;
            _width = width;
            _height = height;
            _fullScreen = false;
        }

        void setFullScreen() override {
            MONITORINFO mi = { sizeof(mi) };
            GetMonitorInfo(MonitorFromWindow(NULL, MONITOR_DEFAULTTOPRIMARY), &mi);
            SetWindowLongPtr(_winHandler, GWL_EXSTYLE, WS_EX_APPWINDOW | WS_EX_TOPMOST);
            SetWindowLongPtr(_winHandler, GWL_STYLE, WS_POPUP | WS_VISIBLE);
            SetWindowPos(
                _winHandler,
                HWND_TOP,
                mi.rcMonitor.left,
                mi.rcMonitor.top,
                mi.rcMonitor.right - mi.rcMonitor.left,
                mi.rcMonitor.bottom - mi.rcMonitor.top,
                SWP_NOOWNERZORDER | SWP_FRAMECHANGED
            );
            _xCoord = mi.rcMonitor.left;
            _yCoord = mi.rcMonitor.top;
            _width = mi.rcMonitor.right - mi.rcMonitor.left;
            _height = mi.rcMonitor.bottom - mi.rcMonitor.top;
            _fullScreen = true;
        }

        WindowEvent getEvent() override {
            MSG msg = { };
            GetMessage(&msg, NULL, 0, 0);
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            while (!_isThereEvent);
            _isThereEvent = true;

            return _currentEvent;
        }

        void expose() override {
            ShowWindow(_winHandler, (_fullScreen ? SW_MAXIMIZE : SW_SHOWNORMAL ));
        }

    private:

        static LRESULT CALLBACK s_WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
            WinWindow* pThis;
            if (uMsg == WM_NCCREATE) {
                LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
                pThis = static_cast<WinWindow*>(lpcs->lpCreateParams);
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
            } else {
                pThis = reinterpret_cast<WinWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
            }

            if (pThis)
                return pThis->WndProc(hwnd, uMsg, wParam, lParam);
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

        LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            UINT eventTypes[] = {
                WM_DESTROY,
                WM_PAINT,
                WM_KEYDOWN,
                WM_KEYUP,
                WM_SYSKEYDOWN,
                WM_SYSKEYUP,
                WM_LBUTTONDOWN,
                WM_LBUTTONUP,
                WM_MBUTTONDOWN,
                WM_MBUTTONUP,
                WM_RBUTTONDOWN,
                WM_RBUTTONUP,
                WM_XBUTTONDOWN,
                WM_XBUTTONUP,
                WM_MOUSEWHEEL,
                WM_MOUSEMOVE,
            };
            void (WinWindow::*eventHandlers[])(HWND, UINT, WPARAM, LPARAM) = {
                &WinWindow::handleWindowDestroy,
                &WinWindow::handleWindowPaint,
                &WinWindow::handleKeyDown,
                &WinWindow::handleKeyUp,
                &WinWindow::handleKeyDown,
                &WinWindow::handleKeyUp,
                &WinWindow::handleLButtonDown,
                &WinWindow::handleLButtonUp,
                &WinWindow::handleMButtonDown,
                &WinWindow::handleMButtonUp,
                &WinWindow::handleRButtonDown,
                &WinWindow::handleRButtonUp,
                &WinWindow::handleXButtonDown,
                &WinWindow::handleXButtonUp,
                &WinWindow::handleMouseWheel,
                &WinWindow::handleMouseMove,
                nullptr
            };

            _currentEvent.type = WE_UNKNOWN;

            for (int i = 0; eventHandlers[i] != nullptr; i += 1) {
                if (eventTypes[i] == msg) {
                    (this->*eventHandlers[i])(hwnd, msg, wParam, lParam);
                    _isThereEvent = true;
                    return 0;
                }
            }
            _isThereEvent = true;
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }

        void handleWindowDestroy(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            closeWindow();
            _currentEvent.type = WE_EXIT;
        }

        void handleWindowPaint(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
            EndPaint(hwnd, &ps);
        }

        void handleKeyDown(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            winInputMap::iterator it = _keyMap.find(wParam);
            _currentEvent.input = (it != _keyMap.end()) ? it->second : WI_UNKNOWN;

            for (inputList::iterator iIt = _currentlyPressedInput.begin(); iIt != _currentlyPressedInput.end(); iIt++) {
                if (*iIt == _currentEvent.input)
                    return;
            }
            _currentEvent.type = WE_INPUT_PRESSED;
            _currentlyPressedInput.push_back(_currentEvent.input);
        }

        void handleKeyUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            _currentEvent.type = WE_INPUT_RELEASED;
            winInputMap::iterator it = _keyMap.find(wParam);
            _currentEvent.input = (it != _keyMap.end()) ? it->second : WI_UNKNOWN;

            for (inputList::iterator iIt = _currentlyPressedInput.begin(); iIt != _currentlyPressedInput.end(); iIt++) {
                if (*iIt == _currentEvent.input) {
                    _currentlyPressedInput.erase(iIt);
                    break;
                }
            }
        }

        void handleLButtonDown(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            _currentEvent.input = WI_M1;

            for (inputList::iterator iIt = _currentlyPressedInput.begin(); iIt != _currentlyPressedInput.end(); iIt++) {
                if (*iIt == _currentEvent.input)
                    return;
            }
            _currentEvent.type = WE_INPUT_PRESSED;
            _currentlyPressedInput.push_back(_currentEvent.input);
        }

        void handleLButtonUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            _currentEvent.type = WE_INPUT_RELEASED;
            _currentEvent.input = WI_M1;

            for (inputList::iterator iIt = _currentlyPressedInput.begin(); iIt != _currentlyPressedInput.end(); iIt++) {
                if (*iIt == _currentEvent.input) {
                    _currentlyPressedInput.erase(iIt);
                    break;
                }
            }
        }

        void handleMButtonDown(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            _currentEvent.input = WI_M3;

            for (inputList::iterator iIt = _currentlyPressedInput.begin(); iIt != _currentlyPressedInput.end(); iIt++) {
                if (*iIt == _currentEvent.input)
                    return;
            }
            _currentEvent.type = WE_INPUT_PRESSED;
            _currentlyPressedInput.push_back(_currentEvent.input);
        }

        void handleMButtonUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            _currentEvent.type = WE_INPUT_RELEASED;
            _currentEvent.input = WI_M3;

            for (inputList::iterator iIt = _currentlyPressedInput.begin(); iIt != _currentlyPressedInput.end(); iIt++) {
                if (*iIt == _currentEvent.input) {
                    _currentlyPressedInput.erase(iIt);
                    break;
                }
            }
        }

        void handleRButtonDown(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            _currentEvent.input = WI_M2;

            for (inputList::iterator iIt = _currentlyPressedInput.begin(); iIt != _currentlyPressedInput.end(); iIt++) {
                if (*iIt == _currentEvent.input)
                    return;
            }
            _currentEvent.type = WE_INPUT_PRESSED;
            _currentlyPressedInput.push_back(_currentEvent.input);
        }

        void handleRButtonUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            _currentEvent.type = WE_INPUT_RELEASED;
            _currentEvent.input = WI_M2;

            for (inputList::iterator iIt = _currentlyPressedInput.begin(); iIt != _currentlyPressedInput.end(); iIt++) {
                if (*iIt == _currentEvent.input) {
                    _currentlyPressedInput.erase(iIt);
                    break;
                }
            }
        }

        void handleXButtonDown(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            switch (wParam)
            {
            case 131136:
                _currentEvent.input = WI_M4;
                break;
            case 65568:
                _currentEvent.input = WI_M5;
                break;
            default:
                _currentEvent.input = WI_UNKNOWN;
                break;
            }

            for (inputList::iterator iIt = _currentlyPressedInput.begin(); iIt != _currentlyPressedInput.end(); iIt++) {
                if (*iIt == _currentEvent.input)
                    return;
            }
            _currentEvent.type = WE_INPUT_PRESSED;
            _currentlyPressedInput.push_back(_currentEvent.input);
        }

        void handleXButtonUp(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            _currentEvent.type = WE_INPUT_RELEASED;
            switch (wParam)
            {
            case 131072:
                _currentEvent.input = WI_M4;
                break;
            case 65536:
                _currentEvent.input = WI_M5;
                break;
            default:
                _currentEvent.input = WI_UNKNOWN;
                break;
            }

            for (inputList::iterator iIt = _currentlyPressedInput.begin(); iIt != _currentlyPressedInput.end(); iIt++) {
                if (*iIt == _currentEvent.input) {
                    _currentlyPressedInput.erase(iIt);
                    break;
                }
            }
        }

        void handleMouseWheel(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            _currentEvent.type = WE_INPUT_PRESSED;
            switch (wParam)
            {
            case 7864320:
                _currentEvent.input = WI_SCROLL_UP;
                break;
            case 4287102976:
                _currentEvent.input = WI_SCROLL_DOWN;
                break;
            default:
                _currentEvent.input = WI_UNKNOWN;
                break;
            }
        }

        void handleMouseMove(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
            _currentEvent.type = WE_POINTER_MOTION;
            _currentEvent.x = GET_X_LPARAM(lParam);
            _currentEvent.y = GET_Y_LPARAM(lParam);
        }

        HINSTANCE _instance;
        HWND _winHandler;
        bool _isThereEvent = false;
        WindowEvent _currentEvent;

        winInputMap _keyMap = {
            {27, WI_ESCAPE},
            {112, WI_F1},
            {113, WI_F2},
            {114, WI_F3},
            {115, WI_F4},
            {116, WI_F5},
            {117, WI_F6},
            {118, WI_F7},
            {119, WI_F8},
            {120, WI_F9},
            {121, WI_F10},
            {122, WI_F11},
            {96, WI_F12},
            {36, WI_UP_LEFT_ARROW},
            {35, WI_FIN},
            {45, WI_INSER},
            {46, WI_SUPPR},
            {222, WI_SQUARE},
            {49, WI_1},
            {50, WI_2},
            {51, WI_3},
            {52, WI_4},
            {53, WI_5},
            {54, WI_6},
            {55, WI_7},
            {56, WI_8},
            {57, WI_9},
            {48, WI_0},
            {219, WI_PAR_CLOSE},
            {187, WI_EQUAL},
            {8, WI_BACKSPACE},
            {9, WI_TAB},
            {65, WI_A},
            {90, WI_Z},
            {69, WI_E},
            {82, WI_R},
            {84, WI_T},
            {89, WI_Y},
            {85, WI_U},
            {73, WI_I},
            {79, WI_O},
            {80, WI_P},
            {221, WI_CIRCUM},
            {186, WI_DOLLARS},
            {13, WI_RETURN},
            {20, WI_CAPS_LOCK},
            {81, WI_Q},
            {83, WI_S},
            {68, WI_D},
            {70, WI_F},
            {71, WI_G},
            {72, WI_H},
            {74, WI_J},
            {75, WI_K},
            {76, WI_L},
            {77, WI_M},
            {192, WI_PERCENT},
            {220, WI_ASTERISK},
            {16, WI_LEFT_SHIFT},
            {226, WI_CHEVRON},
            {87, WI_W},
            {88, WI_X},
            {67, WI_C},
            {86, WI_V},
            {66, WI_B},
            {78, WI_N},
            {188, WI_QUESTION_MARK},
            {190, WI_DOT},
            {192, WI_SLASH},
            {223, WI_EXCLAMATION},
            {116, WI_RIGHT_SHIFT},
            {17, WI_LEFT_CTRL},
            {18, WI_ALT},
            {32, WI_SPACE},
            {18, WI_ALT_GR},
            {17, WI_RIGHT_CTRL},
            {33, WI_PAGE_UP},
            {34, WI_PAGE_DOWN},
            {37, WI_LEFT},
            {38, WI_UP},
            {39, WI_RIGHT},
            {40, WI_DOWN},
            {144, WI_PAD_VER_NUM},
            {111, WI_PAD_SLASH},
            {106, WI_PAD_ASTERISK},
            {109, WI_PAD_MINUS},
            {107, WI_PAD_PLUS},
            {13, WI_PAD_ENTR},
            {110, WI_PAD_SUPPR},
            {96, WI_PAD_0},
            {97, WI_PAD_1},
            {98, WI_PAD_2},
            {99, WI_PAD_3},
            {100, WI_PAD_4},
            {101, WI_PAD_5},
            {102, WI_PAD_6},
            {103, WI_PAD_7},
            {104, WI_PAD_8},
            {105, WI_PAD_9}
        };
};