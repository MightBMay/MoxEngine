#include "pch.h"
#include "TileMapRenderer.h"
#include "ImGUI.h"

void TileMapRenderer::setTilemap(std::unique_ptr<TileMap> newMap)
{
    _tileMap = std::move(newMap);
    rebuild();
}

void TileMapRenderer::rebuild()
{
    if (!_tileMap || !_tileMap->tileset)
        return;

    unsigned width = _tileMap->width;
    unsigned height = _tileMap->height;
    sf::Vector2u ts = _tileMap->tileSize;

    _vertices = sf::VertexBuffer(sf::PrimitiveType::Triangles, sf::VertexBuffer::Usage::Static);
    _vertices.create(width * height * 6);

    std::vector<sf::Vertex> verts;
    verts.resize(width * height * 6);

    int tilesPerRow = _tileMap->tileset->getSize().x / ts.x;

    unsigned idx = 0;

    for (unsigned y = 0; y < height; ++y) {
        for (unsigned x = 0; x < width; ++x) {

            int tileID = _tileMap->tileData[x + y * width];

            int tu = tileID % tilesPerRow;
            int tv = tileID / tilesPerRow;

            float px = x * ts.x;
            float py = y * ts.y;

            float tx = tu * ts.x;
            float ty = tv * ts.y;
            sf::Color col = sf::Color::White;
            verts[idx++] = sf::Vertex{ { px,              py }, col, { tx,              ty } };
            verts[idx++] = sf::Vertex{ { px + ts.x,       py },col,  { tx + ts.x,       ty } };
    verts[idx++] = sf::Vertex{ { px,              py + ts.y },col,  { tx,              ty + ts.y } };

            verts[idx++] = sf::Vertex{ { px + ts.x,       py }, col, { tx + ts.x,       ty } };
            verts[idx++] = sf::Vertex{ { px + ts.x,       py + ts.y },col,  { tx + ts.x,       ty + ts.y } };
            verts[idx++] = sf::Vertex{ { px,              py + ts.y },col,  { tx,              ty + ts.y } };
        }
    }

    _vertices.update(verts.data());
}

void TileMapRenderer::draw(sf::RenderTarget& target, sf::RenderStates states)
{
    if (!_tileMap) {
        setTilemap(CreateTestTileMap());
    }
    if (!_tileMap || !_tileMap->tileset)
        return;

    states.texture = _tileMap->tileset;
    target.draw(_vertices, states);
}


#if IN_EDITOR
void TileMapRenderer::getImGuiParams(nlohmann::json& data){}


void TileMapRenderer::getInspectorParams(){


    auto size = _tileMap->tileSize;
    int tileSize[2] = { size.x, size.y };
    if (ImGui::InputInt2("tile size", tileSize)) {
        _tileMap->tileSize = { tileSize[0], tileSize[1]};
    }




}

#endif