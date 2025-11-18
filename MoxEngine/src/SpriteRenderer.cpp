#include "pch.h"
#include "SpriteRenderer.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include "ImGuiFileDialog.h"
#include "TextureManager.h"


#if IN_EDITOR

void SpriteRenderer::getImGuiParams(nlohmann::json& data) {
	std::string curPath = data.value("spritePath", _spritePath);

	std::array<char, 512> pathBuffer{};
	std::copy_n(curPath.begin(), std::min(curPath.size(), pathBuffer.size() - 1), pathBuffer.begin());


	if (ImGui::InputText("Sprite Path", pathBuffer.data(), pathBuffer.size())) {
		std::string newPath = pathBuffer.data();


		sf::Texture temp;
		if (temp.loadFromFile(newPath)) {

			_texture->swap(temp);
			_sprite->setTexture(*_texture, true);

			_spritePath = newPath;
			data["spritePath"] = newPath;
		}

	}




	bool repeatTexture = _texture->isRepeated();
	if (ImGui::Checkbox("Repeat Texture", &repeatTexture)) {
		_texture->setRepeated(repeatTexture);
	}


	sf::IntRect raw = _sprite->getTextureRect();
	int rectValues[4] = { raw.position.x, raw.position.y, raw.size.x, raw.size.y };

	if (!data.contains("textureRect")) {
		data["textureRect"] = sf::IntRect(
			{ rectValues[0], rectValues[1] },
			{ rectValues[2], rectValues[3] }
		);
	}




	if (ImGui::InputInt4("Texture Rect", rectValues)) {
		auto newRect = sf::IntRect(
			{ rectValues[0], rectValues[1] },
			{ rectValues[2] , rectValues[3] }
		);

		data["textureRect"] = newRect;
	}





	//colour
	if (!data.contains("colour")) data["colour"] = { 255,255,255,255 };
	float colourRaw[4] = { 255,255,255,255 };
	if (ImGui::InputFloat4("Colour", colourRaw)) {
		data["colour"] = sf::Color(colourRaw[0], colourRaw[1], colourRaw[2], colourRaw[3]);

		_sprite->setColor(
			sf::Color(colourRaw[0], colourRaw[1], colourRaw[2], colourRaw[3])
		);
	}







}

void SpriteRenderer::getInspectorParams()  {

	ImGui::SeparatorText("Sprite Renderer");

#pragma region Texture Select Dialog



	IGFD::FileDialogConfig config;
	config.path = "../assets/sprites";
	config.countSelectionMax = 1;


	if (ImGui::Button("Select Texture")) {
		ImGuiFileDialog::Instance()->OpenDialog(
			"ChooseTextureDialog",
			"Choose Texture",
			".png,.jpg,.jpeg,.bmp,.tga,.dds",
			config
		);
	}


	if (ImGuiFileDialog::Instance()->Display("ChooseTextureDialog"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string newPath = ImGuiFileDialog::Instance()->GetFilePathName();

			 sf::Texture* tex = TextureManager::get(newPath);

			if (tex) {
				_texture = tex;
				_sprite->setTexture(*_texture, true);
				_spritePath = newPath;
				std::cout << "Path changed to: " + newPath;
			}
		}

		ImGuiFileDialog::Instance()->Close();
	}

	ImGui::SameLine();
	ImGui::Text(("Path: " + _spritePath).c_str());
#pragma endregion

#pragma region Texture Tiling



	bool repeatTexture = _texture->isRepeated();
	if (ImGui::Checkbox("Repeat Texture", &repeatTexture)) {
		_texture->setRepeated(repeatTexture);
	}

#pragma endregion

#pragma region Texture Rect





	auto rect = _sprite->getTextureRect();
	int rectValues[4]{
		rect.position.x,
		rect.position.y,
		rect.size.x,
		rect.size.y


	};

	if (ImGui::InputInt4("Texture Rect", rectValues)) {
		auto newRect = sf::IntRect(
			{ rectValues[0],rectValues[1] },
			{ rectValues[2] , rectValues[3] }
		);

		_sprite->setTextureRect(newRect);
		SetOrigin();
		std::cout << "changed";


	}
#pragma endregion

#pragma region Colour



	sf::Color curCol = _sprite->getColor();
	float colourRaw[4] = { curCol.r, curCol.g, curCol.b, curCol.a };
	if (ImGui::InputFloat4("Colour", colourRaw)) {
		_sprite->setColor(
			sf::Color(colourRaw[0], colourRaw[1], colourRaw[2], colourRaw[3])
		);
	}

#pragma endregion

}

nlohmann::json SpriteRenderer::SaveToJSON()const {
	nlohmann::json data;
	data["type"] = "sprite";
	data["colour"] = _sprite->getColor();
	data["spritePath"] = _spritePath;
	data["textureRect"] = _sprite->getTextureRect();
	data["isRepeated"] = _texture->isRepeated();

	return data;

}


#endif