#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>


class TextureManager {
public:

	static TextureManager& instance() {
		static TextureManager inst;
		return inst;
	}


	static sf::Texture* load(const std::string& filePath);
	static sf::Texture* get(const std::string& filePath, bool loadIfNotFound = true);

	static void clear() { _textures.clear(); }
private:
	inline static std::unordered_map<std::string, std::unique_ptr<sf::Texture>> _textures;
};