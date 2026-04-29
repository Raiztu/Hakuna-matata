#pragma once

#include <SDL3/SDL.h>  

#include "Vector2D.hpp"
#include <iostream>  
#include <vector>    

// Перечисление кнопок мыши
// SDL_BUTTON_LEFT - 1 вычитаем, чтобы получить индекс 0 для массива
enum mouse_buttons {
    LEFT = SDL_BUTTON_LEFT - 1,    // 0 - левая кнопка
    MIDDLE = SDL_BUTTON_MIDDLE - 1, // 1 - средняя кнопка (колёсико)
    RIGHT = SDL_BUTTON_RIGHT - 1,   // 2 - правая кнопка
    BACK = SDL_BUTTON_X1 - 1,       // 3 - боковая кнопка "назад"
    FORW = SDL_BUTTON_X2 - 1        // 4 - боковая кнопка "вперёд"
};

class InputHandler {
public:

    InputHandler();
    virtual ~InputHandler() { delete mousePosition_; }

    // Основные методы обработки ввода (общие для всех состояний)
    void handle(SDL_Event event);
    Vector2D* getMousePosition();
    bool getMouseButtonState(int buttonNumber);
    bool isKeyDown(SDL_Scancode key);

    // Виртуальные методы для переопределения
    virtual void onMouseButtonDown(SDL_Event event) {}
    virtual void onMouseButtonUp(SDL_Event event) {}
    virtual void onMouseMotion(SDL_Event event) {}
    virtual void onKeyDown(SDL_Event event) {}

protected:

    Vector2D* mousePosition_;         // Указатель на объект с позицией мыши
    std::vector<bool> mouseStates_;  // Массив состояний кнопок мыши 
    const bool* keystates_;           // Указатель на массив состояний клавиш от SDL

};