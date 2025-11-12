#pragma once
#include "pch.h"

class CircleRenderer : public Renderer {

private:

	std::unique_ptr<sf::CircleShape> _circleShape;
	inline static float _radius = 32;
	inline static int _vertexCount = 32;



public:

	static std::unique_ptr<Renderer> Create(const nlohmann::json& data) {

		float radius = 0;
		int vertexCount = 0;
		if (data.contains("radius")) radius = data["radius"];
			
		if (data.contains("vertexCount")) vertexCount = data["vertexCount"];

		return std::make_unique<CircleRenderer>(radius, vertexCount);
	}

	CircleRenderer(float radius =0, float vertexCount = 32) : Renderer() {
		_circleShape = std::make_unique<sf::CircleShape>(radius, vertexCount);
		_drawable = _circleShape.get();
		
	}


	virtual void getImGuiParams(nlohmann::json& data) override {
		if (!data.contains("radius"))       data["radius"] = _radius;
		if (!data.contains("vertexCount"))  data["vertexCount"] = _vertexCount;

		float radius = data.value("radius", _radius);
		int vertexCount = data.value("vertexCount", _vertexCount);

		if (ImGui::InputFloat("Radius", &radius)) data["radius"] = radius;
		if (ImGui::InputInt("Vertex Count", &vertexCount)) data["vertexCount"] = vertexCount;
	}

	virtual void getInspectorParams() override {
		ImGui::SeparatorText("Circle Renderer");
		float radius = _circleShape->getRadius();
		if (ImGui::InputFloat("Radius", &radius)) {
			if (radius < 0.0f) radius = 0.0f;
			_circleShape->setRadius(radius);
		}
		
		int vertexCount = _circleShape->getPointCount();
		if (ImGui::InputInt("Vertex Count", &vertexCount)) {
			if (vertexCount < 3) vertexCount = 3; 
			_circleShape->setPointCount(vertexCount);
		}
	}


	virtual sf::FloatRect GetGlobalBounds() const override {
		// since gameobject has its own transform and we just multiply the shape transform by that to position it on screen, we gotta do this to reflect its position.
		return _transform->GetSFTransform().transformRect(_circleShape->getGlobalBounds());
	}




	void SetRadius(float newRadius) {
		_circleShape->setRadius(newRadius);
	}

	virtual nlohmann::json SaveToJSON() const override {
		nlohmann::json data;
		data["type"] = "circle";
		data["radius"] = _circleShape->getRadius();
		data["vertexCount"] = _circleShape->getPointCount();
		return data;
	}


private:
	inline static bool registered = [] {
		RendererFactory::instance().Register("circle", &CircleRenderer::Create);
		return true;
		}();
};