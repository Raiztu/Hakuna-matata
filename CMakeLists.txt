cmake_minimum_required(VERSION 3.16)
project(Project_parf VERSION 1.0.0 LANGUAGES CXX)

# Устанавливаем стандарт C++20 
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# Находим пакеты SDL3 
find_package(SDL3 REQUIRED CONFIG)
find_package(SDL3_image REQUIRED CONFIG)
find_package(SDL3_ttf REQUIRED CONFIG)
find_package(SDL3_mixer REQUIRED CONFIG)

# Добавляем исполняемый файл 
add_executable(Project_parf WIN32
    main.cpp
    Game.cpp
    Game.hpp
    GameInputHandler.cpp
    GameInputHandler.hpp
    GameObject.cpp
    GameObject.hpp
    InputHandler.cpp
    InputHandler.hpp
    MenuInputHandler.cpp
    MenuInputHandler.hpp
    Player.cpp
    Player.hpp
    TextureManager.cpp
    TextureManager.hpp
    Vector2D.hpp
)

# Подключаем библиотеки SDL3
target_link_libraries(Project_parf PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer
)

# Указываем, что нужно использовать для русского текста
if(MSVC)
    target_compile_options(Project_parf PRIVATE "/utf-8")
endif()

# Копирование DLL в папку с исполняемым файлом 
if(WIN32)
    add_custom_command(TARGET Project_parf POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3::SDL3>
            $<TARGET_FILE_DIR:Project_parf>
        COMMENT "Copying SDL3.dll..."
    )
    add_custom_command(TARGET Project_parf POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_image::SDL3_image>
            $<TARGET_FILE_DIR:Project_parf>
        COMMENT "Copying SDL3_image.dll..."
    )
    add_custom_command(TARGET Project_parf POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_ttf::SDL3_ttf>
            $<TARGET_FILE_DIR:Project_parf>
        COMMENT "Copying SDL3_ttf.dll..."
    )
    add_custom_command(TARGET Project_parf POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:SDL3_mixer::SDL3_mixer>
            $<TARGET_FILE_DIR:Project_parf>
        COMMENT "Copying SDL3_mixer.dll..."
    )
endif()

# Копирование папки assets в выходную директорию
add_custom_command(TARGET Project_parf POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:Project_parf>/assets
    COMMENT "Copying assets folder..."
)

# Настройка рабочей директории для отладки в Visual Studio
set_property(TARGET Project_parf PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:Project_parf>)

# Генерация документации через Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
    # Опционально: сделать цель docs зависимостью от сборки
else()
    message(STATUS "Doxygen not found or Doxyfile missing – documentation target not added")
endif()