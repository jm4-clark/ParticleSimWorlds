#pragma once
#include "GameObject2D.h"

class Actor2D : public GameObject2D
{
public:
	Actor2D();
	virtual ~Actor2D();

	//getters
	float GetMaxSpeed() { return m_maxSpeed; };
	float GetCurrentSpeed() { return m_currentSpeed; };

	//setters
	void SetMaxSpeed(float _maxSpeed) { m_maxSpeed = _maxSpeed; };
	void SetCurrentSpeed(float _currentSpeed) { m_currentSpeed = _currentSpeed; };

protected:
	float m_maxSpeed = 0;
	float m_currentSpeed;

};