#pragma once
#include "SFML/Graphics.hpp"
#include "TileMap.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "LDTK_Manager.h"
#define missing "../assets/sprites/missingTexture.png"
class TileMapRenderer : public Renderer {
private:
	std::unique_ptr<TileMap> _tileMap;
	std::string _projectPath = "";
	std::string _layerName = ""; // actual layer from ldtk to draw
	std::string _levelName = "";

public:

	static std::unique_ptr<Renderer> Create(const nlohmann::json& data) {
		std::unique_ptr<TileMapRenderer> r = std::make_unique<TileMapRenderer>();
		r->_tileMap = std::make_unique<TileMap>();
		r->_projectPath = data.value("projectPath", "");
		r->_layerName = data.value("layer", "");
		r->_levelName = data.value("levelName", "");
		if (!r->_projectPath.empty() && !r->_layerName.empty() && !r->_levelName.empty()) {
			auto& level = LDTK_Manager::instance().getProject(r->_projectPath)->getWorld().getLevel(r->_levelName);
			r->_tileMap->load(level);

		}

		return r;
	}


	TileMapRenderer() {
		LDTK_Manager::instance().onProjectReload += [this](const std::string& path, ldtk::Project& proj) {
				if (_projectPath == path) {
				auto& level = proj.getWorld().getLevel(_levelName);
				_tileMap->load(level);
			}
		};
	}

	void setTilemap(std::unique_ptr<TileMap> map, const std::string& layerName);

	virtual sf::FloatRect GetGlobalBounds() const override { return {}; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) override;



#if IN_EDITOR
	virtual void getImGuiParams(nlohmann::json& data)override;
	virtual void getInspectorParams()override;

	virtual nlohmann::json SaveToJSON() const override {
		nlohmann::json data;
		data["type"] = "tilemap";
		data["layer"] = _layerName;
		data["levelName"] = _levelName;
		data["projectPath"] = _projectPath;
		return data;
	}
#endif

private:
	inline static bool registered = [] {
		RendererFactory::instance().Register("tilemap", &TileMapRenderer::Create);
		return true;
		}();

};