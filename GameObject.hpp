#pragma once

#include <SDL3/SDL.h>

#include <iostream>

/// @brief Базовый класс для игровых объектов (кнопки, фон и т.д.)
class GameObject {
protected:
	std::string texture_tag_; ///< Тег текстуры (для TextureManager
	int currentFrame_; ///< Текущий кадр анимации
	int currentRow_; ///< Текущая строка в спрайт-листе
	float x_, y_; ///< Координаты верхнего левого угла
	float w_, h_; ///< Ширина и высота объекта

public:
	/// @brief Загружает параметры объекта (текстура, позиция, размер)
	/// @param tag Тег текстуры (должна быть загружена в TextureManager)
	/// @param x Координата X
	/// @param y Координата Y
	/// @param w Ширина
	/// @param h Ширина
	void load(std::string tag, float x, float y, float w, float h);
	/// @brief Отрисовывает объект через TextureManager
	/// @param renderer Рендерер SDL
	void draw(SDL_Renderer* renderer);
	/// @brief Обновляет состояние объекта (например, смену кадров анимации)
	void update();
	/// @brief Очищает ресурсы (выводит сообщение в консоль)
	void clean();
};