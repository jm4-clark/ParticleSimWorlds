#pragma once
#include "GameData.h"
#include "CMOGO.h"
#include "VBGO.h"
#include <string>

class Particle3D : public VBGO
{
public:
	Particle3D(std::string _fileName, ID3D11Device * _GD, IEffectFactory* _EF);
	~Particle3D();

	void Spawn(Vector3 _pos, float _life, float _angleXY, float _angleZ, float _speed, Vector3 _scale, float _drag, float _gravity);

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);

	bool isAlive() { return m_alive; };

protected:
	
	bool m_alive;
	float m_life;
	float m_originalLife;
	float m_speed;
	float angleXYInRadians;
	float m_gravity;


	//ImageGO2D* sprite = nullptr;
};