#pragma once
#include "pch.h"

class RectRenderer : public Renderer {
private:
	std::unique_ptr<sf::RectangleShape> _rectShape;
	inline static float _defaultWidth = 32;
	inline static float _defaultHeight = 32;


public:
	static std::unique_ptr<Renderer> Create(const nlohmann::json& data) {
		
		float width = data.value("width", 0); 
		float height = data.value("height", 0);
		sf::Color colour = data.value("colour", sf::Color::White);

		return std::make_unique<RectRenderer>(sf::Vector2f( width, height ), colour);
	}

	RectRenderer(sf::Vector2f size, sf::Color colour) {
		_rectShape = std::make_unique<sf::RectangleShape>(size);
		_rectShape->setFillColor(colour);
		_rectShape->setOrigin(size / 2.0f);
		_drawable = _rectShape.get();
	}


	virtual void SetSize(sf::Vector2f newSize) {
		_rectShape->setSize(newSize);
		_rectShape->setOrigin({ newSize.x/2, newSize.y/2});
	}

	virtual sf::FloatRect GetGlobalBounds() const override {
		return _transform->GetSFTransform().transformRect(_rectShape->getGlobalBounds());
	}

#if IN_EDITOR

	virtual void getImGuiParams(nlohmann::json& data) override {

		if (!data.contains("colour")) data["colour"] = { 1,1,1,1 };
		float colourRaw[4];
		if (ImGui::InputFloat4("Colour", colourRaw)) {
			_rectShape->setFillColor(
				sf::Color(colourRaw[0], colourRaw[1], colourRaw[2], colourRaw[3])
			);
		}


		if (!data.contains("width"))       data["width"] = _defaultWidth;
		if (!data.contains("height"))  data["height"] = _defaultHeight;

		float width = data.value("width", _defaultWidth);
		float height = data.value("height", _defaultHeight);

		if (ImGui::InputFloat("width", &width)) data["width"] = width;
		if (ImGui::InputFloat("height", &height)) data["height"] = height;
	}

	virtual void getInspectorParams() override {

		ImGui::SeparatorText("Rect Renderer");
		sf::Color curCol = _rectShape->getFillColor();
		float colourRaw[4] = { curCol.r, curCol.g, curCol.b, curCol.a };
		if (ImGui::InputFloat4("Colour", colourRaw)) {
			_rectShape->setFillColor(
				sf::Color(colourRaw[0], colourRaw[1], colourRaw[2], colourRaw[3])
			);
		}


		sf::Vector2f size = _rectShape->getSize();
		float width = size.x, height = size.y;
		if (ImGui::InputFloat("Width", &width)) {
			if (width < 0.0f) width = 0.0f;
			SetSize({ width,height });
		}

		if (ImGui::InputFloat("Height", &height)) {
			if (height < 0) height = 0;
			SetSize({ width,height });
		}
	}


	virtual nlohmann::json SaveToJSON() const override {
		nlohmann::json data;
		sf::Vector2f size = _rectShape->getSize();
		data["type"] = "rectangle";
		data["colour"] = _rectShape->getFillColor();;
		data["width"] = size.x;
		data["height"] = size.y;
		return data;
	}
#endif


private:
	inline static bool registered = [] {
		RendererFactory::instance().Register("rectangle", &RectRenderer::Create);
		return true;
		}();

};