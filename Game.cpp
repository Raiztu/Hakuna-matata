#include "Game.hpp" 
#include "InputHandler.hpp"
#include "MenuInputHandler.hpp"
#include "TextureManager.hpp"

#include <iostream>

Game::Game() : mixer_(nullptr), musicAudio_(nullptr), musicTrack_(nullptr) {}
Game::~Game() { stopMusic(); }


bool Game::init(std::string title, int w, int h, int flags) {

    window_ = SDL_CreateWindow(title.c_str(), w, h, flags);
    if (window_ == nullptr) {
        std::cerr << "window error" << std::endl;
        return false;
    }
    std::cout << "window created" << std::endl;

    renderer_ = SDL_CreateRenderer(window_, NULL);
    if (renderer_ == nullptr) {
        std::cerr << "renderer error" << std::endl;
        return false;
    }
    std::cout << "renderer created" << std::endl;

    // Инициализация SDL для работы с аудио
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {  
        // Если инициализация не удалась - выводим ошибку и прерываем запуск
        std::cerr << "SDL could not initialize! SDL Error" << std::endl;
        return false;  
    }
    sdlInitialized_ = true; // SDL успешно инициализирована
    
    // Инициализация SDL_mixer 
    if (!MIX_Init()) {
        std::cerr << "MIX_Init failed" << std::endl; //// Если ошибка - сообщаем и прерываем запуск
        return false;
    }

    // Создаём микшер, привязанный к устройству воспроизведения по умолчанию
    mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer_) {
        std::cerr << "MIX_CreateMixerDevice failed" << std::endl;
        MIX_Quit();
        return false;
    }

    menuHandler_ = new MenuInputHandler();
    currentHandler_ = menuHandler_;

    // По тексту
    // Инициализация 
    if (!TextureManager::Instance().initTTF()) {
        return false;
    }
    // Загрузка шрифта
    if (!TextureManager::Instance().loadFont("assets/anime-ace-v3.ttf", "main_font", 21)) {
        std::cerr << "Failed to load main font" << std::endl; // Продолжаем работу, но текст не будет отображаться 
    }


    // Загрузка текстур
    if (!TextureManager::Instance().load("assets/menu_bg.png", "menu_bg", renderer_)) {
        std::cerr << "Failed to load menu background" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/play_button.png", "play_btn", renderer_)) {
        std::cerr << "Failed to load play button" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/exit_button.png", "exit_btn", renderer_)) {
        std::cerr << "Failed to load exit button" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/volume_da.png", "volume_icon", renderer_)) {
        std::cerr << "Failed to load volume button" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/help.png", "help_icon", renderer_)) {
        std::cerr << "Failed to load hepl button" << std::endl;
        return false;
    }
    //////////////////////////////////////////// Для выхода
    if (!TextureManager::Instance().load("assets/confirm_bg.png", "confirm_bg", renderer_)) {
        std::cerr << "Warning: confirm_bg.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/yes_button.png", "confirm_btn", renderer_)) {
        std::cerr << "Warning: yes_btn.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/no_button.png", "cancel_btn", renderer_)) {
        std::cerr << "Warning: no_btn.png not loaded" << std::endl;
    }
    //////////////////////////////////////////// Для помощи
    if (!TextureManager::Instance().load("assets/help_bg.png", "help_bg", renderer_)) {
        std::cerr << "Warning: help_bg.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/Isee_button.png", "Isee_btn", renderer_)) {
        std::cerr << "Warning: Isee_btn.png not loaded" << std::endl;
    }

    //////////////////////////////////////////// Для звука
    if (!TextureManager::Instance().load("assets/sound_bg.png", "sound_bg", renderer_)) {
        std::cerr << "Warning: sound_bg.png not loaded" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/ok_button.png", "sound_btn", renderer_)) {
        std::cerr << "Warning: ok_button.png not loaded" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/slider_bg.png", "slider_bg", renderer_)) { // Для ползунка
        std::cerr << "Warning: slider_bg.png not loaded" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/slider_handle.png", "slider_handle", renderer_)) {
        std::cerr << "Warning: slider_handle.png not loaded" << std::endl;
        return false;
    }

    ///////////////////////////////////////////////////////// Игра сама
    if (!TextureManager::Instance().load("assets/game_bg.png", "game_bg", renderer_)) {
        std::cerr << "Failed to load game background" << std::endl;
    }

    // Настраиваем
    playButton_.load("play_btn", 198, 653, 325, 80);
    exitButton_.load("exit_btn", 198, 765, 325, 80);
    volumeIcon_.load("volume_icon", 578, 30, 112, 112);
    helpIcon_.load("help_icon", 30, 30, 112, 112);

    /////////////////////////////////// Для выхода
    confirmDialogBg_.load("confirm_bg", 35, 305, 650, 350);
    confirmButton_.load("confirm_btn", 190, 545, 120, 80);
    cancelButton_.load("cancel_btn", 410, 545, 120, 80);

    /////////////////////////////////// Для помощи
    helpDialogBg_.load("help_bg", 35, 305, 650, 350);
    IseeButton_.load("Isee_btn", 300, 545, 120, 80);  


    ////////////////////////////////// Для звука
    soundDialogBg_.load("sound_bg", 35, 305, 650, 350);
    soundOkButton_.load("sound_btn", 300, 545, 120, 80);
    sliderBg_.load("slider_bg", menuHandler_->sliderX, menuHandler_->sliderY - 10, 400, 20); // X-координата левого края полоски, Y-координата верхнего края полоски 
    sliderHandle_.load("slider_handle", menuHandler_->sliderHandleX, menuHandler_->sliderY - 20, 40, 40);

   // Запускаем музыку
    playMusic();
    return true;
}

void Game::playMusic() {
    if (!mixer_) return; // Если микшер не создан — выходим

    // Загружаем аудиофайл 
    musicAudio_ = MIX_LoadAudio(mixer_, "assets/music.ogg", false); // Загружаем аудиофайл
    if (!musicAudio_) {
        std::cerr << "Failed to load music.ogg" << std::endl;
            return;
    }

    // Создаём трек (канал) для воспроизведения звука
    musicTrack_ = MIX_CreateTrack(mixer_);
    if (!musicTrack_) {
        std::cerr << "Failed to create music track" << std::endl;
        MIX_DestroyAudio(musicAudio_); // Если трек не создался — удаляем загруженный аудиофайл
        musicAudio_ = nullptr;
        return;
    }

    // Привязываем загруженный аудиофайл к треку
    if (!MIX_SetTrackAudio(musicTrack_, musicAudio_)) {
        std::cerr << "Failed to set track audio" << std::endl;
        MIX_DestroyTrack(musicTrack_);
        musicTrack_ = nullptr;
        MIX_DestroyAudio(musicAudio_);
        musicAudio_ = nullptr;
        return;
    }

    // Настройка воспроизведения
    SDL_PropertiesID props = SDL_CreateProperties(); // Создаём пустой набор свойств для передачи параметров воспроизведения
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1); // Устанавливаем в свойствах параметр "количество повторов" в значение -1 (бесконечно)
    bool ok = MIX_PlayTrack(musicTrack_, props); // Запускаем трек с переданными свойствами
    SDL_DestroyProperties(props); // Освобождаем свойства, тк не нужны
    if (!ok) { // Если трек не запустился
        std::cerr << "Failed to play track" << std::endl;
        MIX_DestroyTrack(musicTrack_);
        musicTrack_ = nullptr;
        MIX_DestroyAudio(musicAudio_);
        musicAudio_ = nullptr;
        return;
    }

    // Устанавливаем начальную громкость (50%)
    setMusicVolume(menuHandler_->volumeLevel);
}

void Game::stopMusic() {
    if (musicTrack_) { // Если трек существует
        MIX_StopTrack(musicTrack_, 0); // Остановить воспроизведение (0 = без затухания)
        MIX_DestroyTrack(musicTrack_); // Уничтожить трек, освободить ресурсы
        musicTrack_ = nullptr;
    }
    if (musicAudio_) { // // Если аудиоданные загружены
        MIX_DestroyAudio(musicAudio_);
        musicAudio_ = nullptr;
    }
}

void Game::setMusicVolume(float volume) {
    if (!musicTrack_) return; //// Если трек не существует — выходим
    // volume от 0.0 до 1.0, MIX_SetTrackGain принимает float, где 1.0 = норма, >1.0 громче
    float gain = volume * 1.0f;  // Коэффициент усиления равен уровню громкости без доп умножения(1.0)
    if (gain < 0.0f) gain = 0.0f; // Ограничиваем снизу
    if (gain > 2.0f) gain = 2.0f; 
    MIX_SetTrackGain(musicTrack_, gain); // Устанавливаем коэффициент усиления для трека
}

// Отрисовка текста
void Game::renderHelpText() {
    SDL_Color textColor = { 0, 0, 0, 0 };

    std::string helpText =
        u8"   КАК ИГРАТЬ В ВИСЕЛИЦУ   \n\n"
        "1. Компьютер загадывает слово\n"
        "2. Пытайтесь угадать буквы по одной\n"
        "3. Каждая ошибка рисует часть виселицы\n"
        "4. Угадайте все буквы до того, как виселица будет построена!\n\n"
        "Удачи и хорошей игры!";

    TextureManager::Instance().drawTextWrapped(helpText, "main_font", 50, 320, 650, textColor, renderer_); // Отрисовка текста с автоматическим переносом
}

void Game::renderVolumeSlider() {
    // Обновляем позицию кружка
    sliderHandle_.load("slider_handle", menuHandler_->sliderHandleX, menuHandler_->sliderY - 20, 40, 40);

    // Рисуем полоску ползунка
    sliderBg_.draw(renderer_);

    // Рисуем кружок поверх
    sliderHandle_.draw(renderer_);
}

// Уже непосредственно сама отрисовка
void Game::render() {
    // Очищаем экран
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    // Рисуем в зависимости от состояния
    if (currentState_ == STATE_MENU) {
        TextureManager::Instance().draw("menu_bg", 0, 0, 720, 960, renderer_);
        volumeIcon_.draw(renderer_);
        helpIcon_.draw(renderer_);

        if (menuHandler_->isConfirmMode) {
            // Диалог выхода
            confirmDialogBg_.draw(renderer_);
            confirmButton_.draw(renderer_);
            cancelButton_.draw(renderer_);

        }
        else if (menuHandler_->isHelpMode) {  // Диалог помощи
            helpDialogBg_.draw(renderer_);
            IseeButton_.draw(renderer_);

            // Отрисовка текста помощи
            renderHelpText();
        }
        else if (menuHandler_->isSoundMode) { //Диалог звука
            soundDialogBg_.draw(renderer_);
            soundOkButton_.draw(renderer_);
            renderVolumeSlider();
        }
        else {
            // Рисуем кнопки меню
            playButton_.draw(renderer_);
            exitButton_.draw(renderer_);
        }
    }
    else if (currentState_ == STATE_GAME) {
        TextureManager::Instance().draw("game_bg", 0, 0, 720, 960, renderer_);
        volumeIcon_.draw(renderer_);
        helpIcon_.draw(renderer_);

        if  (menuHandler_->isHelpMode) {  // Диалог помощи
            helpDialogBg_.draw(renderer_);
            IseeButton_.draw(renderer_);

            // Отрисовка текста помощи
            renderHelpText();
        }
        else if (menuHandler_->isSoundMode) { // Диалог звука и в игре
            soundDialogBg_.draw(renderer_);
            soundOkButton_.draw(renderer_);

            // Отрисовка ползунка
            renderVolumeSlider();
        }
    }

    SDL_RenderPresent(renderer_);
}

void Game::update() {
    if (currentState_ == STATE_MENU) {
        // Обновляем громкость музыки при изменении ползунка
        if (menuHandler_->isSoundMode) {
            setMusicVolume(menuHandler_->volumeLevel);
        }

    }
    else if (currentState_ == STATE_GAME) {
        // Обновляем громкость музыки при изменении ползунка
        if (menuHandler_->isSoundMode) {
            setMusicVolume(menuHandler_->volumeLevel);
        }
    }
}

void Game::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_EVENT_QUIT:
            stopGame();
            break;
        default:
            if (menuHandler_) {
                menuHandler_->handle(event);
            }
            break;
        }
    }

    if (currentState_ == STATE_MENU && menuHandler_) {
        if (menuHandler_->isConfirmMode) {
            // В режиме подтверждения
            if (menuHandler_->confirmExitConfirmed) {
                stopGame();  // Выходим
            }
            if (menuHandler_->confirmExitCancelled) {
                menuHandler_->exitConfirmMode();  // Возвращаемся в меню
            }
        }
        else if (menuHandler_->isHelpMode) {  // Режим помощи
            if (menuHandler_->helpConfirmed) {
                menuHandler_->exitHelpMode();  // Закрываем диалог помощи
            }
        }
        else if (menuHandler_->isSoundMode) {  // Режим звука 
            if (menuHandler_->soundConfirmed) {
                menuHandler_->exitSoundMode();  // Закрываем диалог звука
            }
        }
        else {
            // Обычный режим меню
            if (menuHandler_->playClicked) {
                std::cout << "Play button clicked! Switching to game..." << std::endl;
                currentState_ = STATE_GAME;
                menuHandler_->resetFlags();
            }
            if (menuHandler_->exitClicked) {
                std::cout << "Exit button clicked! Quitting..." << std::endl;
                stopGame();
            }
        }
    }
    else if (currentState_ == STATE_GAME && menuHandler_) {
        if (menuHandler_->isHelpMode) {  // Режим помощи
            if (menuHandler_->helpConfirmed) {
                menuHandler_->exitHelpMode();  // Закрываем диалог помощи
            }
        }
        else if (menuHandler_->isSoundMode) {  // Режим звука в игре 
            if (menuHandler_->soundConfirmed) {
                menuHandler_->exitSoundMode();  // Закрываем диалог звука
            }
        }
    }
}

void Game::clean() {
    std::cout << "Cleaning up..." << std::endl;

    // Остановить и освободить всё, связанное с музыкой
    stopMusic();

    // Уничтожить микшер
    if (mixer_) {
        MIX_DestroyMixer(mixer_);
        mixer_ = nullptr;
    }

    // Завершить работу SDL_mixer
    MIX_Quit();

    // Очистка TTF 
    TextureManager::Instance().cleanupTTF();

    delete menuHandler_;
    menuHandler_ = nullptr;
    currentHandler_ = nullptr;

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    if (sdlInitialized_) {
        SDL_Quit();
    }
}