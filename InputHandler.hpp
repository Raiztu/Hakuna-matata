#pragma once

#include <SDL3/SDL.h>  
#include "Vector2D.hpp"
#include <iostream>  
#include <vector>    

enum mouse_buttons {
    LEFT = SDL_BUTTON_LEFT - 1,
    MIDDLE = SDL_BUTTON_MIDDLE - 1,
    RIGHT = SDL_BUTTON_RIGHT - 1,
    BACK = SDL_BUTTON_X1 - 1,
    FORW = SDL_BUTTON_X2 - 1
};

class InputHandler {
public:
    InputHandler();
    virtual ~InputHandler() { delete mousePosition_; }

    void handle(SDL_Event event); // Обработка события
    Vector2D* getMousePosition(); // Координаты мыши
    bool getMouseButtonState(int buttonNumber); // Проверка, нажата ли кнопка мыши
    bool isKeyDown(SDL_Scancode key); // Проверка, нажата ли клавиша

    // Виртуальные методы для переопределения
    virtual void onMouseButtonDown(SDL_Event event);
    virtual void onMouseButtonUp(SDL_Event event);
    virtual void onMouseMotion(SDL_Event event);
    virtual void onKeyDown(SDL_Event event);

    // Методы для проверки кликов на иконки и в этих диалогах
    bool checkHelpIconClick(int x, int y);
    bool checkSoundIconClick(int x, int y);
    bool checkHelpDialogClick(int x, int y);
    bool checkSoundDialogClick(int x, int y);

    void updateSliderHandlePosition(); // обновить позицию кружка

    void resetCommonFlags();

    void enterHelpMode(); // Вход в режим помощи
    void exitHelpMode(); // Выход из режима помощи
    void enterSoundMode(); // Вход в режим звука
    void exitSoundMode(); // Выход из режима звука

    // Флаги для определения, в каком мы режиме (чтобы отрисовывать)
    bool isHelpMode;
    bool isSoundMode;
    // Для диалога помощи соглашения (закрыть диалог)
    bool helpConfirmed;
    // Для диалога звука (закрыть диалог)
    bool soundConfirmed;

    // Позиции ползунка 
    int sliderX, sliderY; // координаты ползунка (левая точка)
    int sliderWidth; // ширина ползунка
    int sliderHandleX; // текущая X позиция кружка

    // Для ползунка громкости
    bool isDraggingSlider; // перетаскивается ли кружок
    float volumeLevel; // уровень громкости от 0.0 до 1.0 

    // Для синхронизации звука между состояниями
    float getVolumeLevel() const;
    void setVolumeLevel(float level);

protected:
    Vector2D* mousePosition_;
    std::vector<bool> mouseStates_;
    const bool* keystates_;
};