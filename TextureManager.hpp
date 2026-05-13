#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <string>

class TextureManager {
private:
    TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager(TextureManager&&) = delete;
    TextureManager& operator=(TextureManager&&) = delete;

    std::map<std::string, SDL_Texture*> textureMap_;

    // Для работы со шрифтами
    std::map<std::string, TTF_Font*> fontMap_; // Хранилище шрифта (ключ - тег шрифта, значение - указатель на шрифт)
    TTF_Font* defaultFont_ = nullptr;  // Указатель на шрифт по умолчанию (используется в случае, если при вызове указан несуществующий)

public:
    bool load(std::string fileName, std::string tag, SDL_Renderer* renderer);
    void draw(std::string tag, float x, float y, float width, float height,
        SDL_Renderer* renderer, SDL_FlipMode flip = SDL_FLIP_NONE);
    void drawFrame(std::string tag, float x, float y, float width, float height,
        int currentRow, int currentFrame, SDL_Renderer* renderer,
        SDL_FlipMode flip = SDL_FLIP_NONE);
    static TextureManager& Instance() {
        static TextureManager Instance_;
        return Instance_;
    }

    // Для работы с текстом
    bool loadFont(std::string fontPath, std::string fontTag, int fontSize);
    SDL_Texture* createTextTexture(const std::string& text, const std::string& fontTag, SDL_Color color, SDL_Renderer* renderer); // Создаёт текстуру из текста (временная операция)
    void drawText(const std::string& text, const std::string& fontTag, float x, float y, SDL_Color color, SDL_Renderer* renderer);  // Рисует однострочный текст на экране 
    void drawTextWrapped(const std::string& text, const std::string& fontTag, float x, float y, int wrapWidth, SDL_Color color, SDL_Renderer* renderer); // Рисует текст с автоматическим переносом строк
    
    // Инициализация и очистка
    bool initTTF();
    void cleanupTTF();

};