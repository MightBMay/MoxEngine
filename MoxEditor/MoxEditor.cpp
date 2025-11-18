#pragma comment(lib, "Advapi32.lib")
#include <iostream>



#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_internal.h"
#include "config.h"
#include <fstream>
#include "json.hpp"
#include "SFML/Graphics.hpp"

#include "SceneLoader.h"

#include "ComponentImport.h"

#include "CircleRenderer.h"
#include "RectRenderer.h"
#include "SpriteRenderer.h"
#include "TileMapRenderer.h"


#if IN_EDITOR

#include "GUI_CreateGameObject.h"
#include "SceneHeirarchy.h"
#include "GUI_Manager.h"
#include "GUI_Inspector.h"

#endif



using json = nlohmann::json;

sf::RenderWindow window;
sf::View playerView;
sf::View defaultView;

std::unique_ptr<Scene> scene;

#if IN_EDITOR
void DrawGUI(Scene& scene) {
    GUI_CreateGameObject::instance().Draw(scene);
    GUI_SceneHierarchy::instance().Draw(scene);
    GUI_Manager::instance().Draw();
    GUI_Inspector::instance().Draw();

}

#endif



int main()
{
#pragma region pre loop

    Input::Initialize();

    playerView = sf::View({ 0,0 }, { 1920,1080 });
    window = sf::RenderWindow(sf::VideoMode({ 1920, 1080 }), "MoxEditor");
    defaultView = window.getDefaultView();
   
   
    window.setFramerateLimit(60);
    

#if IN_EDITOR
    ImGui::SFML::Init(window);
#endif
    sf::Clock deltaClock;
    bool sceneLoaded = false;


    scene = SceneLoader::LoadSceneFromPath("../assets/scenes/testing.scene");
    if (!scene) scene = std::make_unique<Scene>();




#pragma endregion

    while (window.isOpen()) {
        sf::Time dt = deltaClock.restart();
        float deltaTime = dt.asSeconds();
        Input::Update();


        window.clear(sf::Color(40,40,40));
#if IN_EDITOR
        ImGui::SFML::Update(window, dt);
#endif

        while (const std::optional event = window.pollEvent()) {
            if (!event.has_value()) continue;
            Input::HandleEvent(event);
            const sf::Event& _event = *event;
#if IN_EDITOR
            ImGui::SFML::ProcessEvent(window, _event);
#endif
            if (event->is<sf::Event::Closed>()) window.close();

        }
#if IN_EDITOR
        DrawGUI(*scene);
#endif
        
        scene->Update(deltaTime);
        window.setView(playerView);
        scene->Draw();

        
        
#if IN_EDITOR
        ImGui::SFML::Render(window);
#endif
        window.display();
    }

#if IN_EDITOR
    ImGui::SFML::Shutdown();
    scene->Save();
#endif
    return 0;

}

