#pragma once
#include "ColliderFactory.h"
#include "Collider.h"
#include "GameObject.h"
class TileMapRenderer;
class TileMap;


struct TileMapCollider: Collider{
	std::string layerName = "";

	static std::unique_ptr<Collider> Create(const nlohmann::json& data) {
		std::unique_ptr<TileMapCollider> col = std::make_unique<TileMapCollider>();
		col->type = ColliderType::Tilemap;
		col->layerName = data.value("collisionLayerName", "");

		return col;

	}



	void OnAddedToGameObject() override;




	TileMap* getTileMap() const{ return _tilemap; }



#if IN_EDITOR

	virtual void getImGuiParams(nlohmann::json& data) {};
	virtual void getInspectorParams() {};

	virtual void SaveToJSON(nlohmann::json& data) {

		data["colliderType"] = type;

	}
#endif


private:

	TileMap* _tilemap = nullptr;

	static inline bool registered = []() {
		ColliderFactory::instance().Register("tilemap", &TileMapCollider::Create);
		return true;
		}();
};


