// MOXEngineGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "pch.h"
#include "PlayerMovement.h"
#include "Event.h"

static sf::RenderWindow window;


int main()
{
    window = sf::RenderWindow(
        sf::VideoMode({ 1920u, 1080u }), 
        "CMake SFML Project");
    window.setFramerateLimit(144);


    GameObject* test = new GameObject({ 960, 540 });
    test->setRenderer<CircleRenderer>(test->_transform.get(), 64);
    test->addComponent<Button>();

    sf::Clock dtClock;

    while (window.isOpen())
    {
        sf::Time dt = dtClock.restart();
        float deltaTime = dt.asSeconds();
        window.clear();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }


        test->Update(deltaTime);
        window.draw(*test, sf::RenderStates::Default);
        window.display();
    }
}
