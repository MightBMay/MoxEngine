#pragma comment(lib, "Advapi32.lib")
#include <iostream>



#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_internal.h"


#include "json.hpp"
#include "SFML/Graphics.hpp"
#include <fstream>
#include "SceneLoader.h"
#include "CircleRenderer.h"
#include "RectRenderer.h"
#include "PlayerMovement.h"
#include "Button.h"

#include "CreateGameObject.h"
#include "SceneHeirarchy.h"
#include "GUI_Manager.h"
#include "GUI_Inspector.h"

using json = nlohmann::json;

sf::RenderWindow window;
sf::View playerView;

std::unique_ptr<Scene> scene;
Scene* curScene;

void DrawGUI(Scene& scene) {
    GUI_CreateGameObject::instance().Draw(scene);
    GUI_SceneHierarchy::instance().Draw(scene);
    GUI_Manager::instance().Draw();
    GUI_Inspector::instance().Draw();

}



int main()
{
#pragma region pre loop

    Input::Initialize();

    playerView = sf::View({ 0,0 }, { 1920,1080 });
    window = sf::RenderWindow(sf::VideoMode({ 1920, 1080 }), "MoxEditor");
    window.setView(playerView);
    window.setFramerateLimit(60);
    
    ImGui::SFML::Init(window);
    sf::Clock deltaClock;
    bool sceneLoaded = false;
    std::ifstream file("../assets/scenes/testing.scene");
    if (!file.is_open()) {
        std::cerr << "failed to open scene \n";
    }

    json data;
    try {
        file >> data;
        sceneLoaded = true;
    }
    catch (const json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
    }
 
    if (sceneLoaded) scene = LoadScene(data);
    else scene = std::make_unique<Scene>();
    curScene = scene.get();



#pragma endregion

    while (window.isOpen()) {
        sf::Time dt = deltaClock.restart();
        float deltaTime = dt.asSeconds();
        Input::Update();


        window.clear(sf::Color(40,40,40));
        ImGui::SFML::Update(window, dt);

        while (const std::optional event = window.pollEvent()) {
            if (!event.has_value()) continue;
            Input::HandleEvent(event);
            const sf::Event& _event = *event;
            ImGui::SFML::ProcessEvent(window, _event);
            if (event->is<sf::Event::Closed>()) window.close();

        }

        DrawGUI(*curScene);

        if (Input::GetKeyDown(sf::Keyboard::Scan::Down)) {
            curScene->SaveToFile("testing.scene");
        }

        scene->Update(deltaTime);
        scene->Draw();
        

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;

}

