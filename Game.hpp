#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "GameObject.hpp"
#include "InputHandler.hpp"
#include "MenuInputHandler.hpp"
#include "Player.hpp"
#include "GameInputHandler.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <ctime>     // для srand

enum GameState {
    STATE_MENU,
    STATE_GAME
};

class Game {
public:
    Game();
    ~Game();
    bool init(std::string, int, int, int);
    void render();
    void update();
    void handleEvents();
    void clean();
    void startGame() { running_ = true; }
    void stopGame() { running_ = false; }
    bool isRunning() { return running_; }
    void playMusic();
    void stopMusic();
    void setMusicVolume(float volume);

private:
    bool running_ = false;
    bool sdlInitialized_ = false;
    GameState currentState_ = STATE_MENU;

    SDL_Window* window_;
    SDL_Renderer* renderer_;

    // Обработчики ввода
    GameInputHandler* gameHandler_; 
    MenuInputHandler* menuHandler_;
    InputHandler* currentHandler_;

    // Для музыки
    MIX_Mixer* mixer_;         // Микшер 
    MIX_Audio* musicAudio_;    // Загруженный аудиофайл
    MIX_Track* musicTrack_;    // Трек для воспроизведения


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

    GameObject backToMenuIcon_; 

    // Mетод для отрисовки текста помощи
    void renderHelpText();

    // Метод для отрисовки ползунка
    void renderVolumeSlider();


    ///// Работа со словом
    std::string currentCategory_;  // Категория текущего слова
    int wrongGuesses_;              // Количество ошибок
    int maxWrongGuesses_;           // Максимум ошибок (6)
    std::vector<std::pair<std::string, std::string>> wordList_;  // Список всех слов из файла

    // Загрузка слов из файла
    bool loadWordList(const std::string& filename);
    // Метод для новой игры
    void startNewGame();
    // Метод для проверки угаданной буквы
    void checkLetterInWord(int letterIndex);
    // Метод для отрисовки текущего состояния слова
    void renderWordProgress();


    ///// Работа с клавиатурой
    // Выведенная клавиатура
    std::string currentWord_;        // Текущее загаданное слово
    std::vector<bool> wordProgress_; // Угаданные буквы в слове

    // Метод для отрисовки буквенной клавиатуры
    void renderKeyboard();
};
