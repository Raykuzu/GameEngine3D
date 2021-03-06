#pragma once

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <iostream>
#include <map>
#include "../IWindow.hpp"

typedef std::map<unsigned int, WindowInput> unixInputMap;

class UnixWindow : public IWindow {
    public:
        UnixWindow() : IWindow() {
            _display = XOpenDisplay(nullptr);

            if (_display == nullptr)
                throw std::runtime_error("ERROR: Cannot connect to X server");
            ArcLogger::trace("TRACE: X server connected");
            _screen = DefaultScreen(_display);
        }

        ~UnixWindow() override {
            if (_windowOpened) {
                XDestroyWindow(_display, _window);
                ArcLogger::trace("TRACE: Window destroyed");
            }
            XCloseDisplay(_display);
            ArcLogger::trace("TRACE: X server disconnected");
        }

        WINDOW_INSTANCE getWindowInstance() override {
            return _display;
        }

        WINDOW_HANDLER getWindowHandler() override {
            return _window;
        }

        void closeWindow() override {
            if (_windowOpened) {
                XDestroyWindow(_display, _window);
                ArcLogger::trace("TRACE: Window destroyed");
            }
            _windowOpened = false;
        }

        void create(WindowSettings settings) override {
            const Window parentWindow = RootWindow(_display, _screen);
            unsigned long colors[] = { BlackPixel(_display, _screen), WhitePixel(_display, _screen) };

            _window = XCreateSimpleWindow(_display, parentWindow, settings.x, settings.y, settings.width, settings.height, settings.borderSize, colors[settings.borderColor], colors[settings.windowColor]);
            // XSelectInput(_display, _window, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
            XSelectInput(_display, _window, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);
            _deleteAtom = XInternAtom(_display, "WM_DELETE_WINDOW", False);
            XSetWMProtocols(_display, _window, &_deleteAtom, 1);

            ArcLogger::trace("TRACE: Window created");

            if (settings.fullscreen) {
                setFullScreen();
            } else {
                setWindowSizePos(settings.x, settings.y, settings.width, settings.height);
            }
            _windowOpened = true;
        }

        void setFullScreen() override {
            Atom wm_state   = XInternAtom (_display, "_NET_WM_STATE", true );
            Atom wm_fullscreen = XInternAtom (_display, "_NET_WM_STATE_FULLSCREEN", true );

            XChangeProperty(_display, _window, wm_state, XA_ATOM, 32, PropModeReplace, (unsigned char *)&wm_fullscreen, 1);

            XWindowAttributes attributes;

            XGetWindowAttributes(_display, _window, &attributes);
            Window root = attributes.root;
            XGetWindowAttributes(_display, root, &attributes);

            _xCoord = attributes.x;
            _yCoord = attributes.y;
            _width = attributes.width;
            _height = attributes.height;
            _screenCenterX = _width / 2;
            _screenCenterY = _height / 2;
            _fullScreen = true;
            ArcLogger::trace("TRACE: Window display mode -> fullscreen");
        }

        void setWindowSizePos(int x, int y, unsigned int width, unsigned int height) override {
            XMoveResizeWindow(_display, _window, x, y, width, height);
            _xCoord = x;
            _yCoord = y;
            _width = width;
            _height = height;
            _screenCenterX = _width / 2;
            _screenCenterY = _height / 2;
            _fullScreen = false;
            ArcLogger::trace("TRACE: Window display mode -> sized");
        }

        void resetPointerPos() override {
            XWarpPointer(_display, None, _window, 0, 0, 0, 0, _screenCenterX, _screenCenterY);
            XFlush(_display);
        }

        WindowEvent getEvent() override {
            const int eventTypes[] = { ClientMessage, KeyPress, KeyRelease/*, MotionNotify*/, ButtonPress, ButtonRelease };
            WindowEvent (UnixWindow::*eventHandlers[])() = {
                &UnixWindow::handleClientMessage,
                &UnixWindow::handleKeyPress,
                &UnixWindow::handleKeyRelease,
                //&UnixWindow::handleMotionNotify,
                &UnixWindow::handleButtonPress,
                &UnixWindow::handleButtonReleased,
                nullptr
            };
            WindowEvent unknownWindowEvent = { WE_UNKNOWN };

            if (!XPending(_display))
                return unknownWindowEvent;
            XNextEvent(_display, &_event);

            for (int i = 0; eventHandlers[i] != nullptr; i += 1) {
                if (eventTypes[i] == _event.type)
                    return (this->*eventHandlers[i])();
            }

            return unknownWindowEvent;
        }

        void expose() override {
            XMapWindow(_display, _window);
            ArcLogger::trace("TRACE: Window exposed");
        }

    private:
        WindowEvent handleClientMessage() {
            WindowEvent windowEvent;

            if (_event.xclient.data.l[0] == static_cast<long int>(_deleteAtom)) {
                windowEvent.type = WE_EXIT;
                closeWindow();
            } else {
                windowEvent.type = WE_UNKNOWN;
            }

            return windowEvent;
        }

        WindowEvent handleButtonPress() {
            WindowEvent windowEvent;

            windowEvent.type = WE_INPUT_PRESSED;
            unixInputMap::iterator it = _buttonMap.find(_event.xbutton.button);
            windowEvent.input = (it != _buttonMap.end()) ? it->second : WI_UNKNOWN;

            _currentlyPressedInput.push_back(windowEvent.input);

            return windowEvent;
        }

        WindowEvent handleButtonReleased() {
            WindowEvent windowEvent;

            windowEvent.type = WE_INPUT_RELEASED;
            unixInputMap::iterator it = _buttonMap.find(_event.xbutton.button);
            windowEvent.input = (it != _buttonMap.end()) ? it->second : WI_UNKNOWN;

            for (inputList::iterator iIt = _currentlyPressedInput.begin(); iIt != _currentlyPressedInput.end(); iIt++) {
                if (*iIt == windowEvent.input) {
                    _currentlyPressedInput.erase(iIt);
                    break;
                }
            }

            return windowEvent;
        }

        WindowEvent handleKeyPress() {
            WindowEvent windowEvent;

            windowEvent.type = WE_INPUT_PRESSED;
            unixInputMap::iterator it = _keyMap.find(_event.xkey.keycode);
            windowEvent.input = (it != _keyMap.end()) ? it->second : WI_UNKNOWN;

            _currentlyPressedInput.push_back(windowEvent.input);

            return windowEvent;
        }

        WindowEvent handleKeyRelease() {
            WindowEvent windowEvent;

            windowEvent.type = WE_INPUT_RELEASED;
            unixInputMap::iterator it = _keyMap.find(_event.xkey.keycode);
            windowEvent.input = (it != _keyMap.end()) ? it->second : WI_UNKNOWN;

            for (inputList::iterator iIt = _currentlyPressedInput.begin(); iIt != _currentlyPressedInput.end(); iIt++) {
                if (*iIt == windowEvent.input) {
                    _currentlyPressedInput.erase(iIt);
                    break;
                }
            }

            return windowEvent;
        }

        WindowEvent handleMotionNotify() {
            WindowEvent windowEvent;

            // if (_event.xmotion.x >= _screenCenterX - MOUSE_DEAD_ZONE && _event.xmotion.x <= _screenCenterX + MOUSE_DEAD_ZONE &&
            // _event.xmotion.y >= _screenCenterY - MOUSE_DEAD_ZONE && _event.xmotion.y <= _screenCenterY + MOUSE_DEAD_ZONE)
            //     return windowEvent;

            // std::cout << "EVENT XMOTION X: " << _event.xmotion.x << " | SCREEN CENTER X: " << _screenCenterX << std::endl;
            // std::cout << "EVENT XMOTION Y: " << _event.xmotion.y << " | SCREEN CENTER Y: " << _screenCenterY << std::endl;
            windowEvent.type = WE_POINTER_MOTION;
            windowEvent.x = _event.xmotion.x - _screenCenterX;
            windowEvent.y = _event.xmotion.y - _screenCenterY;

            // resetPointerPos();

            return windowEvent;
        }

        Display *_display;
        int _screen;
        Window _window;
        Atom _deleteAtom;
        XEvent _event;
        unixInputMap _buttonMap = {
            {1, WI_M1},
            {3, WI_M2},
            {2, WI_M3},
            {9, WI_M4},
            {8, WI_M5},
            {4, WI_SCROLL_UP},
            {5, WI_SCROLL_DOWN}
        };
        unixInputMap _keyMap = {
            {9, WI_ESCAPE},
            {67, WI_F1},
            {68, WI_F2},
            {69, WI_F3},
            {70, WI_F4},
            {71, WI_F5},
            {72, WI_F6},
            {73, WI_F7},
            {74, WI_F8},
            {75, WI_F9},
            {76, WI_F10},
            {95, WI_F11},
            {96, WI_F12},
            {110, WI_UP_LEFT_ARROW},
            {115, WI_FIN},
            {118, WI_INSER},
            {119, WI_SUPPR},
            {49, WI_SQUARE},
            {10, WI_1},
            {11, WI_2},
            {12, WI_3},
            {13, WI_4},
            {14, WI_5},
            {15, WI_6},
            {16, WI_7},
            {17, WI_8},
            {18, WI_9},
            {19, WI_0},
            {20, WI_PAR_CLOSE},
            {21, WI_EQUAL},
            {22, WI_BACKSPACE},
            {23, WI_TAB},
            {24, WI_A},
            {25, WI_Z},
            {26, WI_E},
            {27, WI_R},
            {28, WI_T},
            {29, WI_Y},
            {30, WI_U},
            {31, WI_I},
            {32, WI_O},
            {33, WI_P},
            {34, WI_CIRCUM},
            {35, WI_DOLLARS},
            {36, WI_RETURN},
            {66, WI_CAPS_LOCK},
            {38, WI_Q},
            {39, WI_S},
            {40, WI_D},
            {41, WI_F},
            {42, WI_G},
            {43, WI_H},
            {44, WI_J},
            {45, WI_K},
            {46, WI_L},
            {47, WI_M},
            {48, WI_PERCENT},
            {51, WI_ASTERISK},
            {50, WI_LEFT_SHIFT},
            {94, WI_CHEVRON},
            {52, WI_W},
            {53, WI_X},
            {54, WI_C},
            {55, WI_V},
            {56, WI_B},
            {57, WI_N},
            {58, WI_QUESTION_MARK},
            {59, WI_DOT},
            {60, WI_SLASH},
            {61, WI_EXCLAMATION},
            {62, WI_RIGHT_SHIFT},
            {152, WI_FN},
            {37, WI_LEFT_CTRL},
            {64, WI_ALT},
            {65, WI_SPACE},
            {108, WI_ALT_GR},
            {105, WI_RIGHT_CTRL},
            {112, WI_PAGE_UP},
            {117, WI_PAGE_DOWN},
            {113, WI_LEFT},
            {111, WI_UP},
            {114, WI_RIGHT},
            {116, WI_DOWN}
        };
};
