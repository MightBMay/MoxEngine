#include "CreateGameObject.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "GUI_Action.h"
#include "GUI_Manager.h"


void GUI_CreateGameObject::Draw(Scene& scene) {
    if (!_isVisible) return;
    ImGui::Begin("Create GameObject", &_isVisible,
        ImGuiWindowFlags_AlwaysAutoResize);
    
   
    ImGui::InputText("Name", name, 64);
    ImGui::SeparatorText("Transform");
    ImGui::InputFloat2("Position", pos);
    ImGui::InputFloat2("Scale", scale);
    ImGui::InputFloat("Rotation", &rot);
#pragma region  Renderer



    ImGui::SeparatorText("Renderer");

    // Renderer dropdown
    std::vector<std::string> rendererTypes = RendererFactory::instance().GetTypes();
    rendererTypes.insert(rendererTypes.begin(), "None"); // Add "None" as first option

    std::vector<const char*> rendererOptions;
    rendererOptions.reserve(rendererTypes.size());
    for (auto& s : rendererTypes) rendererOptions.push_back(s.c_str());

    if (!rendererOptions.empty()) {
        if (ImGui::Combo("Renderer Type", &selectedRendererIndex, rendererOptions.data(), (int)rendererOptions.size())) {
            if (selectedRendererIndex != previousRendererIndex) {
                rendererData = nlohmann::json::object();
                previousRendererIndex = selectedRendererIndex;
            }
        }
    }

    // Renderer-specific input fields
    if (!rendererOptions.empty()) {
        std::string selectedRenderer = rendererOptions[selectedRendererIndex];

        if (selectedRenderer != "None") { // Only show renderer params if one is chosen
            try {
                auto tempRenderer = RendererFactory::instance().Create(selectedRenderer, rendererData);
                tempRenderer->getImGuiParams(rendererData);
            }
            catch (const std::exception& e) {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error creating renderer: %s", e.what());
            }
        }
        else {
            ImGui::TextDisabled("No renderer selected.");
        }
    }
#pragma endregion

#pragma region  Component
    ImGui::SeparatorText("Components");

    // Show Add Component button
    if (ImGui::Button("Add Component")) {
        addingComponent = true;
        componentData = nlohmann::json::object(); // reset previous data
    }

    // If we're in the process of adding a component
    if (addingComponent) {
        ImGui::Indent(20); // visually offset the add-component UI

        // Component type dropdown
        const std::vector<std::string> componentTypes = ComponentFactory::instance().GetTypes();
        std::vector<const char*> componentOptions;
        componentOptions.reserve(componentTypes.size());
        for (auto& s : componentTypes) componentOptions.push_back(s.c_str());

        if (!componentOptions.empty()) {
            ImGui::Combo("Component Type", &selectedComponentIndex, componentOptions.data(), (int)componentOptions.size());

            std::string selectedComponent = componentOptions[selectedComponentIndex];

            // Show parameters for the selected component
            try {
                auto tempComponent = ComponentFactory::instance().Create(selectedComponent, componentData);
                tempComponent->getImGuiParams(componentData);
            }
            catch (const std::exception& e) {
                ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error with component UI: %s", e.what());
            }
        }

        // Confirm and Cancel buttons
        if (ImGui::Button("Confirm")) {
            // Add to queue
            std::string selectedComponent = componentOptions[selectedComponentIndex];
            componentsToAdd.push_back({ selectedComponent, componentData });
            addingComponent = false; // close add-component UI
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            addingComponent = false; // discard changes
        }

        ImGui::Unindent(20);
    }

    // Show queued components
    if (!componentsToAdd.empty()) {
        ImGui::SeparatorText("Queued Components");
        for (int i = 0; i < componentsToAdd.size(); ++i) {
            ImGui::Text("%d. %s", i + 1, componentsToAdd[i].type.c_str());
            ImGui::SameLine();
            std::string buttonLabel = "Remove##" + std::to_string(i);
            if (ImGui::Button(buttonLabel.c_str())) {
                componentsToAdd.erase(componentsToAdd.begin() + i);
                i--;
            }
        }
    }

#pragma endregion


    





    // Create button
    if (ImGui::Button("Create")) {



        auto rendererType = rendererOptions[selectedRendererIndex];

        // Construct the action (this captures all needed state)
        auto action = std::make_unique<GUIA_CreateGameObject>(
            &scene,
            std::string(name),
            pos,
            scale,
            rot,
            rendererType,
            rendererData,
            componentsToAdd
        );
        
        GUI_Manager::instance().ExecuteAction(std::move(action));

        componentsToAdd.clear();

        Close();


    }

    ImGui::End();
}