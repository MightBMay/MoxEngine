#pragma once
#include "Component.h"
#include "ComponentFactory.h"
#include "Global.h"
#include "TileMap.h"
#include "LDTK_Manager.h"




class TileMapComponent : public Component {

private:
	std::unique_ptr<TileMap> _tileMap = nullptr;
	std::string _projectPath = "";
	std::string _layerName = ""; // actual layer from ldtk to draw
	std::string _levelName = "";



public:

	static std::unique_ptr<Component> Create(const nlohmann::json& data) {
		auto component = std::make_unique<TileMapComponent>();
		component->_tileMap = std::make_unique<TileMap>();

		component->_projectPath = data.value("projectPath", "");
		component->_layerName = data.value("layer", "");
		component->_levelName = data.value("levelName", "");

		if (!component->_projectPath.empty() && !component->_layerName.empty() && !component->_levelName.empty()) {
			auto& level = LDTK_Manager::instance().getProject(component->_projectPath)->getWorld().getLevel(component->_levelName);
			component->_tileMap->load(level);

		}
		return component;


	}


	TileMap* GetTilemap() const { return _tileMap.get(); }
	TileMap* GetTilemap() { return _tileMap.get(); }

	void ReloadTileMap() {

		ldtk::Project* project = LDTK_Manager::instance().getProject(_projectPath);
		if (!project) { std::cerr << "Project Path not found: " << _projectPath << std::endl; return; }
		const ldtk::Level& level = project->getWorld().getLevel(_levelName);
		_tileMap->load(level);

	}

	void SetPathAndName(std::string projectPath, std::string layerName, std::string levelName) {
		_projectPath = projectPath;
		_layerName = layerName;
		_levelName = levelName;
		std::cout << _projectPath << ", " << _layerName << ", " << _levelName << "\n";
	}

	std::string GetProjectPath() const { return _projectPath; }
	void SetProjectPath(std::string newPath) { _projectPath = newPath; }

	std::string GetLevelName() const { return _levelName; }
	void SetLevelName(std::string newName) { _levelName = newName; }

	std::string GetLayerName() const { return _layerName; }
	void SetLayerName(std::string newName) { _layerName = newName; }

	virtual void Start() override;

	virtual std::string GetName() const override { return "TileMapComponent"; }

#if IN_EDITOR
	virtual void getImGuiParams(nlohmann::json& data) {}
	virtual void getInspectorParams();
	virtual nlohmann::json SaveToJSON() const {
		nlohmann::json data;
		data["type"] = "TileMapComponent";
		data["layer"] = _layerName;
		data["levelName"] = _levelName;
		data["projectPath"] = _projectPath;

		return data;
	}
#endif


private:
	inline static bool registered = [] {
		ComponentFactory::instance().Register("TileMapComponent", &TileMapComponent::Create);
		return true;
		}();
};
