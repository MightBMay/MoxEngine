#pragma once
#include "ColliderFactory.h"
#include "Collider.h"
#include "GameObject.h"
#include "TileMapRenderer.h"

struct TileMapCollider: Collider{
	std::string layerName = "";

	static std::unique_ptr<Collider> Create(const nlohmann::json& data) {
		std::unique_ptr<TileMapCollider> col = std::make_unique<TileMapCollider>();
		col->type = ColliderType::Tilemap;
		col->layerName = data.value("collisionLayerName", "");

		return col;

	}



	void OnAddedToGameObject() override
	{
		Renderer* r = _parent->getRenderer();
		TileMapRenderer* tilemapRenderer = dynamic_cast<TileMapRenderer*>(r);

		if (!tilemapRenderer)
		{
			// This collider was attached to a non-tilemap object
			// Decide how strict you want to be:
			assert(false && "TileMapCollider requires TileMapRenderer");
			return;
		}

		_tilemap = tilemapRenderer->GetTilemap();
	}




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


