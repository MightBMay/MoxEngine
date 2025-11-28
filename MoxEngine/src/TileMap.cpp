// Created by Modar Nasser on 13/06/2021.

#include "TileMap.h"
#include <array>
#include <vector>
#include "LDtkLoader/Tileset.hpp"
#include <string>

auto TileMap::Textures::instance() -> Textures& {
    static Textures instance;
    return instance;
}

auto TileMap::Textures::get(const std::string& name) -> sf::Texture& {
    auto& data = instance().data;
    if (data.count(name) == 0)
        data[name].loadFromFile(TileMap::path + name);
    return instance().data.at(name);
}

TileMap::Layer::Layer(const ldtk::Layer& layer, sf::RenderTexture& render_texture)
    : m_render_texture(render_texture)
{

    auto temp = layer.getTileset().path;

    const std::string prefix = "../";
    if (temp.rfind(prefix, 0) == 0) {
        temp.insert(prefix.size(), "assets/");
    }
    
    m_tileset_texture = &Textures::get(temp);

    const auto& tiles = layer.allTiles();
    m_vertex_array.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertex_array.resize(tiles.size() * 6);

    int i = 0;

    for (const auto& tile : tiles) {
        auto verts = tile.getVertices(); // 4 vertices: TL, TR, BR, BL

        // Write 2 triangles
        sf::Vertex* quad = &m_vertex_array[i * 6];

        // Triangle 1 (TL, TR, BR)
        quad[0].position = { verts[0].pos.x, verts[0].pos.y };
        quad[0].texCoords = { (float)verts[0].tex.x, (float)verts[0].tex.y };

        quad[1].position = { verts[1].pos.x, verts[1].pos.y };
        quad[1].texCoords = { (float)verts[1].tex.x, (float)verts[1].tex.y };

        quad[2].position = { verts[2].pos.x, verts[2].pos.y };
        quad[2].texCoords = { (float)verts[2].tex.x, (float)verts[2].tex.y };

        // Triangle 2 (TL, BR, BL)
        quad[3].position = { verts[0].pos.x, verts[0].pos.y };
        quad[3].texCoords = { (float)verts[0].tex.x, (float)verts[0].tex.y };

        quad[4].position = { verts[2].pos.x, verts[2].pos.y };
        quad[4].texCoords = { (float)verts[2].tex.x, (float)verts[2].tex.y };

        quad[5].position = { verts[3].pos.x, verts[3].pos.y };
        quad[5].texCoords = { (float)verts[3].tex.x, (float)verts[3].tex.y };

        i++;
    }
}

void TileMap::Layer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.texture = m_tileset_texture;
    m_render_texture.clear(sf::Color::Transparent);
    m_render_texture.draw(m_vertex_array, states);
    m_render_texture.display();
    auto sprite = sf::Sprite(m_render_texture.getTexture());
    target.draw(sprite, states);
}

std::string TileMap::path;

void TileMap::load(const ldtk::Level& level) {
    std::cout << level.size<< " size \n";
    m_render_texture = sf::RenderTexture( sf::Vector2u(level.size.x, level.size.y) );
    m_layers.clear();
    for (const auto& layer : level.allLayers()) {
        if (layer.getType() == ldtk::LayerType::AutoLayer) {
            m_layers.insert({ layer.getName(), {layer, m_render_texture} });
        }
    }
}

auto TileMap::getLayer(const std::string& name) const -> const Layer& {
    return m_layers.at(name);
}