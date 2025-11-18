#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
struct TileMap {
	std::string texturePath;
	const sf::Texture* tileset;

	sf::Vector2u tileSize;
	unsigned width;
	unsigned height;
	std::vector<int> tileData;

	TileMap() : tileset(nullptr), width(0), height(0) {}


	void SaveToJSON(nlohmann::json& data) {

		data["spritePath"] = texturePath;
		data["tileSize"] = { tileSize.x, tileSize.y };
		data["width"] = width;
		data["height"] = height;
		data["tileData"] = tileData;
	}

};

