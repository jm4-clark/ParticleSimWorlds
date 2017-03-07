#pragma once
#include "GameData.h"
#include "CMOGO.h"
#include <string>

class Particle3D : public CMOGO
{
public:
	Particle3D(std::string _fileName, ID3D11Device * _GD, IEffectFactory* _EF);
	~Particle3D();

	void Spawn(Vector3 _pos, float _life, float _angle, float _speed, Vector3 _scale, float _drag, float _gravity);

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);

	bool isAlive() { return m_alive; };

protected:
	
	bool m_alive;
	float m_life;
	float m_originalLife;
	float m_speed;
	float angleInRadians;
	float m_gravity;


	//ImageGO2D* sprite = nullptr;
};