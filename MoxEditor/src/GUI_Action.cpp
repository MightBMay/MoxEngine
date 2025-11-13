#include "GUI_Action.h"
#include "pch.h"
#include "GUI_Manager.h"
#include "CreateGameObject.h"
#include "Scene.h"
#include "ComponentFactory.h"
#include "SceneHeirarchy.h"





void GUIA_CreateGameObject::Execute() {
	
    if (!_scene) return;

      // --- Create the GameObject ---
    auto obj = std::make_unique<GameObject>();
    obj->SetName(_name);
    obj->_transform->SetTransform({ _pos[0], _pos[1] }, { _scale[0], _scale[1] }, _rot);

    // --- Create renderer ---
    if (_rendererType != "None") {
        auto renderer = RendererFactory::instance().Create(_rendererType, _rendererData);
        renderer->SetTransform(obj->_transform.get());
        obj->setRenderer(std::move(renderer));

    }

    // --- Create and attach components ---
    GameObject* objPtr = obj.get();
    for (auto& cData : _components) {
        auto c = ComponentFactory::instance().Create(cData.type, cData.data);
        c->SetParent(objPtr);
        obj->addComponent(std::move(c));
    }
    // --- Store pointer to undo later ---
    _createdObject = obj.get();

    // --- Add to scene ---
    _scene->addObject(std::move(obj));

    

}

void GUIA_CreateGameObject::Undo()  {
    if (_scene && _createdObject) {

        if (GUI_SceneHierarchy::instance().getSelectedObject() == _createdObject) {
            GUI_SceneHierarchy::instance().ClearSelectedObject();
        }



        _scene->removeObject(_createdObject);
    }
}