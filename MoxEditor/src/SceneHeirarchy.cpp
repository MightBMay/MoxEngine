#include "SceneHeirarchy.h"
#include "GUI_Manager.h"
#include "GUI_Action.h"

void GUI_SceneHierarchy::Draw(Scene& scene) {
    if (!_isVisible) return;
    ImGui::Begin("Scene Hierarchy");
    ImGui::SameLine();

    // --- Open Create GameObject (one-shot) ---
    if (ImGui::Button("Create GameObject")) {
        GUI_CreateGameObject::instance().Open();
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove GameObject")) {
        if (selectedObject) {
            auto action = std::make_unique<GUIA_RemoveGameObject>(&scene, selectedObject);
            GUI_Manager::instance().ExecuteAction(std::move(action));
            selectedObject = nullptr;
        }
    }

    ImGui::SeparatorText("Scene");

     // Create a child box to contain the selectable objects
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(.15f, .15f, .15f, .50f));
    ImGui::BeginChild("HierarchyBox", ImVec2(0, 0), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);
    ImGui::PopStyleColor();
    const auto& objects = scene.getObjects();

    for (size_t i = 0; i < objects.size(); ++i) {
        GameObject* obj = objects[i].get();

        bool isSelected = (obj == selectedObject);

        // 
        std::string label = obj->GetName() + "##" + std::to_string(reinterpret_cast<uintptr_t>(obj));
        if (ImGui::Selectable(label.c_str(), isSelected, ImGuiSelectableFlags_SpanAllColumns)) {
            selectedObject = obj;
        }

    }
    ImGui::EndChild();
    ImGui::End();
}