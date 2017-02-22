#pragma once
#include "ImageGO2D.h"
#include "GameData.h"


class Particle : public GameObject2D
{
public:
	Particle(string _fileName, ID3D11Device * _GD, float x, float y, float life, float angle, float speed, Vector2 size);
	~Particle();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);

protected:
	float m_life;
	float m_originalLife;
	float m_speed;
	float angleInRadians;
	Vector2 m_originalScale;
	ImageGO2D* sprite = nullptr;

};