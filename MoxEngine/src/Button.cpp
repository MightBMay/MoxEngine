#include "pch.h"
#include "Component.h"
#include "Button.h"

#include "imgui.h"


Event<>& Button::GetOnClick() { return _onClick; }
Event<>& Button::GetOnRelease() { return _onRelease; }
Event<>& Button::GetOnHover() { return _onHover; }
Event<>& Button::GetOnHoverExit() { return _onHoverExit; }

void Button::Start() {
	_onClick.Clear();
	_onRelease.Clear();
	_onHover.Clear();
	_onHoverExit.Clear();


	_renderer = _parent->getRenderer();

	_onClick += [this]() {isClicked = true; };

	_onRelease += [this]() {isClicked = false; };

	_onClick += [this]() {oc(); };

	_onRelease += [this]() {oR(); };
	_onHover += [this]() {oh(); };
	_onHoverExit += [this]() {ohe(); };

}



void Button::Update(float deltaTime) {


	auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	bool isHovering = _renderer->IsPointInside(mousePos);


	if (isHovering && !wasHovering) _onHover(); 

	if (!isHovering && wasHovering) _onHoverExit();

	if (isHovering && Input::GetMouseDown(0)) _onClick();	// only clickable if you are currently hovering.

	if (isClicked && Input::GetMouseUp(0)) _onRelease(); // only release button if it was already clicked.


	wasHovering = isHovering;





}


#if IN_EDITOR

void Button::getImGuiParams(nlohmann::json& data) {
	if (!data.contains("onclicktext")) data["onclicktext"] = _onclicktext;

	std::array<char, 64> buffer{};
	std::string temp = data["onclicktext"].get<std::string>();
	std::copy_n(temp.begin(), std::min(temp.size(), buffer.size() - 1), buffer.begin());

	if (ImGui::InputText("On Click Text", buffer.data(), buffer.size())) {
		data["onclicktext"] = std::string(buffer.data());
	}
}


void Button::getInspectorParams()  {
// Create a local buffer for ImGui input
	std::array<char, 512> buffer{};
	// Copy current value of _onclicktext into buffer
	std::copy_n(_onclicktext.begin(), std::min(_onclicktext.size(), buffer.size() - 1), buffer.begin());

	// InputText modifies buffer in-place
	std::string label = "On Click Text##" + std::to_string(reinterpret_cast<uintptr_t>(this));
	if (ImGui::InputText(label.c_str(), buffer.data(), buffer.size())) {
		// Write back changes into member variable
		_onclicktext = std::string(buffer.data());
	}
}

nlohmann::json Button::SaveToJSON() const {
	nlohmann::json data;
	data["type"] = GetName();
	data["guid"] = GetGUID();
	data["onclicktext"] = _onclicktext;
	return data;
}

#endif