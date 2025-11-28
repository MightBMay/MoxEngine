#include "pch.h"

#include "TileMapRenderer.h"
#include "ImGUI.h"
#include "ImGuiFileDialog.h"
#include "LDtkLoader/Project.hpp"
#include "LDtkLoader/Level.hpp"
#include "LDTK_Manager.h"

void TileMapRenderer::setTilemap(std::unique_ptr<TileMap> newMap, const std::string& layerName) {

    _tileMap = std::move(newMap);    
    _layerName = layerName;
}


void TileMapRenderer::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (!_tileMap) {
        std::cerr << " Null tilemap\n";
        return;
    }

    if (_layerName.empty()) {
        std::cerr << "Layer name was empty\n";
        return;
    }
    try {
  
        const auto& layer = _tileMap->getLayer(_layerName);
        target.draw(layer, states);
    }
    catch (const std::exception& e) {
        std::cerr << "Error drawing layer '" << _layerName << "': " << e.what() << "\n";
    }
}


#if IN_EDITOR
void TileMapRenderer::getImGuiParams(nlohmann::json& data) {
    data["layer"] = _layerName;
}


void TileMapRenderer::getInspectorParams() {
    ImGui::SeparatorText("Tilemap Renderer");


#pragma region Texture Select Dialog



	IGFD::FileDialogConfig config;
	config.path = "../assets/ldtk";
	config.countSelectionMax = 1;


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
			_projectPath = ImGuiFileDialog::Instance()->GetFilePathName();
            

            ldtk::Project* project = LDTK_Manager::instance().getProject(_projectPath);
            
            const ldtk::Level& level = project->getWorld().getLevel("Level_0");
            //curLevel = &level; // store in global so other classes can access, and it stays alive.
            _tileMap->load(level);

		}

		ImGuiFileDialog::Instance()->Close();
	}

	//ImGui::SameLine();
	//ImGui::Text(("Path: " + _spritePath).c_str());
#pragma endregion








    char buf[256];
    strcpy_s(buf, _layerName.c_str());

    if (ImGui::InputText("Layer to draw", buf, sizeof(buf))) {
        _layerName = buf;
    }
}

#endif