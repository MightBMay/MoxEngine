#include "GUI_Inspector.h"
#include "GUI_Manager.h"
#include "GUIA_GameObject.h"
#if IN_EDITOR
#include "imgui.h"


void GUI_Inspector::Draw() {
    if (!_isVisible) return;
    ImGui::Begin("Inspector");

    GameObject* selected = nullptr;
    if (_hierarchy)
        selected = _hierarchy->getSelectedObject();

    if (!selected) {
        ImGui::Text("No object selected.");
        ImGui::End();
        return;
    }

    // --- Object Info ---
    ImGui::SeparatorText("GameObject Info");

    ImGui::TextColored(
        ImVec4(0.4, 0.4, 0.4, 1), 
        ("GUID: " + std::to_string(selected->GetGUID()) ).c_str()
    );


    std::array<char, 64> nameBuffer{};
    const std::string& currentName = selected->GetName();

    // Copy the current name into the buffer safely
    std::copy_n(currentName.begin(), std::min(currentName.size(), nameBuffer.size() - 1), nameBuffer.begin());

    if (ImGui::InputText("Name", nameBuffer.data(), nameBuffer.size())) {
        selected->SetName(std::string(nameBuffer.data()));
    }

    int layer = selected->GetLayer();
    if (ImGui::InputInt("Render Layer", &layer)) {
        selected->SetLayer(layer);
    }

    // --- Transform Info ---
    ImGui::SeparatorText("Transform");
    Transform* transform = selected->_transform.get();
    sf::Vector2f pos = transform->GetPosition();
    sf::Vector2f scale = transform->GetScale();
    float rot = transform->GetRotationDeg();

    if (ImGui::InputFloat2("Position", &pos.x))
        transform->SetPosition(pos);
    if (ImGui::InputFloat2("Scale", &scale.x))
        transform->SetScale(scale);
    if (ImGui::InputFloat("Rotation", &rot))
        transform->SetRotation(rot);



#pragma region renderer



    // --- Renderer Info ---


    ImGui::SeparatorText("Renderer");
    Renderer* renderer = selected->getRenderer();
    if (!_addingRenderer) {


        
        if (!renderer) {

            ImGui::SameLine();
            if (ImGui::Button("Add Renderer")) {
                _addingRenderer = true;
                rendererData = nlohmann::json::object();
            }
        }
        else {
            ImGui::SameLine();
            if (ImGui::Button("Remove Renderer")) {
                auto action = std::make_unique<GUIA_RemoveRenderer>(selected);
                GUI_Manager::instance().ExecuteAction(std::move(action));
            }
        }
    }
    else {
        ImGui::Indent(20);


        const std::vector<std::string> rendererTypes = RendererFactory::instance().GetTypes();
        std::vector<const char*> rendererOptions;
        rendererOptions.reserve(rendererTypes.size());
        for (auto& s : rendererTypes) rendererOptions.push_back(s.c_str());

        if (!rendererOptions.empty()) {
            ImGui::Combo("Renderer Type", &selectedRendererIndex, rendererOptions.data(), (int)rendererOptions.size());
        }


        if (ImGui::Button("Confirm")) {
            auto action = std::make_unique<GUIA_AddRenderer>(selected, rendererOptions[selectedRendererIndex], rendererData);
            GUI_Manager::instance().ExecuteAction(std::move(action));
            _addingRenderer = false;
        }

        ImGui::Unindent(20);
    }


    if (renderer) {
        renderer->getInspectorParams(); // calls each renderer’s own inspector UI
    }
    else {
        ImGui::SeparatorText("No renderer assigned.");
    }


#pragma endregion

#pragma region Collider

    ImGui::SeparatorText("Collider");
    Collider* collider = selected->getCollider();
    if (!_addingCollider) {

        if (!collider) {
            ImGui::SameLine();
            if (ImGui::Button("Add Collider")) {
                _addingCollider = true;
                colliderData = nlohmann::json::object();
            }
        }
        else {
            ImGui::SameLine();
            if (ImGui::Button("Remove Collider")) {
                auto action = std::make_unique<GUIA_RemoveCollider>(selected);
                GUI_Manager::instance().ExecuteAction(std::move(action));
            }
        }
    }

    else {
        ImGui::Indent(20);

        const std::vector<std::string> colliderTypes = ColliderFactory::instance().GetTypes();
        std::vector<const char*> colliderOptions;
        colliderOptions.reserve(colliderTypes.size());
        for (auto& s : colliderTypes) colliderOptions.push_back(s.c_str());
        std::cout << "Size: " << colliderOptions.size() << "\n";

        if (!colliderOptions.empty()) {
            ImGui::Combo("Collider Type", &selectedColliderIndex, colliderOptions.data(), (int)colliderOptions.size());
        }

        if (ImGui::Button("Confirm")) {
            auto action = std::make_unique<GUIA_AddCollider>(selected, colliderOptions[selectedRendererIndex], colliderData);
            GUI_Manager::instance().ExecuteAction(std::move(action));
            _addingCollider = false;
        }
        ImGui::Unindent(20);
    }

    if (collider)
        collider->getInspectorParams();
    else 
        ImGui::SeparatorText("No Collider Assigned");


#pragma endregion

  

#pragma region  add component
      // --- Component Info ---
    ImGui::SeparatorText("Components");
    if (!_addingComponent) {
        ImGui::SameLine();
        if (ImGui::Button("Add Component")) {
            _addingComponent = true;
            componentData = nlohmann::json::object();
        }
    }
    else
    {
        ImGui::Indent(20); // visually offset the add-component UI

   // Component type dropdown
        const std::vector<std::string> componentTypes = ComponentFactory::instance().GetTypes();
        std::vector<const char*> componentOptions;
        componentOptions.reserve(componentTypes.size());
        for (auto& s : componentTypes) componentOptions.push_back(s.c_str());

        if (!componentOptions.empty()) {
            ImGui::Combo("Component Type", &selectedComponentIndex, componentOptions.data(), (int)componentOptions.size());

            std::string selectedComponent = componentOptions[selectedComponentIndex];

            //// Show parameters for the selected component
            //try {
            //    auto tempComponent = ComponentFactory::instance().Create(selectedComponent, componentData);
            //    tempComponent->getInspectorParams(); // NOTE: use inspector version
            //}
            //catch (const std::exception& e) {
            //    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Error with component UI: %s", e.what());
            //}
        }

        // Confirm and Cancel buttons
        if (ImGui::Button("Confirm")) {
            // Create and attach component directly to the selected GameObject

            auto action = std::make_unique<GUIA_AddComponent>(selected, componentOptions[selectedComponentIndex], componentData);
            GUI_Manager::instance().ExecuteAction(std::move(action));


            _addingComponent = false; // close add-component UI
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            _addingComponent = false; // discard changes
        }

        ImGui::Unindent(20);
    }




#pragma endregion





#pragma region Component Inspector



    auto components = selected->getComponents();
    if (!components.empty()) {
        for (size_t i = 0; i < components.size(); ++i) {
            auto& comp = components[i];


            // Remove button
            std::string removeLabel = "Remove##" + std::to_string(i);
            if (ImGui::Button(removeLabel.c_str())) {

                auto action = std::make_unique<GUIA_RemoveComponent>(
                    selected,
                    comp,
                    &i
                );

                GUI_Manager::instance().ExecuteAction(std::move(action));
                continue; // skip rest of loop for removed component
            }

            ImGui::SameLine(); // optional, ensures header is next to remove button

            // Collapsing header for component contents
            std::string header = comp->GetName() + "##" + std::to_string(i);
            if (ImGui::CollapsingHeader(header.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                     // Enabled checkbox
                bool enabled = comp->GetEnabled();
                ImGui::Checkbox(("##enabled" + std::to_string(i)).c_str(), &enabled);
                comp->SetEnabled(enabled);
                ImGui::SameLine();
                ImGui::Text("Enabled");

                comp->getInspectorParams();
            }
        }
    }

    else {
        ImGui::SeparatorText("No components attached.");
    }

#pragma endregion

    ImGui::End();
}

#endif