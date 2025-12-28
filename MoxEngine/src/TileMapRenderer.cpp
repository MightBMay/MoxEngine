#include "pch.h"
#include "TilemapComponent.h"
#include "TileMapRenderer.h"


#include "LDtkLoader/Project.hpp"
#include "LDtkLoader/Level.hpp"
#include "LDTK_Manager.h"
#if IN_EDITOR
#include "ImGUI.h"
#include "ImGuiFileDialog.h"
//#include "GUI_Manager.h"
//#include "GUIA_Tilemap.h"
#endif

void TileMapRenderer::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (!_tilemap) return;
    TileMap* tm = _tilemap->GetTilemap();
    const std::string& layerName = _tilemap->GetLayerName();
    if (! tm||layerName.empty()) return;

    try {
  
        const auto& layer = tm->getLayer(layerName);
        target.draw(layer, states);
    }
    catch (const std::exception& e) {
        //std::cerr << "Error drawing layer '" << _layerName << "': " << e.what() << "\n";
    }
}


#if IN_EDITOR
void TileMapRenderer::getImGuiParams(nlohmann::json& data) {
   // data["layer"] = _layerName;
}


void TileMapRenderer::getInspectorParams() {
    ImGui::Indent(20);
    ImGui::Text("TilemapRenderer");
    ImGui::Unindent(20);
//    ImGui::SeparatorText("Tilemap Renderer");
//
//    bool wasChanged = false;
//#pragma region Texture Select Dialog
//
//
//
//	IGFD::FileDialogConfig config;
//	config.path = "../assets/ldtk";
//	config.countSelectionMax = 1;
//    std::string newProjectPath = _projectPath;
//
//	if (ImGui::Button("Select LDTK Project")) {
//		ImGuiFileDialog::Instance()->OpenDialog(
//			"ChooseLDTKProjectDialog",
//			"Choose LDTK Project",
//			".ldtk",
//			config
//		);
//	}
//    
//
//	if (ImGuiFileDialog::Instance()->Display("ChooseLDTKProjectDialog"))
//	{
//		if (ImGuiFileDialog::Instance()->IsOk())
//		{
//			newProjectPath = ImGuiFileDialog::Instance()->GetFilePathName();
//            
//
//            wasChanged = true;
//            _levelName = "Level_0"; // default level name 
//
//		}
//
//		ImGuiFileDialog::Instance()->Close();
//	}
//
//	//ImGui::SameLine();
//	//ImGui::Text(("Path: " + _spritePath).c_str());
//#pragma endregion
//
//
//
//
//
//
//
//    char levelNameBuf[256]; // create buffer for level name
//    std::string newLevelName = _levelName; // get current level name into a seperate variable to display/modify
//    strcpy_s(levelNameBuf, newLevelName.c_str()); // copy cstring to char buffer.
//    ImGui::InputText("Level Name", levelNameBuf, sizeof(levelNameBuf)); 
//    if (ImGui::IsItemDeactivatedAfterEdit()) // waits until input text is unselected to perform.
//    {
//        newLevelName = levelNameBuf;
//        wasChanged = true;
//    }
//
//    char layerNamebuf[256]; // same as levelNameBuf
//    std::string newLayerName = _layerName;
//    strcpy_s(layerNamebuf, newLayerName.c_str());
//
//    ImGui::InputText("Layer", layerNamebuf, sizeof(layerNamebuf));
//    if (ImGui::IsItemDeactivatedAfterEdit())// waits until input text is unselected to perform.
//    {
//        newLayerName = layerNamebuf;
//        wasChanged = true;
//    }
//
//
//    if (wasChanged) { 
//        GUI_Manager::ExecuteAction(
//            std::move(
//                std::make_unique<GUIA_TileMapSettingsChange>(this, newProjectPath, newLayerName, newLevelName)
//            )
//        );
//        // figure out how to call this
//
//    }
}

#endif