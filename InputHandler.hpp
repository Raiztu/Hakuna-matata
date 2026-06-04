#pragma once

#include <SDL3/SDL.h>  
#include "Vector2D.hpp"
#include <iostream>  
#include <vector>    

/// @brief Номера кнопок мыши для удобства (индексы массива)
enum mouse_buttons {
    LEFT = SDL_BUTTON_LEFT - 1, ///< Левая кнопка
    MIDDLE = SDL_BUTTON_MIDDLE - 1, ///< Средняя кнопка
    RIGHT = SDL_BUTTON_RIGHT - 1, ///< Правая кнопка
    BACK = SDL_BUTTON_X1 - 1, ///< Кнопка "Назад" (X1)
    FORW = SDL_BUTTON_X2 - 1 ///< Кнопка "Вперёд" (X2)
};

/// @brief Базовый класс для обработки ввода
class InputHandler {
public:
    InputHandler();
    virtual ~InputHandler() { delete mousePosition_; }

    /// @brief Обрабатывает событие SDL
    /// @param event Событие SDL
    void handle(SDL_Event event); 
    /// @brief Возвращает текущую позицию мыши
    Vector2D* getMousePosition(); 
    /// @brief Проверяет, нажата ли кнопка мыши
    /// @param buttonNumber Номер кнопки (из enum mouse_buttons)
    bool getMouseButtonState(int buttonNumber); 
    /// @brief Проверяет, нажата ли клавиша на клавиатуре
    /// @param key Скан-код клавиши
    bool isKeyDown(SDL_Scancode key);

    /// Виртуальные методы для переопределения
    virtual void onMouseButtonDown(SDL_Event event);
    virtual void onMouseButtonUp(SDL_Event event);
    virtual void onMouseMotion(SDL_Event event);
    virtual void onKeyDown(SDL_Event event);

    /// @brief Скан-код клавиши
    /// @return true, если клик был по иконке помощи
    bool checkHelpIconClick(int x, int y);
    /// @brief Проверяет клик по иконке звука
    /// @return true, если клик был по иконке звука
    bool checkSoundIconClick(int x, int y);
    /// @brief Проверяет клик в диалоге помощи (кнопка "Понятно")
    bool checkHelpDialogClick(int x, int y);
    /// @brief Проверяет клик в диалоге звука (кнопка "OK")
    bool checkSoundDialogClick(int x, int y);
    void updateSliderHandlePosition(); ///< Обновляет позицию кружка ползунка

    void resetCommonFlags(); ///< Сбрасывает общие флаги (helpConfirmed, soundConfirmed, isDraggingSlider)

    void enterHelpMode(); ///<  Вход в режим помощи
    void exitHelpMode(); ///<  Выход из режима помощи
    void enterSoundMode(); ///<  Вход в режим звука
    void exitSoundMode(); ///<  Выход из режима звука

    bool isHelpMode; ///< Активен ли диалог помощи
    bool isSoundMode; ///< Активен ли диалог звука
    bool helpConfirmed; ///<  Для диалога помощи соглашения (закрыть диалог)
    bool soundConfirmed; ///<  Для диалога звука (закрыть диалог)

    int sliderX, sliderY; ///<  координаты ползунка (левая точка)
    int sliderWidth; ///<  ширина ползунка
    int sliderHandleX; ///<  текущая X позиция кружка

    bool isDraggingSlider; ///< Перетаскивается ли кружок в данный момент
    float volumeLevel; ///< Уровень громкости от 0.0 до 1.0 

    float getVolumeLevel() const; ///< Возвращает текущую громкость (0..1)
    void setVolumeLevel(float level); ///< Устанавливает громкость и обновляет положение кружка

protected:
    Vector2D* mousePosition_; ///< Текущая позиция мыши
    std::vector<bool> mouseStates_; ///< Состояния кнопок мыши (нажата/не нажата)
    const bool* keystates_; ///< Состояния клавиш клавиатуры (от SDL)
};