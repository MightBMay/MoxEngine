#pragma once
#include "Component.h"
#include <SFML/Graphics.hpp>
#include <string>
#include "Global.h"

class CameraController: public Component
{
private:
    Transform* _transform;
    float _moveSpeed = 300.0f;          // Movement speed in units per second
    float _zoomSpeed = 1.0f;            // Speed of zoom
    float _currentZoom = 1.0f;          // Current zoom level
    bool _active = true;                // Whether this camera is currently controlling the view

public:

    static std::unique_ptr<Component> Create(const nlohmann::json& data) {

        bool enabled = data.value("enabled", true);
        return std::make_unique<CameraController>(enabled);
    }

    CameraController(bool enabled) {
        _enabled = enabled; 

    };

    virtual std::string GetName() const override { return "CameraController"; }

 
    virtual void Start() override {
        _transform = _parent->_transform.get();
    }
    void SetActive(bool active) { _active = active; }

    virtual void Update(float dt) override
    {
        if (!_active || !_parent) return;

        playerView.setCenter(_transform->GetPosition());        
        
    }

    virtual void getImGuiParams(nlohmann::json& data) override {
        std::string label = "CameraController##" + std::to_string(reinterpret_cast<uintptr_t>(this));
        if (ImGui::InputFloat(label.c_str(), &_moveSpeed)) {

        }
    }

    virtual void getInspectorParams() override
    {
        ImGui::SeparatorText("Camera Controller");
    }

    // --- Serialization ---
    virtual nlohmann::json SaveToJSON() const override
    {
        nlohmann::json data;
        data["type"] = "CameraController";
        data["enabled"] = _enabled;
        return data;
    }

private:
    inline static bool registered = [] {
        ComponentFactory::instance().Register("CameraController", &CameraController::Create);
        return true;
        }();

};
