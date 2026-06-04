#pragma once  

#include <cmath> 

/// @brief Класс для работы с двухмерными векторами (точками на плоскости)
class Vector2D {
private:
    float x_;  ///< Координата X
    float y_;  ///< Координата Y

public:
    /// @brief Конструктор, инициализирующий вектор заданными координатами
    /// @param x Начальное значение координаты X
    /// @param y Начальное значение координаты Y
    Vector2D(float x, float y) : x_(x), y_(y) {}

    /// @brief Возвращает координату X
    /// @return Значение координаты X
    float get_x() { return x_; }

    /// @brief Возвращает координату Y
    /// @return Значение координаты Y
    float get_y() { return y_; }

    /// @brief Устанавливает новое значение координаты X
    /// @param new_x Новое значение координаты X
    void set_x(float new_x) { x_ = new_x; }

    /// @brief Устанавливает новое значение координаты Y
    /// @param new_y Новое значение координаты Y
    void set_y(float new_y) { y_ = new_y; }

    /// @brief Вычисляет длину вектора по теореме Пифагора
    /// @return Длина вектора = sqrt(x² + y²)
    float length() { return sqrt(x_ * x_ + y_ * y_); }
};