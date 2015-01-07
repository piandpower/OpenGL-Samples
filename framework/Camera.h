#pragma once

#include <glm/glm.hpp>
#include "IComponent.h"

class NodeBase;

class Camera : public IComponent
{
public:
	Camera();

	void setProjection(float fovy, float aspectRatio, float near = 0.1f, float far = 1000.0f);

	void setPosition(const glm::vec3& pos);
	const glm::vec3& getPosition() const;

	void setRotation(const glm::quat& rot);
	const glm::quat& getRotation() const;

	void updateTransformMatrix();
	void updateViewMatrix();

	const glm::mat4& getTransformMatrix() const;

	const glm::mat4& getViewMatrix() const;
	const glm::mat4& getProjectionMatrix() const;

	const glm::mat4& getVP() const;

	void renderNodes(NodeBase* sceneNode);
	void lastUpdate();

	virtual void onUpdate();
	virtual void onLateUpdate();

	virtual void onAttached();

	virtual void onDettached();
private:
	struct Impl;
	Impl* dImpl;
};