//
// Created by raykuzu on 16/11/2020.
//

#pragma once

#include "AModule.hpp"
#include "IWindow.hpp"

class InputModule : public AModule {
    public:
        explicit InputModule(IWindow *window): _window(window) {}
        ~InputModule() override = default;

        void init() override {

        }

        void term() override {

        }

        void update(Scene &scene) override {
            WindowEvent event = _window->getEvent();

            switch (event.type) {
                case WE_EXIT:
                    scene.setExit(true);
                    break;
                case WE_INPUT_PRESSED:
                    scene.setLastInput(event.input);
                    break;
                case WE_INPUT_RELEASED:
                    break;
                case WE_POINTER_MOTION:
                    break;
                case WE_UNKNOWN:
                    break;
            }

            scene.setInputs(_window->getCurrentlyPressedInput());
        }

    private:
        IWindow *_window;
};
