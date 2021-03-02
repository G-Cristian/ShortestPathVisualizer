#pragma once

#include <memory>
#include <vector>

class Button;

typedef void (*onButtonClick)(Button*);

class ButtonClickNotifier
{
public:
	ButtonClickNotifier() = default;

	~ButtonClickNotifier() = default;

	void addObserver(onButtonClick);
	//I will not remove observers in this application
	//void removeObserver(onButtonClick);

	void notify(Button*);
private:
	std::vector<onButtonClick> _observers;
};

