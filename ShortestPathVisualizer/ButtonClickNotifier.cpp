#include "ButtonClickNotifier.h"

#include "Button.h"

#include <memory>
#include <vector>

void ButtonClickNotifier::addObserver(onButtonClick observer) {
	_observers.push_back(observer);
}

void ButtonClickNotifier::notify(Button* button) {
	for (auto it = _observers.begin(); it != _observers.end(); ++it) {
		onButtonClick func = *it;
		func(button);
	}
}