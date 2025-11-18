#include "pch.h"
#include "TextureManager.h"


 sf::Texture* TextureManager::load(const std::string& filePath) {

	 std::cout << "Loaded Texture : " + filePath << "\n";
	auto it = _textures.find(filePath);
	if (it != _textures.end())
		return it->second.get();


	auto tex = std::make_unique<sf::Texture>();
	if(!tex->loadFromFile(filePath)) {
		std::cerr << "Error loading texture with path: " + filePath << std::endl;
		nullptr;
	}
	sf::Texture* result = tex.get();
	_textures[filePath] = std::move(tex);
	return result;

}


 sf::Texture* TextureManager::get(const std::string& filePath, bool loadIfNotFound) {

	 std::cout << "Re Got Texture : " + filePath << "\n";
	auto it = _textures.find(filePath);

	
	if (it != _textures.end())
		return it->second.get();
	
	else if (loadIfNotFound)
		return load(filePath);
	else
		return nullptr;


}