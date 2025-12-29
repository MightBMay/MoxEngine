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
    target.draw(m_vertex_array, states);
}

std::string TileMap::path;

void TileMap::load(const ldtk::Level& level) {
    m_render_texture = sf::RenderTexture( sf::Vector2u(level.size.x, level.size.y) );
    m_layers.clear();
    m_collisionLayers.clear();
    std::string layerName = "";
    for (const auto& layer : level.allLayers()) {
        if (layer.getType() == ldtk::LayerType::AutoLayer) {
            layerName = layer.getName();
            m_layers.insert({ layerName, {layer, m_render_texture} });


            /* see message in intgrid if case below - this is temporary
            IntGridCollisionLayer col; 
            col.name = layerName;

            auto gridSize = layer.getGridSize();
            col.gridSize = { gridSize.x, gridSize.y };
            col.cellSize = layer.getCellSize();
            col.cells.resize(col.gridSize.x * col.gridSize.y);

            for (int y = 0; y < col.gridSize.y; y++)
                for (int x = 0; x < col.gridSize.x; x++) {
                    auto temp = (int)layer.getIntGridVal(x, y).value;
                    col.cells[x + y * col.gridSize.x] = temp;
                    std::cout << "value: " << temp << "\n";
                }

            */ // m_collisionLayers.insert({ layerName, std::move(col) });
            
            // see message in intgrid if case below- this is temporary


        }


        if (layer.getType() == ldtk::LayerType::IntGrid) {
            /*
             DEBUG / TODO : right now we index m_collisionlayers with the rendering layer, meaning since we do layer.getName here and assign
             to collision layer, it will never be found when indexing. temporary solution is to just use the rendering layers for collision for now.

            */
            IntGridCollisionLayer coll;
            coll.name = layerName.empty() ? layer.getName() : layerName;
            std::cout << "phys layer found : " << coll.name<<"\n";
            coll.gridSize = { layer.getGridSize().x, layer.getGridSize().y };
            coll.cellSize = layer.getCellSize();
            coll.cells.resize(coll.gridSize.x * coll.gridSize.y);

            for (int y = 0; y < coll.gridSize.y; y++)
                for (int x = 0; x < coll.gridSize.x; x++)
                    coll.cells[x + y * coll.gridSize.x] = layer.getIntGridVal(x, y).value;


            m_collisionLayers.insert({ coll.name, std::move(coll) });
        }


    }

    onTileMapLoad(this);




}

bool TileMap::isSolidTile(const std::string& layerName, int x, int y) const
{

    auto it = m_collisionLayers.find(layerName);
    if (it == m_collisionLayers.end())
        return false;

    const auto& layer = it->second;

    if (x < 0 || y<0 || x>=layer.gridSize.x || y >= layer.gridSize.y)return false;

    const int index = x + y * layer.gridSize.x;

    return layer.cells[index] >0;
}

bool TileMap::isSolidTile(const IntGridCollisionLayer& layer, int x, int y) const
{

    if (x < 0 || y < 0 || x >= layer.gridSize.x || y >= layer.gridSize.y)return false;

    const int index = x + y * layer.gridSize.x;
    return layer.cells[index] >0;
}


sf::FloatRect TileMap::getTileBounds(
    const std::string& layer, int x, int y) const
{
    const auto& l = m_collisionLayers.at(layer);

    return {
        
        {float(x * l.cellSize),float(y * l.cellSize)},
        { float(l.cellSize), float(l.cellSize)}
    };
}




auto TileMap::getLayer(const std::string& name) const -> const Layer& {
    return m_layers.at(name);
}