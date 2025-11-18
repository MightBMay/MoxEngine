#pragma once
#include "pch.h"
#include "TextureManager.h"


#if  IN_EDITOR


#endif //  IN_EDITOR

#define missing "../assets/sprites/missingTexture.png"

class SpriteRenderer : public Renderer {


private:
	std::unique_ptr<sf::Sprite> _sprite;
	sf::Texture* _texture;
	std::string _spritePath = "../assets/sprites/missingTexture.png";

public:
	
	static std::unique_ptr<Renderer> Create(const nlohmann::json& data) {
		std::string spritePath = data.value("spritePath", missing);
		sf::Color colour = data.value("colour", sf::Color::White);
		sf::IntRect rect = data.value("textureRect", sf::IntRect());
		bool repeatTexture = data.value("isRepeated", false);


		return std::make_unique<SpriteRenderer>(spritePath, rect, colour, repeatTexture);
	}


	SpriteRenderer(std::string& spritePath,sf::IntRect textureRect, sf::Color colour, bool repeatTexture) {
		_spritePath = spritePath;

	
		_texture = TextureManager::get(spritePath);
		
		_texture->setRepeated(repeatTexture);
		_sprite = std::make_unique<sf::Sprite>(*_texture);
		_sprite->setTextureRect(textureRect);

		sf::Vector2f origin = { textureRect.size.x / 2.0f, textureRect.size.y / 2.0f };

		_sprite->setOrigin(origin);

		_drawable = _sprite.get();


		_sprite->setColor(colour);
		

	}

	virtual void SetOrigin() {
		sf::IntRect rect = _sprite->getTextureRect();
		_sprite->setOrigin(sf::Vector2f(rect.size.x / 2.0f, rect.size.y / 2.0f ));
	}
	virtual void SetOrigin(sf::Vector2i& newOrigin) {
		_sprite->setOrigin(sf::Vector2f(newOrigin.x, newOrigin.y));
	}

	virtual sf::FloatRect GetGlobalBounds() const override {
		return _transform->GetSFTransform().transformRect(_sprite->getGlobalBounds());
	}

#if IN_EDITOR

	virtual void getImGuiParams(nlohmann::json& data) override;

	virtual void getInspectorParams() override;

	virtual nlohmann::json SaveToJSON()const override;


#endif


private:
	inline static bool registered = [] {
		RendererFactory::instance().Register("sprite", &SpriteRenderer::Create);
		return true;
		}();

};