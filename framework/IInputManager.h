#pragma once

#include "Event.h"
#include <tuple>

#include <functional>

typedef std::function<void(const Event&)> InputEventCallback;

class IInputManager
{
public:
	IInputManager();
	virtual ~IInputManager();

	virtual int addEventListener(InputEventCallback eventCallback) = 0;
	virtual void removeEventListener(int id) = 0;

	virtual bool isKeyPressed(KeyCode keyCode) const = 0;

	virtual double getMousePosX() const = 0;
	virtual double getMousePosY() const = 0;
	virtual std::tuple<double, double> getMousePosition() const = 0;
	virtual float getMouseDeltaX() const = 0;
	virtual float getMouseDeltaY() const = 0;
	virtual std::tuple<float, float> getMouseDelta() const = 0;

	virtual bool isMouseButtonDown(MouseButtonCode buttonCode) const = 0;

	virtual void clearEventStates() = 0;

	virtual void setRelativeMouseMode(bool relativeMode) = 0;

	virtual bool getRelativeMouseMode() const = 0;
};