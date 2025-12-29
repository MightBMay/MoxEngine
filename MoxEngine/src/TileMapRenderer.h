#pragma once
#include "SFML/Graphics.hpp"
#include "TileMap.h"
#include "Renderer.h"
#include "TextureManager.h"
#include "LDTK_Manager.h"
#define missing "../assets/sprites/missingTexture.png"

class TileMapComponent;

class TileMapRenderer : public Renderer {
private:
	TileMapComponent* _tilemap;

public:

	static std::unique_ptr<Renderer> Create(const nlohmann::json& data) {
		return std::make_unique<TileMapRenderer>();
	}
	
	TileMapRenderer() = default;

	void setTilemap(TileMapComponent* newTilemapComponent) {
		_tilemap = newTilemapComponent;
	}

	virtual void OnAddedToGameObject() override {
		if (_tilemap) return;

		auto tmc = _parent->GetComponent<TileMapComponent>();
		if (tmc) {
			_tilemap = tmc;
		}
		else {
			std::cerr << "Tilemap Renderer attempted to get non existant tilemap component. remove renderer and add a tilemap component first.";
		}


	}

	virtual sf::FloatRect GetGlobalBounds() const override { return {}; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) override;


#if IN_EDITOR
	virtual void getImGuiParams(nlohmann::json& data)override;
	virtual void getInspectorParams()override;

	virtual nlohmann::json SaveToJSON() const override {
		nlohmann::json data;
		data["type"] = "tilemap";
		return data;
	}
#endif

private:
	inline static bool registered = [] {
		RendererFactory::instance().Register("tilemap", &TileMapRenderer::Create);
		return true;
		}();

};