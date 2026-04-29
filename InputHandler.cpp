#include "InputHandler.hpp"

// Конструктор 
InputHandler::InputHandler() {
    // Инициализируем массив состояний кнопок мыши 
    for (int i = 0; i < 5; i++) {
        mouseStates_.push_back(false);
    }
    // Создаём объект позиции мыши, начинаем с координат (0, 0)
    mousePosition_ = new Vector2D(0, 0);
}

// Возвращает указатель на объект с текущей позицией мыши
Vector2D* InputHandler::getMousePosition() {
    return mousePosition_;
}

// Возвращает состояние указанной кнопки мыши
bool InputHandler::getMouseButtonState(int buttonNumber) {
    return mouseStates_[buttonNumber];  
}

// Проверяет, нажата ли конкретная клавиша
bool InputHandler::isKeyDown(SDL_Scancode key) {
    if (keystates_ != 0) {  // Если массив состояний существует
        return keystates_[key];  // Если клавиша нажата
    }
    return false;  
}

// Главный диспетчер событий 
void InputHandler::handle(SDL_Event event) {
    keystates_ = SDL_GetKeyboardState(nullptr);

    switch (event.type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:  // Нажата кнопка мыши
        onMouseButtonDown(event);
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:    // Отпущена кнопка мыши
        onMouseButtonUp(event);
        break;
    case SDL_EVENT_MOUSE_MOTION:       // Мышь переместилась
        onMouseMotion(event);
        break;
    case SDL_EVENT_KEY_DOWN:           // Нажата клавиша
        onKeyDown(event);
        break;
    default:  // Другие события (движение окна, отпускание клавиш и т.д.) игнорируем
        break;
    }
}