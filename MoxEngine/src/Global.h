#pragma once
#include "pch.h"
class Scene;
class LDTK_Manager;
class Timer;

extern sf::RenderWindow window;
extern sf::View playerView;
extern sf::View defaultView;

extern std::unique_ptr<Scene> scene;

extern std::unique_ptr<Timer> secondTimer;
