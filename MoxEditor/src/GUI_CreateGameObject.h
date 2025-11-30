#pragma once
#include "Scene.h"
#include "RendererFactory.h"
#include "ColliderFactory.h"

struct QueuedComponent
{
    std::string type;
    nlohmann::json data;
};


class GUI_CreateGameObject {
private:
    bool addingComponent = false;
    
    // Persistent GUI state
    char name[64] = "NewObject";
    int renderLayer = 0;
    float pos[2] = { 0.f, 0.f };
    float scale[2] = { 1.f, 1.f };
    float rot = 0.f;

    static inline int selectedRendererIndex = 0;
    static inline int previousRendererIndex = -1;
    static inline nlohmann::json rendererData = nlohmann::json::object();

    static inline int selectedComponentIndex = 0;
    static inline int previousComponentIndex = -1;
    static inline nlohmann::json componentData = nlohmann::json::object();


    std::vector<QueuedComponent> componentsToAdd;
    bool _isVisible = false;

public:

    void SetVisible(bool visible) { _isVisible = visible; }
    bool IsVisible() const { return _isVisible; }

    void Open() { _isVisible = true; }
    void Close() { _isVisible = false; }


    static GUI_CreateGameObject& instance() {
        static GUI_CreateGameObject inst;
        return inst;
    }

    void Draw(Scene& scene);
};
