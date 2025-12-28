#include "TilemapCollider.h"
#include "TilemapComponent.h"

#if  IN_EDITOR
#include "imgui.h"

void TileMapCollider::getInspectorParams() {
	ImGui::Indent(20);
	ImGui::Text("TileMap Collider");
	ImGui::Unindent(20);
}
#endif //  IN_EDITOR

void TileMapCollider::setTileMap(TileMapComponent* tmc) {
	_tilemapC = tmc; 
}
