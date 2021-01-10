// yEngine v.0.0.0 - beta
// Данный файл соединяет стандартные заголовочные файлы..
#include <iostream>

// ..заголовочные файлы SFML
#include <SFML/Graphics.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Audio/Sound.hpp>

// ..и заголовочные файлы движка
#include "load.h" // перегружает операторы векторов и загружает шрифты
#include "inputs.h" // содержит класс input
#include "world.h" // содежит класс мир, в котором собрана информация обо всех объектах внутри него и просчитывается физика для них
#include "graphics.h" // загружает шейдеры и содержит функции 

// ..тем самым образуя фреймворк yEngine
