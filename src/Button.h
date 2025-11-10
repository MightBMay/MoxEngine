#pragma once
#include "Global.h"
#include "Event.h"
class Button : public Component {

protected:
	Renderer* _renderer;
	/// <summary>
	/// was the cursor hovering this component's renderer last update.
	/// </summary>
	bool wasHovering = false;

	Event<> _onClick;
	Event<> _onRelease;
	Event<> _onHover;
	Event<> _onHoverExit;


	void oc() { std::cout << "onclick"; }
	void oR() { std::cout << "onrelease"; }
	void oh() { std::cout << "onhover"; }
	void ohe() { std::cout << "onhoverexit"; }

public:
	Event<>& GetOnClick() { return _onClick; }
	Event<>& GetOnRelease() { return _onRelease; }
	Event<>& GetOnHover() { return _onHover; }
	Event<>& GetOnHoverExit() { return _onHoverExit; }


	virtual void Start() { 
		_renderer = _parent->getRenderer();


		_onClick += [this]() {oc(); };
		_onRelease += [this]() {oR(); };
		_onHover += [this]() {oh(); };
		_onHoverExit += [this]() {ohe(); };

	}



	virtual void Update(float deltaTime) {
	

		auto mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		bool isHovering = _renderer->IsPointInside(mousePos);


		if (isHovering && !wasHovering) {
			_onHover();
			
		}

		if (!isHovering && wasHovering) {
			_onHoverExit();
		}


		wasHovering = isHovering;



		
	
	}


};
