#pragma once
#include "pch.h"

class RectRenderer : public Renderer {
private:
	std::unique_ptr<sf::RectangleShape> _rectShape;
	inline static float _defaultWidth = 32;
	inline static float _defaultHeight = 32;


public:
	static std::unique_ptr<Renderer> Create(const nlohmann::json& data) {
		float width = 0, height = 0;
		if (data.contains("width")) width = data["width"];
		if (data.contains("height")) height = data["height"];

		return std::make_unique<RectRenderer>(sf::Vector2f( width, height ));
	}

	RectRenderer(sf::Vector2f size) {
		_rectShape = std::make_unique<sf::RectangleShape>(size);
		_drawable = _rectShape.get();
	}

	virtual void getImGuiParams(nlohmann::json& data) override {
		if (!data.contains("width"))       data["width"] = _defaultWidth;
		if (!data.contains("height"))  data["height"] = _defaultHeight;

		float width = data.value("width", _defaultWidth);
		float height = data.value("height", _defaultHeight);

		if (ImGui::InputFloat("width", &width)) data["width"] = width;
		if (ImGui::InputFloat("height", &height)) data["height"] = height;
	}

	virtual void getInspectorParams() override {
		ImGui::SeparatorText("Rect Renderer");
		sf::Vector2f size = _rectShape->getSize();
		float width = size.x, height = size.y;
		if (ImGui::InputFloat("Width", &width)) {
			if (width< 0.0f) width= 0.0f;
			_rectShape->setSize({ width,height });
		}

		if (ImGui::InputFloat("Height", &height)) {
			if (height < 0) height = 0;
			_rectShape->setSize({ width,height });
		}
	}



	virtual sf::FloatRect GetGlobalBounds() const override {
		return _transform->GetSFTransform().transformRect(_rectShape->getGlobalBounds());
	}
	
	virtual nlohmann::json SaveToJSON() const override {
		nlohmann::json data;
		sf::Vector2f size = _rectShape->getSize();
		data["type"] = "rectangle";
		data["width"] = size.x;
		data["height"] = size.y;
		return data;
	}


private:
	inline static bool registered = [] {
		RendererFactory::instance().Register("rectangle", &RectRenderer::Create);
		return true;
		}();

};