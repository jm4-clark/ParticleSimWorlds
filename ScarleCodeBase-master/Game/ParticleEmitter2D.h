#pragma once
#include "GameData.h"
#include "GameObject2D.h"
#include <list>

class Particle;

class ParticleEmitter2D : public GameObject2D
{
public:
	ParticleEmitter2D(ID3D11Device* _pd3dDevice, string _fileName, 
		float _x, float _y, float _life, float _lifeVar, float _angle, float _angleVar, float _speed, float _speedVar, float _size, float _sizeVar, Color _colour, int _particleNum);
	~ParticleEmitter2D();
	list<Particle*> getParticles();


	virtual void Draw(DrawData2D* _DD);
	virtual void Tick(GameData* _GD);
protected:
	list<Particle*> m_particles;

	float x;
	float y;
	float life;
	float speed;
	float angle;
	float scale;
	float minLife;
	float maxLife;
	float minSpeed;
	float maxSpeed;
	float minAngle;
	float maxAngle;
	float minScale;
	float maxScale;
	Color colour;

	bool onOff;
};