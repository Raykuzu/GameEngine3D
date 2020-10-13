#pragma once

#define DEFAULT_WINDOW_WIDTH   1200
#define DEFAULT_WINDOW_HEIGHT  800
#define DEFAULT_WINDOW_X 0
#define DEFAULT_WINDOW_Y 0

enum KeyboardKey {
    KK_UNDEFINED = -1,
    KK_ESCAPE,
    KK_F1,
    KK_F2,
    KK_F3,
    KK_F4,
    KK_F5,
    KK_F6,
    KK_F7,
    KK_F8,
    KK_F9,
    KK_F10,
    KK_F11,
    KK_F12,
    KK_UP_LEFT_ARROW,
    KK_FIN,
    KK_INSER,
    KK_SUPPR,
    KK_SQUARE,
    KK_1,
    KK_2,
    KK_3,
    KK_4,
    KK_5,
    KK_6,
    KK_7,
    KK_8,
    KK_9,
    KK_0,
    KK_PAR_CLOSE,
    KK_EQUAL,
    KK_DELETE,
    KK_BACKSPACE,
    KK_TAB,
    KK_A,
    KK_Z,
    KK_E,
    KK_R,
    KK_T,
    KK_Y,
    KK_U,
    KK_I,
    KK_O,
    KK_P,
    KK_CIRCUM,
    KK_DOLLARS,
    KK_RETURN,
    KK_CAPS_LOCK,
    KK_Q,
    KK_S,
    KK_D,
    KK_F,
    KK_G,
    KK_H,
    KK_J,
    KK_K,
    KK_L,
    KK_M,
    KK_PERCENT,
    KK_ASTERISK,
    KK_LEFT_SHIFT,
    KK_CHEVRON,
    KK_W,
    KK_X,
    KK_C,
    KK_V,
    KK_B,
    KK_N,
    KK_QUESTION_MARK,
    KK_DOT,
    KK_SLASH,
    KK_EXCLAMATION,
    KK_RIGHT_SHIFT,
    KK_FN,
    KK_LEFT_CTRL,
    KK_ALT,
    KK_SPACE,
    KK_ALT_GR,
    KK_RIGHT_CTRL,
    KK_PAGE_UP,
    KK_PAGE_DOWN,
    KK_LEFT,
    KK_UP,
    KK_RIGHT,
    KK_DOWN,
    KK_PAD_SLASH,
    KK_PAD_ASTERISK,
    KK_PAD_MINUS,
    KK_PAD_PLUS,
    KK_PAD_ENTR,
    KK_PAD_SUPPR,
    KK_PAD_0,
    KK_PAD_1,
    KK_PAD_2,
    KK_PAD_3,
    KK_PAD_4,
    KK_PAD_5,
    KK_PAD_6,
    KK_PAD_7,
    KK_PAD_8,
    KK_PAD_9
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
    WE_KEY_PRESSED,
    WE_KEY_RELEASED,
    WE_POINTER_MOTION,
    WE_UNKNOWN,
};

struct WindowEvent {
    WindowEventType type = WE_UNKNOWN;
    KeyboardKey key = KK_UNDEFINED;
    int x = 0;
    int y = 0;
};

struct WindowSettings {
    int x = 0;
    int y = 0;
    unsigned int width = DEFAULT_WINDOW_WIDTH;
    unsigned int height = DEFAULT_WINDOW_HEIGHT;
    bool fullscreen = false;
    unsigned int borderSize = 1;
    BorderColor borderColor = B_BLACK;
    WindowColor windowColor = W_WHITE;
};


class IWindow {
    public:
        IWindow() = default;
        virtual ~IWindow() = default;

        virtual void create(WindowSettings settings) = 0;
        virtual void expose() = 0;
        virtual WindowEvent getEvent() = 0;
        virtual void closeWindow() = 0;

    protected:
        int _xCoord = DEFAULT_WINDOW_X;
        int _yCoord = DEFAULT_WINDOW_Y;
        unsigned int _width = DEFAULT_WINDOW_WIDTH;
        unsigned int _height = DEFAULT_WINDOW_HEIGHT;
};
