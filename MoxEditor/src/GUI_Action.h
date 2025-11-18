#pragma once
#include "GUI_CreateGameObject.h"

struct GUI_Action
{
public:
    virtual ~GUI_Action() = default;
	virtual void Execute()  = 0;

	virtual void Undo()  = 0;
};



struct GUIA_RemoveGameObject :public GUI_Action {
private:
    Scene* _scene = nullptr;
    GameObject* _toRemove;
    std::unique_ptr<GameObject> _removedObject;


public:

    GUIA_RemoveGameObject(Scene* scene, GameObject* toRemove):
    _scene(scene), _toRemove(toRemove){}

    virtual void Execute()override{
        _removedObject = _scene->removeObject(_toRemove);
    }
    virtual void Undo() override {
        _scene->addObject(std::move(_removedObject));
    }
};

struct  GUIA_CreateGameObject : public GUI_Action
{
private:
    Scene* _scene = nullptr;
    std::string _name;
    int _renderLayer;
    float _pos[2];
    float _scale[2];
    float _rot;
    std::string _rendererType;
    nlohmann::json _rendererData;
    std::vector<QueuedComponent> _components;

    GameObject* _createdObject = nullptr; // non-owning, to Undo()

public:


    GUIA_CreateGameObject(
        Scene* scene,
        const std::string& name,
        const int renderLayer,
        const float pos[2],
        const float scale[2],
        const float rot,
        const std::string& rendererType,
        const nlohmann::json& rendererData,
        const std::vector<QueuedComponent>& components
    )
        : _scene(scene),
        _name(name),
        _renderLayer(renderLayer),
        _rot(rot),
        _rendererType(rendererType),
        _rendererData(rendererData),
        _components(components)
    {
        _pos[0] = pos[0];
        _pos[1] = pos[1];
        _scale[0] = scale[0];
        _scale[1] = scale[1];
    }

	virtual void Execute()  override;

	virtual void Undo()  override;
};


struct GUIA_AddComponent :public GUI_Action {
private:
    GameObject* _obj;
    const std::string _componentType;
    const nlohmann::json& _componentData;
    Component* _toAdd = nullptr;

public:
    GUIA_AddComponent(GameObject* obj, const std::string& componentType, const nlohmann::json& componentData) :
        _obj(obj), _componentType(componentType), _componentData(componentData) {}

    virtual void Execute() override {
        auto newComp = ComponentFactory::instance().Create(_componentType, _componentData);
        newComp->SetParent(_obj);
        _toAdd = newComp.get();
        _obj->addComponent(std::move(newComp));
        
    }

    virtual void Undo() override {
        if (_obj && _toAdd) {
            _obj->removeComponent(_toAdd);
        }
    }

};

struct GUIA_RemoveComponent : public GUI_Action {
private:
    GameObject* _obj;
    Component* _toRemove; 
    std::unique_ptr<Component> _removedComponent;
    size_t* _guiComponentIndex;

public:
    GUIA_RemoveComponent(GameObject* obj, Component* component, size_t* guiComponentIndex)
        : _obj(obj), _toRemove(component), _guiComponentIndex(guiComponentIndex) {}

    virtual void Execute() override {
        _removedComponent = _obj->removeComponent(_toRemove);
        *_guiComponentIndex -=1;
    }

    virtual void Undo() override {
        if(_obj && _removedComponent) _obj->addComponent(std::move(_removedComponent));
    }
};



struct GUIA_AddRenderer :public GUI_Action {
private:
    GameObject* _obj = nullptr;
    const std::string _rendererType;
    const nlohmann::json& _rendererData;
    Renderer* _toAdd = nullptr;
public:
    GUIA_AddRenderer(GameObject* obj, const std::string& rendererType, const nlohmann::json& rendererData):
        _obj(obj), _rendererType(rendererType), _rendererData(rendererData){}

    virtual void Execute() override {
        auto newRenderer = RendererFactory::instance().Create(_rendererType, _rendererData);
        _obj->setRenderer(std::move(newRenderer));
    }

    virtual void Undo() override {
        _obj->RemoveRenderer();
    }

};


struct GUIA_RemoveRenderer : public GUI_Action {

private:
    GameObject* _obj = nullptr;

    std::unique_ptr<Renderer> _removed = nullptr;
public:
    GUIA_RemoveRenderer(GameObject* obj) :
        _obj(obj) {}

    virtual void Execute() override {
        _removed = _obj->RemoveRenderer();
    }

    virtual void Undo() override {
        _obj->setRenderer(std::move(_removed));
    }


};