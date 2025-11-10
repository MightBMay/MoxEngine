#pragma once

template<typename... Args>
class Event {
private:
	std::vector<std::function<void(Args...)>> listeners;


public:
	void operator+=(const std::function<void(Args...)>& listener) {
		listeners.push_back(listener);
	}

    void operator-=(const std::function<void(Args...)>& listener) {
        listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
            [&](const std::function<void(Args...)>& f) {
                // Can't compare std::function directly, so we compare target pointers
                return f.target_type() == listener.target_type();
            }), listeners.end());
    }

        // Invoke all listeners
    void operator()(Args... args) {
        for (auto& listener : listeners) {
            listener(args...);
        }
    }

      // Clear all listeners
    void Clear() {
        listeners.clear();
    }
};