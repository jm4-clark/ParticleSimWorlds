#pragma once
#include "GameData.h"
#include "gameobject.h"
#include <list>

class Particle3D;

class ParticleEmitter3D : public GameObject
{
public:
	ParticleEmitter3D(ID3D11Device* _pd3dDevice, std::string _fileName,
		Vector3 _pos, float _life, float _lifeVar, float _angle, float _angleVar, float _speed, float _speedVar);
	~ParticleEmitter3D() = default;

	std::list<Particle3D*> getParticles();

	virtual void Draw(DrawData* _DD);
	virtual void Tick(GameData* _GD);

protected:
	std::list<Particle3D*> m_particles;

	float life;
	float speed;
};