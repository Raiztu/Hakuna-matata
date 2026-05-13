#include "TextureManager.hpp"

#include <SDL3/SDL_rect.h>
#include <SDL3_image/SDL_image.h>


#include <iostream>

bool TextureManager::load(std::string fileName, std::string tag,
    SDL_Renderer* renderer) {
    SDL_Surface* tmpSurface = IMG_Load(fileName.c_str());
    if (tmpSurface == 0) {
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tmpSurface); 
    SDL_DestroySurface(tmpSurface);

    if (texture != 0) {
        textureMap_[tag] = texture;
        return true;
    }
    return false;
}


void TextureManager::draw(std::string tag, float x, float y, float width,
    float height, SDL_Renderer* renderer,
    SDL_FlipMode flip) {
    SDL_FRect srcRect;
    SDL_FRect dstRect;

    srcRect.x = srcRect.y = 0;
    srcRect.w = dstRect.w = width;
    srcRect.h = dstRect.h = height;
    dstRect.x = x;
    dstRect.y = y;


    SDL_RenderTextureRotated(renderer, textureMap_[tag], &srcRect, &dstRect, 0,
        NULL, flip);
}


void TextureManager::drawFrame(std::string tag, float x, float y, float width,
    float height, int currentRow, int currentFrame,
    SDL_Renderer* renderer, SDL_FlipMode flip) {
    SDL_FRect srcRect;
    SDL_FRect dstRect;


    srcRect.x = width * currentFrame;
    srcRect.y = height * (currentRow - 1);
    srcRect.w = dstRect.w = width;
    srcRect.h = dstRect.h = height;
    dstRect.x = x;
    dstRect.y = y;


    SDL_RenderTextureRotated(renderer, textureMap_[tag], &srcRect, &dstRect, 0,
        NULL, flip);
}

// Для работы с текстом
bool TextureManager::loadFont(std::string fontPath, std::string fontTag, int fontSize) {
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (font == nullptr) {
        std::cerr << "Failed to load font" << std::endl;
        return false;
    }
    fontMap_[fontTag] = font;
    if (defaultFont_ == nullptr) {
        defaultFont_ = font;
    }
    return true;
}

SDL_Texture* TextureManager::createTextTexture(const std::string& text, const std::string& fontTag,
    SDL_Color color, SDL_Renderer* renderer) {
    TTF_Font* font = defaultFont_; //запасной вариант
    if (fontMap_.find(fontTag) != fontMap_.end()) { 
        font = fontMap_[fontTag]; // меняем, если нашли
    }

    if (font == nullptr) {
        std::cerr << "No font available for text rendering" << std::endl;
        return nullptr;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), 0, color); // Рендеринг текста в поверхность
    if (surface == nullptr) {
        std::cerr << "Failed to create text surface" << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); // Преобразование поверхности в текстуру
    SDL_DestroySurface(surface); // Больше не нужна

    return texture;
}

void TextureManager::drawText(const std::string& text, const std::string& fontTag,
    float x, float y, SDL_Color color, SDL_Renderer* renderer) {
    SDL_Texture* textTexture = createTextTexture(text, fontTag, color, renderer);
    if (textTexture != nullptr) {
        float texW, texH;
        SDL_GetTextureSize(textTexture, &texW, &texH); // записываем реальные ширину и высоту

        SDL_FRect dstRect; // Структура для хранения прямоугольника (область экрана)
        dstRect.x = x;
        dstRect.y = y;
        dstRect.w = texW;
        dstRect.h = texH;

        SDL_RenderTexture(renderer, textTexture, nullptr, &dstRect); // Отрисовка
        SDL_DestroyTexture(textTexture); // Временную текстуру уничтожаем
    }
}

void TextureManager::drawTextWrapped(const std::string& text, const std::string& fontTag,
    float x, float y, int wrapWidth, SDL_Color color,
    SDL_Renderer* renderer) {
    TTF_Font* font = defaultFont_;
    if (fontMap_.find(fontTag) != fontMap_.end()) {
        font = fontMap_[fontTag];
    }

    if (font == nullptr) {
        std::cerr << "No font available for text rendering" << std::endl;
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), 0, color, wrapWidth);
    if (surface == nullptr) {
        std::cerr << "Failed to create wrapped text surface" << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);

    if (textTexture != nullptr) {
        float texW, texH;
        SDL_GetTextureSize(textTexture, &texW, &texH);

        SDL_FRect dstRect;
        dstRect.x = x;
        dstRect.y = y;
        dstRect.w = texW;
        dstRect.h = texH;

        SDL_RenderTexture(renderer, textTexture, nullptr, &dstRect);
        SDL_DestroyTexture(textTexture);
    }
}

bool TextureManager::initTTF() {
    if (TTF_Init() < 0) {
        std::cerr << "TTF_Init error" << std::endl;
        return false;
    }
    return true;
}

void TextureManager::cleanupTTF() {
    for (auto& pair : fontMap_) {
        if (pair.second != nullptr) {
            TTF_CloseFont(pair.second); // Очищает память, закрывает файл шрифта и теперь pair.second недействительный
        }
    }
    fontMap_.clear(); // Удаляет все элементы из map
    defaultFont_ = nullptr;
    TTF_Quit();
}