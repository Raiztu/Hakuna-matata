#pragma once

#include "InputHandler.hpp"

/// @brief Обработчик ввода для главного меню
class MenuInputHandler : public InputHandler {
public:
    MenuInputHandler();
    ~MenuInputHandler();

    /// @brief Обрабатывает нажатие кнопки мыши в меню
    /// @param event Событие SDL
    virtual void onMouseButtonDown(SDL_Event event) override;
    /// @brief Обрабатывает нажатие клавиши (Escape для выхода)
    /// @param event Событие SDL
    virtual void onKeyDown(SDL_Event event) override;

    /// @brief Проверяет, нажата ли кнопка "Играть" или "Выход"
    /// @param x Координата X клика
    /// @param y Координата Y клика
    /// @return true, если клик по одной из кнопок
    bool checkButtonClick(int x, int y);
    /// @brief Проверяет клик в диалоге подтверждения выхода
    /// @param x Координата X
    /// @param y Координата Y
    /// @return true, если клик по кнопке "Да" или "Нет"
    bool checkConfirmDialogClick(int x, int y);

    bool playClicked; ///< Нажата ли кнопка "Играть"
    bool exitClicked; ///< Нажата ли кнопка "Выход"

    bool isConfirmMode; ///< Активен ли диалог подтверждения выхода

    bool confirmExitConfirmed; ///< Подтверждён ли выход (кнопка "Да")
    bool confirmExitCancelled; ///< Отменён ли выход (кнопка "Нет")

    /// @brief Сбрасывает все флаги кнопок и диалога
    void resetFlags(); 
    /// @brief Включает режим диалога подтверждения выхода
    void enterConfirmMode(); 
    /// @brief Выключает режим диалога подтверждения выхода
    void exitConfirmMode(); 
};