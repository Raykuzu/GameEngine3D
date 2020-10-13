#pragma once

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <iostream>
#include <map>
#include "../IWindow.hpp"

typedef std::map<unsigned int, KeyboardKey> winKeyMap;

class UnixWindow : public IWindow {
    public:
        UnixWindow() : IWindow() {
            _display = XOpenDisplay(nullptr);

            if (_display == nullptr) {
                fprintf(stderr, "ERROR : Display not opened");
                exit(84);
            }
            _screen = DefaultScreen(_display);
        }

        ~UnixWindow() override {
            if (_windowOpened)
                XDestroyWindow(_display, _window);
            XCloseDisplay(_display);
        }

        void closeWindow() override {
            if (_windowOpened)
                XDestroyWindow(_display, _window);
            _windowOpened = false;
        }

        void create(WindowSettings settings) override {
            settings.fullscreen ? createFullScreenWindow(settings) : createSizedWindow(settings);
        }

        WindowEvent getEvent() override {
            XNextEvent(_display, &_event);
            const int eventTypes[] = { ClientMessage, KeyPress, KeyRelease, MotionNotify };
            WindowEvent (UnixWindow::*eventHandlers[])() = {
                &UnixWindow::handleClientMessage,
                &UnixWindow::handleKeyPress,
                &UnixWindow::handleKeyRelease,
                &UnixWindow::handleMotionNotify,
                nullptr
            };

            for (int i = 0; eventHandlers[i] != nullptr; i += 1) {
                if (eventTypes[i] == _event.type)
                    return (this->*eventHandlers[i])();
            }

            WindowEvent unknownWindowEvent = { WE_UNKNOWN };

            return unknownWindowEvent;
        }

        void expose() override {
            XMapWindow(_display, _window);
        }

    private:

        void createFullScreenWindow(WindowSettings settings) {
            const Window parentWindow = RootWindow(_display, _screen);
            unsigned long colors[] = { BlackPixel(_display, _screen), WhitePixel(_display, _screen) };

            _window = XCreateSimpleWindow(_display, parentWindow, 0, 0, 1, 1, settings.borderSize, colors[settings.borderColor], colors[settings.windowColor]);
            XSelectInput(_display, _window, ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask);
            _deleteAtom = XInternAtom(_display, "WM_DELETE_WINDOW", False);
            XSetWMProtocols(_display, _window, &_deleteAtom, 1);


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
            _windowOpened = true;
        }

        void createSizedWindow(WindowSettings settings) {
            const Window parentWindow = RootWindow(_display, _screen);
            unsigned long colors[] = { BlackPixel(_display, _screen), WhitePixel(_display, _screen) };

            _window = XCreateSimpleWindow(_display, parentWindow, settings.x, settings.y, settings.width, settings.height, settings.borderSize, colors[settings.borderColor], colors[settings.windowColor]);
            XSelectInput(_display, _window, ExposureMask | KeyPressMask | KeyReleaseMask | PointerMotionMask);
            _deleteAtom = XInternAtom(_display, "WM_DELETE_WINDOW", False);
            XSetWMProtocols(_display, _window, &_deleteAtom, 1);

            _xCoord = settings.x;
            _yCoord = settings.y;
            _width = settings.width;
            _height = settings.height;
            _windowOpened = true;
        }

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

        WindowEvent handleKeyPress() {
            WindowEvent windowEvent;

            windowEvent.type = WE_KEY_PRESSED;
            winKeyMap::iterator it = _keyMap.find(_event.xkey.keycode);
            windowEvent.key = (it != _keyMap.end()) ? it->second : KK_UNDEFINED;

            return windowEvent;
        }

        WindowEvent handleKeyRelease() {
            WindowEvent windowEvent;

            windowEvent.type = WE_KEY_RELEASED;
            winKeyMap::iterator it = _keyMap.find(_event.xkey.keycode);
            windowEvent.key = (it != _keyMap.end()) ? it->second : KK_UNDEFINED;

            return windowEvent;
        }

        WindowEvent handleMotionNotify() {
            WindowEvent windowEvent;

            windowEvent.type = WE_POINTER_MOTION;
            windowEvent.x = _event.xmotion.x;
            windowEvent.y = _event.xmotion.y;

            return windowEvent;
        }

        Display *_display;
        int _screen;
        Window _window;
        bool _windowOpened = false;
        Atom _deleteAtom;
        XEvent _event;
        winKeyMap _keyMap = {
            {9, KK_ESCAPE},
            {67, KK_F1},
            {68, KK_F2},
            {69, KK_F3},
            {70, KK_F4},
            {71, KK_F5},
            {72, KK_F6},
            {73, KK_F7},
            {74, KK_F8},
            {75, KK_F9},
            {76, KK_F10},
            {95, KK_F11},
            {96, KK_F12},
            {110, KK_UP_LEFT_ARROW},
            {115, KK_FIN},
            {118, KK_INSER},
            {119, KK_SUPPR},
            {49, KK_SQUARE},
            {10, KK_1},
            {11, KK_2},
            {12, KK_3},
            {13, KK_4},
            {14, KK_5},
            {15, KK_6},
            {16, KK_7},
            {17, KK_8},
            {18, KK_9},
            {19, KK_0},
            {20, KK_PAR_CLOSE},
            {21, KK_EQUAL},
            {22, KK_BACKSPACE},
            {23, KK_TAB},
            {24, KK_A},
            {25, KK_Z},
            {26, KK_E},
            {27, KK_R},
            {28, KK_T},
            {29, KK_Y},
            {30, KK_U},
            {31, KK_I},
            {32, KK_O},
            {33, KK_P},
            {34, KK_CIRCUM},
            {35, KK_DOLLARS},
            {36, KK_RETURN},
            {66, KK_CAPS_LOCK},
            {38, KK_Q},
            {39, KK_S},
            {40, KK_D},
            {41, KK_F},
            {42, KK_G},
            {43, KK_H},
            {44, KK_J},
            {45, KK_K},
            {46, KK_L},
            {47, KK_M},
            {48, KK_PERCENT},
            {51, KK_ASTERISK},
            {50, KK_LEFT_SHIFT},
            {94, KK_CHEVRON},
            {52, KK_W},
            {53, KK_X},
            {54, KK_C},
            {55, KK_V},
            {56, KK_B},
            {57, KK_N},
            {58, KK_QUESTION_MARK},
            {59, KK_DOT},
            {60, KK_SLASH},
            {61, KK_EXCLAMATION},
            {62, KK_RIGHT_SHIFT},
            {152, KK_FN},
            {37, KK_LEFT_CTRL},
            {64, KK_ALT},
            {65, KK_SPACE},
            {108, KK_ALT_GR},
            {105, KK_RIGHT_CTRL},
            {112, KK_PAGE_UP},
            {117, KK_PAGE_DOWN},
            {113, KK_LEFT},
            {111, KK_UP},
            {114, KK_RIGHT},
            {116, KK_DOWN}
        };
};
