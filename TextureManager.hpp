#pragma once

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <map>
#include <string>

/// @brief Менеджер текстур и шрифтов.
/// Отвечает за загрузку, хранение и отрисовку изображений.
class TextureManager {
private:
    TextureManager() = default;
    TextureManager(const TextureManager&) = delete;
    TextureManager& operator=(const TextureManager&) = delete;
    TextureManager(TextureManager&&) = delete;
    TextureManager& operator=(TextureManager&&) = delete;

    std::map<std::string, SDL_Texture*> textureMap_; ///< Хранилище текстуры

    std::map<std::string, TTF_Font*> fontMap_; ///<  Хранилище шрифта (ключ - тег шрифта, значение - указатель на шрифт)
    TTF_Font* defaultFont_ = nullptr;  ///< Шрифт по умолчанию (первый загруженный)

public:
    /// @brief Загружает текстуру из файла и сохраняет её под указанным тегом
    /// @param fileName Путь к файлу изображения
    /// @param tag Уникальный строковый идентификатор для доступа к текстуре
    /// @param renderer Рендерер SDL, используемый для создания текстуры
    /// @return true, если загрузка успешна, иначе false
    bool load(std::string fileName, std::string tag, SDL_Renderer* renderer);
    /// @brief Отрисовывает текстуру целиком 
    /// @param tag Тег ранее загруженной текстуры
    /// @param x Координата X верхнего левого угла на экране
    /// @param y Координата Y верхнего левого угла на экране
    /// @param width Ширина отрисовки
    /// @param height Высота отрисовки
    /// @param renderer Рендерер SDL
    /// @param flip Отразить текстуру по горизонтали/вертикали
    void draw(std::string tag, float x, float y, float width, float height,
        SDL_Renderer* renderer, SDL_FlipMode flip = SDL_FLIP_NONE);
    /// @brief Отрисовывает один кадр из текстурного атласа
    /// @param tag Тег текстуры-атласа
    /// @param x Координата X на экране
    /// @param y Координата Y на экране
    /// @param width Ширина одного кадра
    /// @param height Высота одного кадра
    /// @param currentRow Номер строки в атласе (начиная с 1)
    /// @param currentFrame Номер кадра в строке (начиная с 0)
    /// @param renderer Рендерер SDL
    /// @param flip Отразить кадр
    void drawFrame(std::string tag, float x, float y, float width, float height,
        int currentRow, int currentFrame, SDL_Renderer* renderer,
        SDL_FlipMode flip = SDL_FLIP_NONE);
    /// @brief Возвращает единственный экземпляр менеджера
    /// @return Ссылка на статический объект TextureManager
    static TextureManager& Instance() {
        static TextureManager Instance_;
        return Instance_;
    }

    /// @brief Загружает шрифт из файла и сохраняет под указанным тегом
    /// @param fontPath Путь к файлу шрифта
    /// @param fontTag Уникальный идентификатор шрифта
    /// @param fontSize Размер шрифта в пунктах
    /// @return true, если загрузка успешна, иначе false
    bool loadFont(std::string fontPath, std::string fontTag, int fontSize);
    /// @brief Создаёт временную текстуру из строки текста (не отрисовывает)
    /// @param text Текст в кодировке UTF-8
    /// @param fontTag Тег шрифта
    /// @param color Цвет текста
    /// @param renderer Рендерер SDL
    /// @return Указатель на текстуру или nullptr при ошибке. Вызывающий должен уничтожить текстуру через SDL_DestroyTexture
    SDL_Texture* createTextTexture(const std::string& text, const std::string& fontTag, SDL_Color color, SDL_Renderer* renderer); 
    /// @brief Вычисляет X-координату, необходимую для центрирования текста по горизонтали
    /// @param text Строка текста
    /// @param fontTag Тег шрифта
    /// @param color Цвет текста (влияет на расчёт ширины через рендеринг)
    /// @param renderer Рендер
    /// @param screenWidth Ширина области (обычно ширина окна)
    /// @return X-координата левого края текста, при которой текст будет расположен по центру
    float getCenteredX(const std::string& text, const std::string& fontTag, SDL_Color color, SDL_Renderer* renderer, float screenWidth); 
    /// @brief Отрисовывает однострочный текст в заданной позиции
    /// @param text Текст UTF-8
    /// @param fontTag Тег шрифта
    /// @param x Координата X левого верхнего угла
    /// @param y Координата Y левого верхнего угла
    /// @param color Цвет текста
    /// @param renderer Рендер
    void drawText(const std::string& text, const std::string& fontTag, float x, float y, SDL_Color color, SDL_Renderer* renderer);  
    /// @brief Отрисовывает текст с автоматическим переносом строк в пределах wrapWidth
    /// @param text Текст UTF-8 (перенос строки можно задать явно символами \n)
    /// @param fontTag Тег шрифта
    /// @param x Координата X
    /// @param y Координата Y
    /// @param wrapWidth Максимальная ширина текста в пикселях, после которой будет выполнен перенос
    /// @param color Цвет текста
    /// @param renderer Рендер
    void drawTextWrapped(const std::string& text, const std::string& fontTag, float x, float y, int wrapWidth, SDL_Color color, SDL_Renderer* renderer);
    
    /// @brief Инициализирует библиотеку SDL_ttf. Должен быть вызван до использования шрифтов
    /// @return true, если инициализация успешна, иначе false
    bool initTTF();
    /// @brief Освобождает все загруженные шрифты и завершает работу SDL_ttf
    void cleanupTTF();

};