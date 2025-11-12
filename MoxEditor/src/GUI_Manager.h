#pragma once
#include <stack>
#include "GUI_Action.h"
#include "GUI_Inspector.h"
#include "SceneHeirarchy.h"
#include "Global.h"




class GUI_Manager {
public:
	inline static std::stack<std::unique_ptr<GUI_Action>> performedActions;

	static GUI_Manager& instance() {
		static GUI_Manager inst;
		return inst;
	}
	 
    static void ExecuteAction(std::unique_ptr<GUI_Action> action) {
        action->Execute();
        performedActions.push(std::move(action));
    }

    static void UndoLastAction() {
        if (!performedActions.empty()) {
            performedActions.top()->Undo();
            performedActions.pop();
        }
    }

    void Draw() {
        // Position top-left overlay
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


        // --- Save Button ---

        if (ImGui::Button("Save Scene")) {
            curScene->SaveToFile("testing.scene");
        }

        ImGui::SameLine();

        // --- Undo Button ---
        bool canUndo = !performedActions.empty();
        if (!canUndo) ImGui::BeginDisabled();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.25f, 0.45f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.35f, 0.55f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.35f, 0.45f, 0.65f, 1.0f));
        if (ImGui::Button("Undo"))
            GUI_Manager::UndoLastAction();
        ImGui::PopStyleColor(3);

        if (!canUndo) ImGui::EndDisabled();

        ImGui::SameLine();

        // --- Toggle Hierarchy ---
        static bool showHierarchy = false;
        if (ImGui::Button(showHierarchy ? "Hierarchy [ON]" : "Hierarchy [OFF]")) {
            showHierarchy = !showHierarchy;
            GUI_SceneHierarchy::instance().SetVisible(showHierarchy);
        }

        ImGui::SameLine();

        // --- Toggle Inspector ---
        static bool showInspector = false;
        if (ImGui::Button(showInspector ? "Inspector [ON]" : "Inspector [OFF]")) {
            showInspector = !showInspector;
            GUI_Inspector::instance().SetVisible(showInspector);
        }

        ImGui::End();
    }



};