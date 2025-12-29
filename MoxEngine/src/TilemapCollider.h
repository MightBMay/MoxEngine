#pragma once
#include "ColliderFactory.h"
#include "Collider.h"
#include "GameObject.h"

class TileMapComponent;
class TileMap;

struct TileMapCollider: Collider{
	static std::unique_ptr<Collider> Create(const nlohmann::json& data) {
		std::unique_ptr<TileMapCollider> col = std::make_unique<TileMapCollider>();
		col->type = ColliderType::Tilemap;

		return col;

	}

	void setTileMap(TileMapComponent* tmc);

	TileMapComponent* getTileMapComponent() const{ return _tilemapC; }

	virtual void OnAddedToGameObject() override {
		if (_tilemapC) return;

		auto tmc = _parent->GetComponent<TileMapComponent>();
		if (tmc) _tilemapC = tmc;
		else {
			std::cerr << "Tilemap Collider attempted to get non existant tilemap component. remove collider and add a tilemap component first.";
		}

	
	}
	


#if IN_EDITOR

	virtual void getImGuiParams(nlohmann::json& data) {};
	virtual void getInspectorParams() override;

	virtual void SaveToJSON(nlohmann::json& data) {
		nlohmann::json colliderData;
		colliderData["type"] = "tilemap";
		data["colliderData"] = colliderData;
		

	}
#endif


private:

	TileMapComponent* _tilemapC = nullptr;

	static inline bool registered = []() {
		ColliderFactory::instance().Register("tilemap", &TileMapCollider::Create);
		return true;
		}();
};


