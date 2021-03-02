#include "WindowClickNotifier.h"

#include "IClickable.h"

#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

void WindowClickNotifier::addObserver(std::shared_ptr<IClickable> observer) {
	_observers.push_back(observer);
}

void WindowClickNotifier::notify(float x, float y) {
	for (auto it = _observers.begin(); it != _observers.end(); ++it) {
		auto iClickable = *it;
		iClickable->onCLick(_window, x, y);
	}
}