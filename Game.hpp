#pragma once

#include "GameObject.hpp"
#include "InputHandler.hpp"
#include "MenuInputHandler.hpp"
#include "Player.hpp"

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>

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

    // Mетод для отрисовки текста помощи
    void renderHelpText();

    // Метод для отрисовки ползунка
    void renderVolumeSlider();

};
