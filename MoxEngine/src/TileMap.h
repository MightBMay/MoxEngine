// Created by Modar Nasser on 13/06/2021.

#pragma once

#include <vector>
#include <map>
#include "Event.h"
#include <SFML/Graphics.hpp>
#include <LDtkLoader/Level.hpp>

class TileMap {
public:
    static std::string path;

    class Textures {
        Textures() = default;
        std::map<std::string, sf::Texture> data;
        static auto instance() -> Textures&;
    public:
        Textures(const Textures&) = delete;
        static auto get(const std::string& name) -> sf::Texture&;
    };

    class Layer : public sf::Drawable {
        friend TileMap;
        Layer(const ldtk::Layer& layer, sf::RenderTexture& render_texture);
        sf::Texture* m_tileset_texture;
        sf::RenderTexture& m_render_texture;
        sf::VertexArray m_vertex_array;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };


    struct IntGridCollisionLayer {
        std::string name;
        sf::Vector2i gridSize; // in cells
        int cellSize;          // pixels
        std::vector<int> cells; // 0 = empty, 1 = solid (or store raw value)

    };




    TileMap() = default;
    void load(const ldtk::Level& level);
    auto getLayer(const std::string& name) const -> const Layer&;

    inline const IntGridCollisionLayer* getCollisionLayer(const std::string& name)  const { 
        //TODO make this safer in case invalid input.
        auto it = m_collisionLayers.find(name);
        if (it == m_collisionLayers.end()) return {};
        return &it->second;

    }


    bool isSolidTile(const std::string& layerName, int x, int y) const;
    bool isSolidTile(const IntGridCollisionLayer& layer, int x, int y) const;
    sf::FloatRect getTileBounds(const std::string& layer, int x, int y) const;

    void CheckLayers() {
        if (m_layers.empty()) std::cout << "empty layuers";
        for (const auto& pair : m_layers) {
            std::cout << "layer: " << pair.first << "\n";
        }
    }




private:

    //Event<> onTileMapLoad{};
    mutable sf::RenderTexture m_render_texture;
    std::map<std::string, Layer> m_layers;
    std::map<std::string, IntGridCollisionLayer> m_collisionLayers;

};