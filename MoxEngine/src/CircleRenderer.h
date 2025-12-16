#pragma once
#include "pch.h"

class CircleRenderer : public Renderer {

private:

	std::unique_ptr<sf::CircleShape> _circleShape;
	inline static float _radius = 32;
	inline static int _vertexCount = 32;



public:

	static std::unique_ptr<Renderer> Create(const nlohmann::json& data) {

		float radius = data.value("radius", 0); 
			
		int vertexCount = data.value("vertexCount", 3);
		
		sf::Color colour = data.value("colour", sf::Color::White);
		

		return std::make_unique<CircleRenderer>(radius, vertexCount, colour);
	}

	CircleRenderer(float radius =0, int vertexCount = 32, sf::Color colour = sf::Color::White) : Renderer() {
		_circleShape = std::make_unique<sf::CircleShape>(radius, vertexCount);
		_circleShape->setFillColor(colour);
		_circleShape->setOrigin({radius, radius});
		_drawable = _circleShape.get();

		
	}


	virtual sf::FloatRect GetGlobalBounds() const override {
	// since gameobject has its own transform and we just multiply the shape transform by that to position it on screen, we gotta do this to reflect its position.
		return _transform->GetSFTransform().transformRect(_circleShape->getGlobalBounds());
	}




	void SetRadius(float newRadius) {
		_circleShape->setRadius(newRadius);
		_circleShape->setOrigin({ newRadius, newRadius });
	}


#if IN_EDITOR
	virtual void getImGuiParams(nlohmann::json& data) override {


		if (!data.contains("colour")) data["colour"] = { 255,255,255,255 };
		float colourRaw[4] = {255,255,255,255};
		if (ImGui::InputFloat4("Colour", colourRaw)) {
			_circleShape->setFillColor(
				sf::Color(colourRaw[0], colourRaw[1], colourRaw[2], colourRaw[3])
			);
		}



		if (!data.contains("radius"))       data["radius"] = _radius;
		if (!data.contains("vertexCount"))  data["vertexCount"] = _vertexCount;

		float radius = data.value("radius", _radius);
		int vertexCount = data.value("vertexCount", _vertexCount);

		if (ImGui::InputFloat("Radius", &radius)) data["radius"] = radius;
		if (ImGui::InputInt("Vertex Count", &vertexCount)) data["vertexCount"] = vertexCount;
	}

	virtual void getInspectorParams() override {

		ImGui::SeparatorText("Circle Renderer");

		sf::Color curCol = _circleShape->getFillColor();
		float colourRaw[4] = { curCol.r, curCol.g, curCol.b, curCol.a };
		if (ImGui::InputFloat4("Colour", colourRaw)) {
			_circleShape->setFillColor(
				sf::Color(colourRaw[0], colourRaw[1], colourRaw[2], colourRaw[3])
			);
		}

		float radius = _circleShape->getRadius();
		if (ImGui::InputFloat("Radius", &radius)) {
			if (radius < 0.0f) radius = 0.0f;
			SetRadius(radius);
		}
		
		int vertexCount = _circleShape->getPointCount();
		if (ImGui::InputInt("Vertex Count", &vertexCount)) {
			if (vertexCount < 3) vertexCount = 3; 
			_circleShape->setPointCount(vertexCount);
		}
	}




	virtual nlohmann::json SaveToJSON() const override {
		nlohmann::json data;
		data["type"] = "circle";
		data["radius"] = _circleShape->getRadius();

		sf::Color curCol = _circleShape->getFillColor();
		data["colour"] = curCol;

		data["vertexCount"] = _circleShape->getPointCount();
		return data;
	}
#endif

private:
	inline static bool registered = [] {
		RendererFactory::instance().Register("circle", &CircleRenderer::Create);
		return true;
		}();
};