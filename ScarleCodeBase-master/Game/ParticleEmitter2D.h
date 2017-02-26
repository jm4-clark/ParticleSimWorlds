#pragma once
#include "GameData.h"
#include "GameObject2D.h"
#include <list>

class Particle;

class ParticleEmitter2D : public GameObject2D
{
public:
	ParticleEmitter2D(ID3D11Device* _pd3dDevice, string _fileName, 
		float _x, float _y, float _minLife, float _maxLife, float _angleA, float _angleB, float _minSpeed, float _maxSpeed, float _minSize, float _maxSize, int _particleNum);
	~ParticleEmitter2D();
	list<Particle*> getParticles();


	virtual void Draw(DrawData2D* _DD);
	virtual void Tick(GameData* _GD);
protected:
	list<Particle*> m_particles;

	float x;
	float y;
	float minLife;
	float maxLife;
	float minSpeed;
	float maxSpeed;
	float angleA;
	float angleB;
	float minScale;
	float maxScale;
};