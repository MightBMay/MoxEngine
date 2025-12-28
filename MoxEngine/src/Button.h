#pragma once
#include "pch.h"
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
		uint64_t guid = data.value("guid", GenerateGUID());
		return std::make_unique<Button>(guid, enabled, data.value("onclicktext", "onclickDefault\n"));
	}

	virtual void Start() override;

	virtual void Update(float deltaTime) override;


	Button(uint64_t guid, bool enabled, std::string onclicktext) :Component(guid), _onclicktext(onclicktext) { _enabled = enabled; }
	virtual std::string GetName() const override { static std::string name = "Button"; return  name; };

#if IN_EDITOR

	virtual void getImGuiParams(nlohmann::json& data) override;

	virtual void getInspectorParams() override;

	virtual nlohmann::json SaveToJSON() const override;

#endif

private:
	inline static bool registered = [] {// static bool with a lambda assignment. ensures this is calculated exactly once, at startup, to register for the SceneLoader's use later.
		ComponentFactory::instance().Register("Button", &Button::Create);
		return true;
		}();



};
