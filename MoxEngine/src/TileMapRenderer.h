#pragma once
#include "SFML/Graphics.hpp"
#include "TileMap.h"
#include "Renderer.h"
#include "TextureManager.h"
#define missing "../assets/sprites/missingTexture.png"
class TileMapRenderer : public Renderer {
private:
	std::unique_ptr<TileMap> _tileMap = nullptr;
	sf::VertexBuffer _vertices;

public:

	static std::unique_ptr<Renderer> Create(const nlohmann::json& data) {
		std::string spritePath = data.value("spritePath", missing);
        sf::Vector2u tileSize{16,16};
        if (data.contains("tileSize")) {
            tileSize.x = data["tileSize"][0].get<unsigned>();
            tileSize.y = data["tileSize"][1].get<unsigned>();
        }
        
        unsigned width = data.value("width", 0);
        unsigned height = data.value("height", 0);

        std::vector<int> tileData;
        if (data.contains("tileData") && data["tileData"].is_array()) {
            tileData = data["tileData"].get<std::vector<int>>();
        }


        std::unique_ptr<TileMap> map = std::make_unique<TileMap>();
        map->texturePath = spritePath;
        map->tileset = TextureManager::get(spritePath);
        map->tileSize = { tileSize };
        map->width = width;
        map->height = height;
        map->tileData = tileData;


        return std::make_unique<TileMapRenderer>( std::move(map) );
	}


	TileMapRenderer() = default;
    TileMapRenderer(std::unique_ptr<TileMap> map) {

        setTilemap(std::move(map));
        
    }

	void setTilemap(std::unique_ptr<TileMap> newMap);
	void rebuild();

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) override;

	virtual sf::FloatRect GetGlobalBounds() const { return sf::FloatRect(); }


#if IN_EDITOR
    virtual void getImGuiParams(nlohmann::json& data)override;
    virtual void getInspectorParams()override;

    virtual nlohmann::json SaveToJSON() const override{ 
        if (!_tileMap) return {};
        nlohmann::json data;
        data["type"] = "tilemap";
        _tileMap->SaveToJSON(data);
        return data;
    }
#endif

private:
    inline static bool registered = [] {
        RendererFactory::instance().Register("tilemap", &TileMapRenderer::Create);
        return true;
        }();

};