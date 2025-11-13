#pragma once
#include "pch.h"
#include "imgui.h"
#include "Event.h"

class Button : public Component {

protected:
	Renderer* _renderer;
	/// <summary>
	/// was the cursor hovering this component's renderer last update.
	/// </summary>
	bool wasHovering = false;
	bool isClicked = false;
	Event<> _onClick;
	Event<> _onRelease;
	Event<> _onHover;
	Event<> _onHoverExit;

	std::string _onclicktext;


	void oc() { std::cout << _onclicktext; }
	void oR() { std::cout << "onrelease\n"; }
	void oh() { std::cout << "onhover\n"; }
	void ohe() { std::cout << "onhoverexit\n"; }

public:
	Event<>& GetOnClick();
	Event<>& GetOnRelease();
	Event<>& GetOnHover();
	Event<>& GetOnHoverExit();

	static std::unique_ptr<Component> Create(const nlohmann::json& data) {
		bool enabled = data.value("enabled", true);
		return std::make_unique<Button>(enabled, data.value("onclicktext", "onclickDefault\n"));
	}

	virtual void Start();

	virtual void Update(float deltaTime);


	Button(bool enabled, std::string onclicktext) :_onclicktext(onclicktext) { _enabled = enabled; }
	virtual std::string GetName() const override { static std::string name = "Button"; return  name; };


	virtual void getImGuiParams(nlohmann::json& data) override {
		if (!data.contains("onclicktext")) data["onclicktext"] = _onclicktext;

		std::array<char, 64> buffer{};
		std::string temp = data["onclicktext"].get<std::string>();
		std::copy_n(temp.begin(), std::min(temp.size(), buffer.size() - 1), buffer.begin());

		if (ImGui::InputText("On Click Text", buffer.data(), buffer.size())) {
			data["onclicktext"] = std::string(buffer.data());
		}
	}


	virtual void getInspectorParams() override {
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

	virtual nlohmann::json SaveToJSON() const override {
		nlohmann::json data;
		data["type"] = GetName();
		data["onclicktext"] = _onclicktext;
		return data;
	}

private:
	inline static bool registered = [] {// static bool with a lambda assignment. ensures this is calculated exactly once, at startup, to register for the SceneLoader's use later.
		ComponentFactory::instance().Register("Button", &Button::Create);
		return true;
		}();



};
