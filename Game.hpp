#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "GameObject.hpp"
#include "InputHandler.hpp"
#include "MenuInputHandler.hpp"
#include "GameInputHandler.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <ctime> 

/// @brief Состояния игры: меню или активная игра
enum GameState {
    STATE_MENU, ///< Главное меню
    STATE_GAME ///< Режим игры
};

/// @brief Главный класс игры. Управляет окном, рендерером, обработчиками ввода, музыкой и логикой
class Game { 
public:
    Game();
    ~Game();
    /// @brief Инициализирует игру: окно, рендерер, загрузку текстур и слов, музыку
    bool init(std::string, int, int, int);
    void render(); ///< Отрисовывает всё: меню, игровой фон, виселицу, клавиатуру, диалоги
    void update(); ///< Обновляет логику (например, применяет громкость)
    void handleEvents(); ///< Обрабатывает все события SDL (ввод, закрытие окна)
    void clean(); ///< Освобождает ресурсы: музыку, обработчики, окно, SDL
    void startGame() { running_ = true; } ///< Запускает главный цикл
    void stopGame() { running_ = false; } ///< Останавливает главный цикл
    bool isRunning() { return running_; } ///< Возвращает true, пока игра работает
    void playMusic(); ///< Загружает и запускает фоновую музыку (бесконечный цикл)
    void stopMusic(); ///< Останавливает и освобождает музыку
    void setMusicVolume(float volume); ///< Устанавливает громкость музыки (0..1)

private:
    bool running_ = false; ///< Флаг работы главного цикла
    bool sdlInitialized_ = false; ///< Флаг успешной инициализации SDL
    GameState currentState_ = STATE_MENU; ///< Текущее состояние: меню или игра
     
    SDL_Window* window_; ///< Окно SDL
    SDL_Renderer* renderer_; ///< Рендерер SDL

    GameInputHandler* gameHandler_; ///< Обработчик ввода в игре
    MenuInputHandler* menuHandler_; ///< Обработчик ввода в меню
    InputHandler* currentHandler_; ///< Текущий активный обработчик

    MIX_Mixer* mixer_;         ///< Микшер SDL_mixer
    MIX_Audio* musicAudio_;    ///< Загруженный аудиофайл
    MIX_Track* musicTrack_;    ///< Трек для воспроизведения


    GameObject playButton_;
    GameObject exitButton_;
    GameObject volumeIcon_;
    GameObject helpIcon_;


    GameObject confirmDialogBg_;    
    GameObject confirmButton_;      
    GameObject cancelButton_;       


    GameObject helpDialogBg_;
    GameObject IseeButton_;

    GameObject soundDialogBg_;     
    GameObject soundOkButton_;    

    GameObject sliderBg_;       
    GameObject sliderHandle_;

    GameObject backToMenuIcon_;  ///< Иконка возврата в меню
    GameObject winDialogBg_; ///< Фон диалога победы
    GameObject loseDialogBg_; ///< Фон диалога поражения
    GameObject newGameButton_; ///< Кнопка "Новая игра"
    GameObject menuButton_; ///< Кнопка "В меню"    

    void renderWinDialogText();   ///< Текст для диалога победы
    void renderLoseDialogText();  ///< Текст для диалога поражения

    void renderHelpText(); ///< Текст для диалога помощи

    void renderVolumeSlider(); ///< Отрисовка ползунка громкости


    std::string currentCategory_; ///< Категория текущего слова
    int wrongGuesses_; ///< Текущее количество ошибок
    int maxWrongGuesses_; ///< Максимум ошибок (6)
    std::vector<std::pair<std::string, std::string>> wordList_; ///< Список слов и категорий

    bool loadWordList(const std::string& filename); ///< Загружает слова из файла
    void startNewGame(); ///< Начинает новую игру (случайное слово)
    void checkLetterInWord(int letterIndex); ///< Проверяет, есть ли буква в слове
    void renderWordProgress(); ///< Отрисовывает угаданные буквы и категорию


    std::string currentWord_; ///< Текущее загаданное слово
    std::vector<bool> wordProgress_; ///< Угаданы ли буквы (true – угадана)

    void renderKeyboard(); ///< Отрисовывает экранную клавиатуру

    void renderHangman(); ///< Отрисовывает части виселицы в зависимости от числа ошибок
};
