#pragma once
#include "GameData.h"
#include "CMOGO.h"
#include <list>

class Particle3D;

class ParticleEmitter3D : public CMOGO
{
public:
	ParticleEmitter3D(string _fileName, ID3D11Device * _GD, IEffectFactory* _EF,
		Vector3 _pos, float _life, float _lifeVar, float _angle, float _angleVar, float _speed, float _speedVar, float _size, float _sizeVar, float _drag, float _gravity, int _particleNum);
	~ParticleEmitter3D() = default;

	std::list<Particle3D*> getParticles();

	virtual void Draw(DrawData* _DD);
	virtual void Tick(GameData* _GD);

protected:
	std::list<Particle3D*> m_particles;

	float x, y, z;
	float life, minLife, maxLife;
	float speed, minSpeed, maxSpeed;
	float angle, minAngle, maxAngle;
	float scale, minScale, maxScale;
	float drag, gravity;
	bool onOff;
};