#pragma once
#include "Event.h"


class Timer {

private:
	bool isPaused = false;
	float duration = 0;
	float remainingTime = 0.f;
	bool looping = true;

	Event<> _onTimerReset;
	Event<> _onTimerEnd;
	Event<bool> _onTimerPause;
	Event<> _onTimerTick;
public:

	
	Timer() = default;

	Timer(float duration ): duration(duration){}
	Timer(float duration, bool looping) : duration(duration), looping(looping) {}


	void Start() {
		_onTimerPause(false);
		isPaused = false;
		remainingTime = duration;
		_onTimerReset();
	}
	
	void Pause() {
		_onTimerPause(true);
		isPaused = true;
	}
	void Resume() {
		isPaused = false;
		_onTimerPause(false);
	}
	void Update(float deltaTime) {

		if (isPaused) return;

		if (remainingTime >= 0) {
			remainingTime -= deltaTime;
			_onTimerTick();
		}
		else if (looping) {
			remainingTime = duration;
			_onTimerReset();
		}
		else
		{
			_onTimerEnd();
			isPaused = true;
		}


	}

	void SetDuration(float newDuration, bool restart = true) {
		duration = newDuration;
		if (restart) {
			Start();
		}
	}

	bool IsPaused() const { return isPaused; }
	bool IsFinished() const { return remainingTime <= 0.f && !looping; }
	bool IsLooping() const { return looping; }

	float GetRemainingTime() const { return remainingTime; }
	float GetDuration() const { return duration; }

	Event<>& OnTimerReset() { return _onTimerReset; }
	Event<>& OnTimerEnd() { return _onTimerEnd; }
	Event<bool>& OnTimerPause() { return _onTimerPause; }
	Event<>& OnTimerTick() { return _onTimerTick; }

};