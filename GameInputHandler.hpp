#pragma once

#include "InputHandler.hpp"
#include <string>
#include <vector>

/// @brief Класс обработки ввода во время активной игры.
/// Обрабатывает клики по клавиатуре на экране, нажатия клавиш (Esc),
/// клики по иконкам помощи/звука, а также диалоги выигрыша/проигрыша.
class GameInputHandler : public InputHandler {
public:
    GameInputHandler();
    ~GameInputHandler();

    /// @brief Обрабатывает событие нажатия кнопки мыши
    /// @param event Событие SDL, содержащее координаты и тип кнопки
    virtual void onMouseButtonDown(SDL_Event event) override;
    /// @brief Обрабатывает событие нажатия клавиши клавиатуры
    /// @param event Событие SDL с информацией о нажатой клавише
    virtual void onKeyDown(SDL_Event event) override;

    /// @brief Проверяет, попал ли клик на одну из букв клавиатуры
    /// @param x Координата X клика
    /// @param y Координата Y клика
    /// @param letterIndex Индекс нажатой буквы, если клик попал
    /// @return true, если клик был по любой из букв, иначе false
    bool checkLetterClick(int x, int y, int& letterIndex);

    /// @brief Возвращает состояние буквы (была ли она нажата)
    /// @param index Индекс буквы (0..32)
    /// @return true, если буква уже была нажата (угадана или нет)
    bool isLetterGuessed(int index) const { return letterStates_[index]; }

    /// @brief Отмечает букву как уже нажатую (используется при сбросе)
    /// @param index Индекс буквы
    void setLetterGuessed(int index) { letterStates_[index] = true; }

    /// @brief Сбрасывает состояния всех букв, очищает последнюю нажатую букву и флаг возврата
    void resetLetters();

    /// @brief Возвращает букву по индексу
    /// @param index Индекс буквы
    /// @return Однобайтовая строка с буквой в UTF-8
    std::string getLetter(int index) const { return letters_[index]; }

    /// @brief Возвращает общее количество букв на клавиатуре
    /// @return 33 буквы
    int getLetterCount() const { return letters_.size(); }

    /// @brief Заполняет координаты и размеры прямоугольника кнопки для заданной буквы
    /// @param index Индекс буквы
    /// @param x X координата левого верхнего угла
    /// @param y Y координата левого верхнего угла
    /// @param w Ширина кнопки
    /// @param h Высота кнопки
    void getLetterRect(int index, float& x, float& y, float& w, float& h) const;
    /// @brief Добавляет прямоугольник кнопки для буквы (используется при инициализации)
    /// @param x X координата
    /// @param y Y координата
    /// @param w Ширина
    /// @param h Высота
    void addLetterRect(float x, float y, float w, float h);

    /// @brief Возвращает индекс последней нажатой буквы
    /// @return Индекс буквы (0..32) или -1, если ни одной не нажато
    int getLastPressedLetter() const { return lastPressedLetter_; }
    /// @brief Сбрасывает индекс последней нажатой буквы в -1 (используется после обработки)
    void clearLastPressedLetter() { lastPressedLetter_ = -1; }

    bool backToMenuClicked; ///< Флаг для возврата в меню
    /// @brief Проверяет, нажата ли кнопка возврата в меню
    /// @param x Координата X клика
    /// @param y Координата Y клика
    void checkBackButtonClick(int x, int y); 
    /// @brief Сбрасывает флаги, связанные с игрой
    void resetGameFlags() { backToMenuClicked = false; } 

    bool isWinDialogMode;      ///< Активен ли диалог победы
    bool isLoseDialogMode;     ///< Активен ли диалог поражения
    bool newGameFromWin;       ///< Запрошена новая игра из диалога победы
    bool backToMenuFromWin;    ///< Запрошен выход в меню из диалога победы
    bool newGameFromLose;      ///< Запрошена новая игра из диалога поражения
    bool backToMenuFromLose;   ///< Запрошен выход в меню из диалога поражения

    /// @brief Переводит обработчик в режим диалога победы
    void enterWinDialog();
    /// @brief Выход из режима диалога победы
    void exitWinDialog();
    /// @brief Переводит обработчик в режим диалога поражения
    void enterLoseDialog();
    /// @brief Выход из режима диалога поражения
    void exitLoseDialog();
    /// @brief Сбрасывает флаги диалогов (новой игры/выхода в меню) для обоих диалогов
    void resetDialogFlags();

    /// @brief Проверяет клик в диалоге победы
    /// @param x Координата X
    /// @param y Координата Y
    /// @return true, если клик был по одной из кнопок
    bool checkWinDialogClick(int x, int y);
    /// @brief Проверяет клик в диалоге поражения
    /// @param x Координата X
    /// @param y Координата Y
    /// @return true, если клик был по одной из кнопок
    bool checkLoseDialogClick(int x, int y);

private:
    int lastPressedLetter_; ///< Индекс последней нажатой буквы
    std::vector<std::string> letters_; ///< Список всех букв русского алфавита 
    std::vector<bool> letterStates_; ///< Состояние каждой буквы: нажата (true) или нет
    std::vector<SDL_FRect> letterRects_; ///< Позиции и размеры буквенных кнопок
};