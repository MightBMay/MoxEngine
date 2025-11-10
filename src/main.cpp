#include "pch.h"

//int main()
//{
//    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
//    window.setFramerateLimit(144);
//
//
//    auto test = GameObject({ 960, 540 });
//    test.setRenderer<CircleRenderer>(64);
//
//    sf::Clock dtClock;
//
//    while (window.isOpen())
//    {
//
//        sf::Time dt = dtClock.restart();
//
//        window.clear();
//        while (const std::optional event = window.pollEvent())
//        {
//            if (event->is<sf::Event::Closed>())
//            {
//                window.close();
//            }
//        }
//
//       
//        test.Update(dt.asSeconds());
//        window.draw(test, sf::RenderStates::Default);
//        window.display();
//    }
//}
