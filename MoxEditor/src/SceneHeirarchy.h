#pragma once
#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "CreateGameObject.h"

#include <memory>

class GUI_SceneHierarchy {
private:
    GameObject* selectedObject = nullptr; // currently selected object
    bool _isVisible = false;
public:

    static GUI_SceneHierarchy& instance() {
        static GUI_SceneHierarchy inst;
        return inst;
    }

    void SetVisible(bool visible) { _isVisible = visible; }
    bool IsVisible() const { return _isVisible; }

    void Open() { _isVisible = true; }
    void Close() { _isVisible = false; }


    // Get the currently selected object (nullptr if none)
    GameObject* getSelectedObject() const {
        return selectedObject;
    }

    void ClearSelectedObject() {
        selectedObject = nullptr;
    }

    void Draw(Scene& scene);
};
