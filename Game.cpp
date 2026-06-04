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

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error" << std::endl;
        return false;
    }
    sdlInitialized_ = true; 

    if (!MIX_Init()) {
        std::cerr << "MIX_Init failed" << std::endl;
        return false;
    }

    mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!mixer_) {
        std::cerr << "MIX_CreateMixerDevice failed" << std::endl;
        MIX_Quit();
        return false;
    }

    menuHandler_ = new MenuInputHandler();
    gameHandler_ = new GameInputHandler();
    currentHandler_ = menuHandler_;

    if (!loadWordList("assets/wordlist.txt")) {
        std::cerr << "Failed to load word list" << std::endl;
        return false;
    }
 
    maxWrongGuesses_ = 6;
    wrongGuesses_ = 0;
    startNewGame();

    if (!TextureManager::Instance().initTTF()) {
        return false;
    }

    if (!TextureManager::Instance().loadFont("assets/anime-ace-v3.ttf", "main_font", 21)) {
        std::cerr << "Failed to load main font" << std::endl; 
    }

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
    if (!TextureManager::Instance().load("assets/confirm_bg.png", "confirm_bg", renderer_)) {
        std::cerr << "Warning: confirm_bg.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/yes_button.png", "confirm_btn", renderer_)) {
        std::cerr << "Warning: yes_btn.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/no_button.png", "cancel_btn", renderer_)) {
        std::cerr << "Warning: no_btn.png not loaded" << std::endl;
    }
    if (!TextureManager::Instance().load("assets/help_bg.png", "help_bg", renderer_)) {
        std::cerr << "Warning: help_bg.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/Isee_button.png", "Isee_btn", renderer_)) {
        std::cerr << "Warning: Isee_btn.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/sound_bg.png", "sound_bg", renderer_)) {
        std::cerr << "Warning: sound_bg.png not loaded" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/ok_button.png", "sound_btn", renderer_)) {
        std::cerr << "Warning: ok_button.png not loaded" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/slider_bg.png", "slider_bg", renderer_)) { 
        std::cerr << "Warning: slider_bg.png not loaded" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/slider_handle.png", "slider_handle", renderer_)) {
        std::cerr << "Warning: slider_handle.png not loaded" << std::endl;
        return false;
    }

    if (!TextureManager::Instance().load("assets/game_bg.png", "game_bg", renderer_)) {
        std::cerr << "Failed to load game background" << std::endl;
    }

    if (!TextureManager::Instance().loadFont("assets/anime-ace-v3.ttf", "keyboard_font", 28)) {
        std::cerr << "Warning: keyboard font not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/back.png", "back_icon", renderer_)) {
        std::cerr << "Warning: back_button.png not loaded" << std::endl;

    }

    if (!TextureManager::Instance().load("assets/win_bg.png", "win_bg", renderer_)) {
        std::cerr << "Warning: win_bg.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/lose_bg.png", "lose_bg", renderer_)) {
        std::cerr << "Warning: lose_bg.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/new_game_button.png", "new_game_btn", renderer_)) {
        std::cerr << "Warning: new_game_button.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/to_menu_button.png", "to_menu_btn", renderer_)) {
        std::cerr << "Warning: to_menu_button.png not loaded" << std::endl;
    }

    if (!TextureManager::Instance().load("assets/hangman_head.png", "hangman_head", renderer_)) {
        std::cerr << "Warning: hangman_head.png not loaded" << std::endl;
    }
    if (!TextureManager::Instance().load("assets/hangman_body.png", "hangman_body", renderer_)) {
        std::cerr << "Warning: hangman_body.png not loaded" << std::endl;
    }
    if (!TextureManager::Instance().load("assets/hangman_right_arm.png", "hangman_right_arm", renderer_)) {
        std::cerr << "Warning: hangman_right_arm.png not loaded" << std::endl;
    }
    if (!TextureManager::Instance().load("assets/hangman_left_arm.png", "hangman_left_arm", renderer_)) {
        std::cerr << "Warning: hangman_left_arm.png not loaded" << std::endl;
    }
    if (!TextureManager::Instance().load("assets/hangman_right_leg.png", "hangman_right_leg", renderer_)) {
        std::cerr << "Warning: hangman_right_leg.png not loaded" << std::endl;
    }
    if (!TextureManager::Instance().load("assets/hangman_left_leg.png", "hangman_left_leg", renderer_)) {
        std::cerr << "Warning: hangman_left_leg.png not loaded" << std::endl;
    }

    playButton_.load("play_btn", 198, 653, 325, 80);
    exitButton_.load("exit_btn", 198, 765, 325, 80);
    volumeIcon_.load("volume_icon", 578, 30, 112, 112);
    helpIcon_.load("help_icon", 30, 30, 112, 112);

    confirmDialogBg_.load("confirm_bg", 35, 305, 650, 350);
    confirmButton_.load("confirm_btn", 190, 545, 120, 80);
    cancelButton_.load("cancel_btn", 410, 545, 120, 80);

    helpDialogBg_.load("help_bg", 35, 305, 650, 350);
    IseeButton_.load("Isee_btn", 300, 545, 130, 80);

    backToMenuIcon_.load("back_icon", 20, 890, 50, 50);

    winDialogBg_.load("win_bg", 35, 305, 650, 350);
    loseDialogBg_.load("lose_bg", 35, 305, 650, 350);
    newGameButton_.load("new_game_btn", 190, 545, 120, 80);
    menuButton_.load("to_menu_btn", 410, 545, 120, 80);

    soundDialogBg_.load("sound_bg", 35, 305, 650, 350);
    soundOkButton_.load("sound_btn", 300, 545, 120, 80);
    sliderBg_.load("slider_bg", menuHandler_->sliderX, menuHandler_->sliderY - 10, 400, 20); 
    sliderHandle_.load("slider_handle", menuHandler_->sliderHandleX, menuHandler_->sliderY - 20, 40, 40);

    playMusic();
    return true;
}

void Game::playMusic() {
    if (!mixer_) return; 
 
    musicAudio_ = MIX_LoadAudio(mixer_, "assets/music.ogg", false); 
    if (!musicAudio_) {
        std::cerr << "Failed to load music.ogg" << std::endl;
        return;
    }

    musicTrack_ = MIX_CreateTrack(mixer_);
    if (!musicTrack_) {
        std::cerr << "Failed to create music track" << std::endl;
        MIX_DestroyAudio(musicAudio_); 
        musicAudio_ = nullptr;
        return;
    }

    if (!MIX_SetTrackAudio(musicTrack_, musicAudio_)) {
        std::cerr << "Failed to set track audio" << std::endl;
        MIX_DestroyTrack(musicTrack_);
        musicTrack_ = nullptr;
        MIX_DestroyAudio(musicAudio_);
        musicAudio_ = nullptr;
        return;
    }

    SDL_PropertiesID props = SDL_CreateProperties(); 
    SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    bool ok = MIX_PlayTrack(musicTrack_, props);
    SDL_DestroyProperties(props); 
    if (!ok) {
        std::cerr << "Failed to play track" << std::endl;
        MIX_DestroyTrack(musicTrack_);
        musicTrack_ = nullptr;
        MIX_DestroyAudio(musicAudio_);
        musicAudio_ = nullptr;
        return;
    }

    setMusicVolume(menuHandler_->volumeLevel);
}

void Game::stopMusic() {
    if (musicTrack_) { 
        MIX_StopTrack(musicTrack_, 0); 
        MIX_DestroyTrack(musicTrack_); 
        musicTrack_ = nullptr;
    }
    if (musicAudio_) {
        MIX_DestroyAudio(musicAudio_);
        musicAudio_ = nullptr;
    }
}

void Game::setMusicVolume(float volume) {
    if (!musicTrack_) return; 
    float gain = volume * 1.0f;  
    if (gain < 0.0f) gain = 0.0f; 
    if (gain > 2.0f) gain = 2.0f;
    MIX_SetTrackGain(musicTrack_, gain); 
}

void Game::renderHelpText() {
    SDL_Color textColor = { 0, 0, 0, 0 };

    std::string helpText =
        u8"   КАК ИГРАТЬ В ВИСЕЛИЦУ   \n\n"
        "1. Компьютер загадывает слово\n"
        "2. Пытайся угадать буквы по одной\n"
        "3. Каждая ошибка рисует часть виселицы\n"
        "4. Угадай все буквы до того, как вся виселица будет построена!\n\n"
        "Удачи и хорошей игры!";

    TextureManager::Instance().drawTextWrapped(helpText, "main_font", 50, 320, 650, textColor, renderer_); 
}

void Game::renderWinDialogText() {
    SDL_Color textColor = { 0, 0, 0, 0 };
    std::string line1 = u8"Молодец!";
    std::string line2 = u8"Ты угадал слово!";
    std::string line3 = u8"Слово: " + currentWord_;
    float textX1 = 35 + TextureManager::Instance().getCenteredX(line1, "main_font", textColor, renderer_, 650);
    float textX2 = 35 + TextureManager::Instance().getCenteredX(line2, "main_font", textColor, renderer_, 650);
    float textX3 = 35 + TextureManager::Instance().getCenteredX(line3, "main_font", textColor, renderer_, 650);
    TextureManager::Instance().drawText(line1, "main_font", textX1, 330, textColor, renderer_);
    TextureManager::Instance().drawText(line2, "main_font", textX2, 360, textColor, renderer_);
    TextureManager::Instance().drawText(line3, "main_font", textX3, 390, textColor, renderer_);
}

void Game::renderLoseDialogText() {
    SDL_Color textColor = { 0, 0, 0, 255 };
    std::string line1 = u8"Ты проиграл :(";
    std::string line2 = u8"Загаданное слово: " + currentWord_;;
    float textX1 = 35 + TextureManager::Instance().getCenteredX(line1, "main_font", textColor, renderer_, 650);
    float textX2 = 35 + TextureManager::Instance().getCenteredX(line2, "main_font", textColor, renderer_, 650);
    TextureManager::Instance().drawText(line1, "main_font", textX1, 330, textColor, renderer_); 
    TextureManager::Instance().drawText(line2, "main_font", textX2, 360, textColor, renderer_);
}

void Game::renderVolumeSlider() {
    if (currentState_ == STATE_MENU && menuHandler_) {
        sliderBg_.load("slider_bg", menuHandler_->sliderX, menuHandler_->sliderY - 10, 400, 20);
        sliderHandle_.load("slider_handle", menuHandler_->sliderHandleX, menuHandler_->sliderY - 20, 40, 40);
    }
    else if (currentState_ == STATE_GAME && gameHandler_) {
        sliderBg_.load("slider_bg", gameHandler_->sliderX, gameHandler_->sliderY - 10, 400, 20);
        sliderHandle_.load("slider_handle", gameHandler_->sliderHandleX, gameHandler_->sliderY - 20, 40, 40);
    }

    sliderBg_.draw(renderer_);

    sliderHandle_.draw(renderer_);
}

void Game::renderKeyboard() {
    if (currentState_ != STATE_GAME) return;
    SDL_Color normalColor = { 70, 70, 100, 255 };    
    SDL_Color pressedColor = { 100, 100, 100, 255 }; 
    SDL_Color textNormalColor = { 0, 0, 0, 0 };
    SDL_Color textPressedColor = { 150, 150, 150, 255 }; 

    for (int i = 0; i < gameHandler_->getLetterCount(); i++) {
        float x, y, w, h;
        gameHandler_->getLetterRect(i, x, y, w, h);

        SDL_FRect bgRect = { x, y, w, h };  

        if (gameHandler_->isLetterGuessed(i)) {
            SDL_SetRenderDrawColor(renderer_, pressedColor.r, pressedColor.g, pressedColor.b, pressedColor.a);
        }
        else {
            SDL_SetRenderDrawColor(renderer_, normalColor.r, normalColor.g, normalColor.b, normalColor.a);
        }
        SDL_RenderFillRect(renderer_, &bgRect); 

        SDL_SetRenderDrawColor(renderer_, 200, 200, 200, 255);
        SDL_RenderRect(renderer_, &bgRect); 

        SDL_Color textColor;
        if (gameHandler_->isLetterGuessed(i)) {
            textColor = textPressedColor;  
        }
        else {
            textColor = textNormalColor; 
        }
        float textX = x + (w - 28) / 2;
        float textY = y + (h - 28) / 2;

        TextureManager::Instance().drawText(gameHandler_->getLetter(i), "keyboard_font", textX, textY, textColor, renderer_);
    }
}

bool Game::loadWordList(const std::string& filename) {
    std::ifstream file(filename); 
    if (!file.is_open()) {
        std::cerr << "Failed to open wordlist.txt" << std::endl;
        return false;
    }

    wordList_.clear(); 
    std::string line;

    while (std::getline(file, line)) { 
        size_t spacePos = line.find(' ');
        if (spacePos != std::string::npos) {
            std::string word = line.substr(0, spacePos);
            std::string category = line.substr(spacePos + 1);
            wordList_.push_back({ word, category });
        }
    }

    file.close();
    std::cout << "Total words loaded: " << wordList_.size() << std::endl;
    return !wordList_.empty(); 
}

void Game::startNewGame() {
    if (wordList_.empty()) {
        std::cerr << "Word list is empty!" << std::endl;
        return;
    }

    srand(time(nullptr));
    int randomIndex = rand() % wordList_.size();
    currentWord_ = wordList_[randomIndex].first;
    currentCategory_ = wordList_[randomIndex].second;

    size_t wordLength = currentWord_.length() / 2; 

    std::cout << "--- NEW GAME ---" << std::endl;
    std::cout << "Word: '" << currentWord_ << "'" << std::endl;
    std::cout << "Category: '" << currentCategory_ << "'" << std::endl;
    std::cout << "Word length: " << wordLength << " letters" << std::endl;

    wordProgress_.clear();
    wordProgress_.resize(wordLength, false); 

    wrongGuesses_ = 0;

    if (gameHandler_) {
        gameHandler_->resetLetters(); 
    }
}

void Game::checkLetterInWord(int letterIndex) {
    std::string letter = gameHandler_->getLetter(letterIndex); 
    bool found = false; 

    size_t wordLength = currentWord_.length() / 2; 

    for (size_t i = 0; i < wordLength; i++) {
        std::string wordLetter = currentWord_.substr(i * 2, 2);

        if (wordLetter == letter && !wordProgress_[i]) {
            wordProgress_[i] = true;
            found = true;
            std::cout << "  Match at position " << i << std::endl;
        }
    }

    if (!found) {
        wrongGuesses_++;
        std::cout << "Wrong! Errors: " << wrongGuesses_ << "/" << maxWrongGuesses_ << std::endl;
    }
    else {
        std::cout << "Correct!" << std::endl;
    }

    bool won = true;
    for (bool b : wordProgress_) {
        if (!b) {
            won = false;
            break;
        }
    }

    if (won) {
        std::cout << "YOU WIN! Word was: " << currentWord_ << std::endl;
        gameHandler_->enterWinDialog(); 
        return;
    }

    if (wrongGuesses_ >= maxWrongGuesses_) {
        std::cout << "GAME OVER! Word was: " << currentWord_ << std::endl;
        gameHandler_->enterLoseDialog();  
    }
}

void Game::renderHangman() {
    if (wrongGuesses_ >= 1) {
        TextureManager::Instance().draw("hangman_head", 0, 0, 720, 960, renderer_);
    }
    
    if (wrongGuesses_ >= 2) {
        TextureManager::Instance().draw("hangman_body", 0, 0, 720, 960, renderer_);
    }
    
    if (wrongGuesses_ >= 3) {
        TextureManager::Instance().draw("hangman_left_arm", 0, 0, 720, 960, renderer_);
    }
    
    if (wrongGuesses_ >= 4) {
        TextureManager::Instance().draw("hangman_right_arm", 0, 0, 720, 960, renderer_);
    }
    
    if (wrongGuesses_ >= 5) {
        TextureManager::Instance().draw("hangman_right_leg", 0, 0, 720, 960, renderer_);
    }
    
    if (wrongGuesses_ >= 6) {
        TextureManager::Instance().draw("hangman_left_leg", 0, 0, 720, 960, renderer_);
    }
}

void Game::renderWordProgress() {

    const int SCREEN_WIDTH = 720;

    std::string categoryText = u8"Категория: " + currentCategory_;
    SDL_Color categoryColor = { 100, 100, 150, 255 };
    float categoryX = TextureManager::Instance().getCenteredX(categoryText, "main_font", categoryColor, renderer_, SCREEN_WIDTH); 
    TextureManager::Instance().drawText(categoryText, "main_font", categoryX, 80, categoryColor, renderer_);

    std::string displayWord = "";
    size_t wordLength = currentWord_.length() / 2; 

    for (size_t i = 0; i < wordLength; i++) {
        if (wordProgress_[i]) { 
            displayWord += currentWord_.substr(i * 2, 2);
            displayWord += " ";
        }
        else {
            displayWord += "_ ";
        }
    }

    SDL_Color wordColor = { 0, 0, 0, 255 };
    float wordX = TextureManager::Instance().getCenteredX(displayWord, "main_font", wordColor, renderer_, SCREEN_WIDTH);
    TextureManager::Instance().drawText(displayWord, "main_font", wordX, 625, wordColor, renderer_);
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    if (currentState_ == STATE_MENU) {
        TextureManager::Instance().draw("menu_bg", 0, 0, 720, 960, renderer_);
        volumeIcon_.draw(renderer_);
        helpIcon_.draw(renderer_);

        if (menuHandler_->isConfirmMode) {
            confirmDialogBg_.draw(renderer_);
            confirmButton_.draw(renderer_);
            cancelButton_.draw(renderer_);

        }
        else if (menuHandler_->isHelpMode) {  
            helpDialogBg_.draw(renderer_);
            IseeButton_.draw(renderer_);

            renderHelpText();
        }
        else if (menuHandler_->isSoundMode) { 
            soundDialogBg_.draw(renderer_);
            soundOkButton_.draw(renderer_);
            renderVolumeSlider();
        }
        else {
            playButton_.draw(renderer_);
            exitButton_.draw(renderer_);
        }
    }
    else if (currentState_ == STATE_GAME) {
        TextureManager::Instance().draw("game_bg", 0, 0, 720, 960, renderer_);

        renderHangman();

        volumeIcon_.draw(renderer_);
        helpIcon_.draw(renderer_);
        backToMenuIcon_.draw(renderer_);

        renderWordProgress();
        renderKeyboard();

        if (gameHandler_->isWinDialogMode) { 
            winDialogBg_.draw(renderer_);
            newGameButton_.draw(renderer_);
            menuButton_.draw(renderer_);

            renderWinDialogText();
        }

        else if (gameHandler_->isLoseDialogMode) { 
            loseDialogBg_.draw(renderer_);
            newGameButton_.draw(renderer_);
            menuButton_.draw(renderer_);

            renderLoseDialogText();
        }

        else if (gameHandler_->isHelpMode) { 
            helpDialogBg_.draw(renderer_);
            IseeButton_.draw(renderer_);

            renderHelpText();
        }

        else if (gameHandler_->isSoundMode) { 
            soundDialogBg_.draw(renderer_);
            soundOkButton_.draw(renderer_);

            renderVolumeSlider();
        }
    }

    SDL_RenderPresent(renderer_);
}

void Game::update() {
    if (currentState_ == STATE_MENU && menuHandler_) {
        setMusicVolume(menuHandler_->getVolumeLevel());  
    }
    else if (currentState_ == STATE_GAME && gameHandler_) {
        setMusicVolume(gameHandler_->getVolumeLevel()); 
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
            if (currentState_ == STATE_MENU && menuHandler_) {
                menuHandler_->handle(event);
            }
            else if (currentState_ == STATE_GAME && gameHandler_) {
                gameHandler_->handle(event); 

                if (gameHandler_->isWinDialogMode) {
                    if (gameHandler_->newGameFromWin) {
                        std::cout << "Starting new game from win dialog" << std::endl;
                        gameHandler_->exitWinDialog();
                        gameHandler_->resetDialogFlags();
                        startNewGame();
                        gameHandler_->clearLastPressedLetter();
                    }
                    else if (gameHandler_->backToMenuFromWin) {
                        std::cout << "Returning to menu from win dialog" << std::endl;
                        gameHandler_->exitWinDialog();
                        gameHandler_->resetDialogFlags();
                        gameHandler_->resetGameFlags();
                        menuHandler_->setVolumeLevel(gameHandler_->getVolumeLevel());
                        startNewGame();
                        gameHandler_->clearLastPressedLetter();
                        currentState_ = STATE_MENU;
                    }
                    continue;
                }

                if (gameHandler_->isLoseDialogMode) {
                    if (gameHandler_->newGameFromLose) {
                        std::cout << "Starting new game from lose dialog" << std::endl;
                        gameHandler_->exitLoseDialog();
                        gameHandler_->resetDialogFlags();
                        startNewGame();
                        gameHandler_->clearLastPressedLetter();
                    }
                    else if (gameHandler_->backToMenuFromLose) {
                        std::cout << "Returning to menu from lose dialog" << std::endl;
                        gameHandler_->exitLoseDialog();
                        gameHandler_->resetDialogFlags();
                        gameHandler_->resetGameFlags();
                        menuHandler_->setVolumeLevel(gameHandler_->getVolumeLevel());
                        startNewGame();
                        gameHandler_->clearLastPressedLetter();
                        currentState_ = STATE_MENU;
                    }
                    continue;
                }

                if (gameHandler_->backToMenuClicked) {
                    std::cout << "Returning to main menu from game" << std::endl;
                    menuHandler_->setVolumeLevel(gameHandler_->getVolumeLevel());  
                    currentState_ = STATE_MENU;
                    gameHandler_->resetGameFlags(); 

                    if (gameHandler_) {
                        gameHandler_->resetLetters();
                        gameHandler_->clearLastPressedLetter();
                    }
                    continue; 
                }

                int pressedLetter = gameHandler_->getLastPressedLetter();
                if (pressedLetter != -1) {
                    checkLetterInWord(pressedLetter);
                    gameHandler_->clearLastPressedLetter();
                }
            }
        }
    }

    if (currentState_ == STATE_MENU && menuHandler_) {
        if (menuHandler_->isConfirmMode) {
            if (menuHandler_->confirmExitConfirmed) {
                stopGame();  
            }
            if (menuHandler_->confirmExitCancelled) {
                menuHandler_->exitConfirmMode();  
            }
        }
        else if (menuHandler_->isHelpMode) {  
            if (menuHandler_->helpConfirmed) {
                menuHandler_->exitHelpMode();
            }
        }
        else if (menuHandler_->isSoundMode) { 
            if (menuHandler_->soundConfirmed) {
                menuHandler_->exitSoundMode();  
            }
        }
        else {
            if (menuHandler_->playClicked) {
                std::cout << "Play button clicked! Switching to game..." << std::endl;
                gameHandler_->setVolumeLevel(menuHandler_->getVolumeLevel()); 
                currentState_ = STATE_GAME;
                menuHandler_->resetFlags();
            }
            if (menuHandler_->exitClicked) {
                std::cout << "Exit button clicked! Quitting..." << std::endl;
                stopGame();
            }
        }
    }
    else if (currentState_ == STATE_GAME && gameHandler_) {
        if (gameHandler_->isHelpMode) {
            if (gameHandler_->helpConfirmed) {
                gameHandler_->exitHelpMode();
            }
        }
        else if (gameHandler_->isSoundMode) {
            if (gameHandler_->soundConfirmed) {
                gameHandler_->exitSoundMode();
            }
        }
    }
}

void Game::clean() {
    std::cout << "Cleaning up..." << std::endl;

    stopMusic();

    if (mixer_) {
        MIX_DestroyMixer(mixer_);
        mixer_ = nullptr;
    }

    MIX_Quit();

    TextureManager::Instance().cleanupTTF();

    delete menuHandler_;
    delete gameHandler_;
    menuHandler_ = nullptr;
    gameHandler_ = nullptr;
    currentHandler_ = nullptr;

    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    if (sdlInitialized_) {
        SDL_Quit();
    }
}