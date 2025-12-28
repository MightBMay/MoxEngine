
#include "TilemapComponent.h"
#include "TileMapRenderer.h"
#include "TilemapCollider.h"

#include  "ImGuiFileDialog.h"


void TileMapComponent::Start(){


    Renderer* r = _parent->getRenderer();
    if (r) {
        TileMapRenderer* tmr = dynamic_cast<TileMapRenderer*>(r);
        if (!tmr) {
            // This collider was attached to a non-tilemap object
            // Decide how strict you want to be:
            std::cout << "No tilemap renderer found when adding tilemap component.";
        }

        tmr->setTilemap(this);


    }


    Collider* c = _parent->getCollider();
    if (c) {
        TileMapCollider* tmc = dynamic_cast<TileMapCollider*>(c);
        if (!tmc) {
            std::cout << "No tilemap collider found when adding tilemap component.";
        }
        tmc->setTileMap(this);
    }



    LDTK_Manager::instance().onProjectReload += [this](const std::string& path, ldtk::Project& proj) {
        if (_projectPath == path) {
            auto& level = proj.getWorld().getLevel(_levelName);
            _tileMap->load(level);
        }
        };


}

void TileMapComponent::getInspectorParams() {
    ImGui::SeparatorText("Tilemap Renderer");

    bool wasChanged = false;
#pragma region Texture Select Dialog



    IGFD::FileDialogConfig config;
    config.path = "../assets/ldtk";
    config.countSelectionMax = 1;
    std::string newProjectPath = _projectPath;

    if (ImGui::Button("Select LDTK Project")) {
        ImGuiFileDialog::Instance()->OpenDialog(
            "ChooseLDTKProjectDialog",
            "Choose LDTK Project",
            ".ldtk",
            config
        );
    }


    if (ImGuiFileDialog::Instance()->Display("ChooseLDTKProjectDialog"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            newProjectPath = ImGuiFileDialog::Instance()->GetFilePathName();
            wasChanged = true;
            _levelName = "Level_0"; // default level name 

        }

        ImGuiFileDialog::Instance()->Close();
    }

    //ImGui::SameLine();
    //ImGui::Text(("Path: " + _spritePath).c_str());
#pragma endregion







    char levelNameBuf[256]; // create buffer for level name
    std::string newLevelName = _levelName; // get current level name into a seperate variable to display/modify
    strcpy_s(levelNameBuf, newLevelName.c_str()); // copy cstring to char buffer.
    ImGui::InputText("Level Name", levelNameBuf, sizeof(levelNameBuf));
    if (ImGui::IsItemDeactivatedAfterEdit()) // waits until input text is unselected to perform.
    {
        newLevelName = levelNameBuf;
        wasChanged = true;
    }

    char layerNamebuf[256]; // same as levelNameBuf
    std::string newLayerName = _layerName;
    strcpy_s(layerNamebuf, newLayerName.c_str());

    ImGui::InputText("Layer", layerNamebuf, sizeof(layerNamebuf));
    if (ImGui::IsItemDeactivatedAfterEdit())// waits until input text is unselected to perform.
    {
        newLayerName = layerNamebuf;
        wasChanged = true;
    }


    if (wasChanged) {
        _levelName = newLevelName;
        _projectPath = newProjectPath;
        _layerName = newLayerName;
        ReloadTileMap();
    }
}