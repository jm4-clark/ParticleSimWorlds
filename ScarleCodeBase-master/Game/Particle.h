#pragma once
#include "ImageGO2D.h"
#include "GameData.h"


class Particle : public GameObject2D
{
public:
	Particle(string _fileName, ID3D11Device * _GD);
	~Particle();

	void Spawn(float x, float y, float life, float angle, float speed, Vector2 size, Color colour, Color endColour, float drag, float xGravity, float yGravity);

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);

	bool isAlive();

protected:
	bool  m_alive;
	float m_life;
	float m_originalLife;
	float m_speed;
	float angleInRadians;
	Vector2 m_originalScale;
	ImageGO2D* sprite = nullptr;
	float m_alpha;
	float m_originalAlpha;
	bool m_xGravity, m_yGravity;
	Color m_originalColour, m_endColour;

};