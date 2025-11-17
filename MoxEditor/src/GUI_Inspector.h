#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Component.h"
#include "SceneHeirarchy.h"


class GUI_Inspector {


private:
    GUI_SceneHierarchy* _hierarchy = &GUI_SceneHierarchy::instance(); // reference to hierarchy to get selected object
    bool _isVisible = true;
    bool _addingComponent = false;
    int selectedComponentIndex = 0;
    int previousComponentIndex = -1;

    bool _addingRenderer = false;
    int selectedRendererIndex = 0;
    int previousRendererIndex = -1;


    static inline nlohmann::json componentData = nlohmann::json::object();
    static inline nlohmann::json rendererData = nlohmann::json::object();
public:

    static GUI_Inspector& instance() {
        static GUI_Inspector inst;
        return inst;
    }

    void SetVisible(bool visible) { _isVisible = visible; }
    bool IsVisible() const { return _isVisible; }

    void Open() { _isVisible = true; }
    void Close() { _isVisible = false; }


    void Draw(); 

};
