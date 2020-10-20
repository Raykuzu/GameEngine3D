#pragma once

#include <vector>

#define DEFAULT_WINDOW_WIDTH   1200
#define DEFAULT_WINDOW_HEIGHT  800
#define DEFAULT_WINDOW_X 0
#define DEFAULT_WINDOW_Y 0

enum WindowInput {
    WI_UNKNOWN = -1,
    WI_ESCAPE,
    WI_F1,
    WI_F2,
    WI_F3,
    WI_F4,
    WI_F5,
    WI_F6,
    WI_F7,
    WI_F8,
    WI_F9,
    WI_F10,
    WI_F11,
    WI_F12,
    WI_UP_LEFT_ARROW,
    WI_FIN,
    WI_INSER,
    WI_SUPPR,
    WI_SQUARE,
    WI_1,
    WI_2,
    WI_3,
    WI_4,
    WI_5,
    WI_6,
    WI_7,
    WI_8,
    WI_9,
    WI_0,
    WI_PAR_CLOSE,
    WI_EQUAL,
    WI_DELETE,
    WI_BACKSPACE,
    WI_TAB,
    WI_A,
    WI_Z,
    WI_E,
    WI_R,
    WI_T,
    WI_Y,
    WI_U,
    WI_I,
    WI_O,
    WI_P,
    WI_CIRCUM,
    WI_DOLLARS,
    WI_RETURN,
    WI_CAPS_LOCK,
    WI_Q,
    WI_S,
    WI_D,
    WI_F,
    WI_G,
    WI_H,
    WI_J,
    WI_K,
    WI_L,
    WI_M,
    WI_PERCENT,
    WI_ASTERISK,
    WI_LEFT_SHIFT,
    WI_CHEVRON,
    WI_W,
    WI_X,
    WI_C,
    WI_V,
    WI_B,
    WI_N,
    WI_QUESTION_MARK,
    WI_DOT,
    WI_SLASH,
    WI_EXCLAMATION,
    WI_RIGHT_SHIFT,
    WI_FN,
    WI_LEFT_CTRL,
    WI_ALT,
    WI_SPACE,
    WI_ALT_GR,
    WI_RIGHT_CTRL,
    WI_PAGE_UP,
    WI_PAGE_DOWN,
    WI_LEFT,
    WI_UP,
    WI_RIGHT,
    WI_DOWN,
    WI_PAD_VER_NUM,
    WI_PAD_SLASH,
    WI_PAD_ASTERISK,
    WI_PAD_MINUS,
    WI_PAD_PLUS,
    WI_PAD_ENTR,
    WI_PAD_SUPPR,
    WI_PAD_0,
    WI_PAD_1,
    WI_PAD_2,
    WI_PAD_3,
    WI_PAD_4,
    WI_PAD_5,
    WI_PAD_6,
    WI_PAD_7,
    WI_PAD_8,
    WI_PAD_9,
    WI_M1,
    WI_M2,
    WI_M3,
    WI_M4,
    WI_M5,
    WI_SCROLL_UP,
    WI_SCROLL_DOWN
};

enum BorderColor {
    B_BLACK,
    B_WHITE
};

enum WindowColor {
    W_BLACK,
    W_WHITE
};

enum WindowEventType {
    WE_EXIT,
    WE_INPUT_PRESSED,
    WE_INPUT_RELEASED,
    WE_POINTER_MOTION,
    WE_UNKNOWN,
};

struct WindowEvent {
    WindowEventType type = WE_UNKNOWN;
    WindowInput input = WI_UNKNOWN;
    int x = 0;
    int y = 0;
};

struct WindowSettings {
    int x = 0;
    int y = 0;
    std::string title = "window";
    unsigned int width = DEFAULT_WINDOW_WIDTH;
    unsigned int height = DEFAULT_WINDOW_HEIGHT;
    bool fullscreen = false;
    unsigned int borderSize = 1;
    BorderColor borderColor = B_BLACK;
    WindowColor windowColor = W_WHITE;
};

typedef std::vector<WindowInput> inputList;

class IWindow {
    public:
        IWindow() = default;
        virtual ~IWindow() = default;

        virtual void create(WindowSettings settings) = 0;
        virtual void expose() = 0;
        virtual WindowEvent getEvent() = 0;
        virtual void closeWindow() = 0;
        bool isOpened() const {
            return _windowOpened;
        };
        inputList getCurrentlyPressedInput() const {
            return _currentlyPressedInput;
        };

    protected:
        int _xCoord = DEFAULT_WINDOW_X;
        int _yCoord = DEFAULT_WINDOW_Y;
        unsigned int _width = DEFAULT_WINDOW_WIDTH;
        unsigned int _height = DEFAULT_WINDOW_HEIGHT;
        bool _windowOpened = false;
        bool _fullScreen = false;
        inputList _currentlyPressedInput;
};
