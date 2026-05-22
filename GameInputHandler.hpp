#pragma once

#include "InputHandler.hpp"
#include <string>
#include <vector>

class GameInputHandler : public InputHandler {
public:
    GameInputHandler();
    ~GameInputHandler();

    virtual void onMouseButtonDown(SDL_Event event) override;
    virtual void onKeyDown(SDL_Event event) override;

    // Проверка клика по букве
    bool checkLetterClick(int x, int y, int& letterIndex);

    // Получение состояния буквы (нажата/не нажата)
    bool isLetterGuessed(int index) const { return letterStates_[index]; }

    // Отметить букву как угаданную
    void setLetterGuessed(int index) { letterStates_[index] = true; }

    // Сброс всех букв (для новой игры)
    void resetLetters();

    // Получить букву по индексу
    std::string getLetter(int index) const { return letters_[index]; }

    // Количество букв
    int getLetterCount() const { return letters_.size(); }

    // Получить прямоугольник буквы для отрисовки
    void getLetterRect(int index, float& x, float& y, float& w, float& h) const;
    void addLetterRect(float x, float y, float w, float h);

    // Метод для получения последней нажатой буквы
    int getLastPressedLetter() const { return lastPressedLetter_; }
    void clearLastPressedLetter() { lastPressedLetter_ = -1; }

    bool backToMenuClicked; // Флаг для возврата в меню
    void checkBackButtonClick(int x, int y); // Метод для проверки клика по кнопке возврата
    void resetGameFlags() { backToMenuClicked = false; } // Метод для сброса флагов
    
private:
    int lastPressedLetter_;
    std::vector<std::string> letters_; // Все буквы алфавита
    std::vector<bool> letterStates_; // true = уже нажата
    // Позиции и размеры буквенных кнопок
    std::vector<SDL_FRect> letterRects_;
};