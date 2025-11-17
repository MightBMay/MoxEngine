#include "GUI_Manager.h"
#include "imgui-SFML.h"
#include "ImGuiFileDialog.h"
#include "SceneLoader.h"

void GUI_Manager::Draw() {
#pragma region Imgui Window Setup

    static bool showHierarchy = true;
    static bool showInspector = true;

    const ImVec2 windowPos = ImVec2(0, 0);
    const ImVec2 windowPivot = ImVec2(0.0f, 0.0f);
    ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always, windowPivot);
    ImGui::SetNextWindowBgAlpha(0.8f);

    ImGui::Begin(
        "##ToolbarOverlay",
        nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_AlwaysAutoResize |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoFocusOnAppearing |
        ImGuiWindowFlags_NoNav |
        ImGuiWindowFlags_NoMove
    );
#pragma endregion

#pragma region Scene Dropdown
    ImGui::SameLine(); // keep it on the same line as previous buttons

    // Make a proper button for Scene
    if (ImGui::Button("Scene")) {
        ImGui::OpenPopup("ScenePopup");
    }

    // Create the popup right under the button
    if (ImGui::BeginPopup("ScenePopup")) {
        // Save Scene
        if (ImGui::MenuItem("Save Scene")) {
            scene->Save();
        }

        // Create Scene
        if (ImGui::MenuItem("Create Scene")) {
            scene->Save();
            GUI_SceneHierarchy::instance().ClearSelectedObject();
            scene = std::make_unique<Scene>();
            window.setTitle("MoxEngine | Untitled Scene");
         }

        // Open Scene
        IGFD::FileDialogConfig config;
        config.path = "../assets/scenes";
        config.countSelectionMax = 1;

        if (ImGui::MenuItem("Open Scene")) {
            ImGuiFileDialog::Instance()->OpenDialog(
                "ChooseSceneDialog",
                "Choose Scene",
                ".scene",
                config
            );
        }

        ImGui::EndPopup();
    }

    // Handle the file dialog outside the popup
    if (ImGuiFileDialog::Instance()->Display("ChooseSceneDialog")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string newPath = ImGuiFileDialog::Instance()->GetFilePathName();
            scene = SceneLoader::LoadSceneFromPath(newPath);
        }
        ImGuiFileDialog::Instance()->Close();
    }
#pragma endregion
    
#pragma region Window Dropdown
    ImGui::SameLine();
    if (ImGui::Button("Window")) {
        ImGui::OpenPopup("WindowPopup");
    }

    if (ImGui::BeginPopup("WindowPopup")) {

        if (ImGui::MenuItem("Heirarchy",nullptr, &showHierarchy)) {
            //showHierarchy = !showHierarchy;
            GUI_SceneHierarchy::instance().SetVisible(showHierarchy);
        }

        if (ImGui::MenuItem("Inspector", nullptr, &showInspector)) {
            //showInspector = !showInspector;
            GUI_Inspector::instance().SetVisible(showInspector);
        }

        ImGui::EndPopup();
    }


#pragma endregion



#pragma region Undo 

    ImGui::SameLine();

    bool canUndo = !performedActions.empty();
    if (!canUndo) ImGui::BeginDisabled();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.25f, 0.45f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.35f, 0.55f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.45f, 0.65f, 1.0f));
    if (ImGui::Button("Undo"))
        GUI_Manager::UndoLastAction();
    ImGui::PopStyleColor(3);

    if (!canUndo) ImGui::EndDisabled();
#pragma endregion

    ImGui::End();
}
