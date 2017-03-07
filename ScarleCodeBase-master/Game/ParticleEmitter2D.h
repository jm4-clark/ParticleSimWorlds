#pragma once
#include "GameData.h"
#include "GameObject2D.h"
#include <list>

class Particle;

class ParticleEmitter2D : public GameObject2D
{
public:
	ParticleEmitter2D(ID3D11Device* _pd3dDevice, string _fileName, 
		float _x, float _y, float _life, float _lifeVar, float _angle, float _angleVar, float _speed, float _speedVar, float _size, float _sizeVar, 
		Color _colour, Color _endColour, float _drag, float _xGravity, float _yGravity, int _particleNum);
	~ParticleEmitter2D();
	list<Particle*> getParticles();


	virtual void Draw(DrawData2D* _DD);
	virtual void Tick(GameData* _GD);

	void SpawnParticles(float randLife, float randAngle, float randSpeed, float randSize);
protected:
	list<Particle*> m_particles;

	float x;
	float y;
	float life, minLife, maxLife;
	float speed, minSpeed, maxSpeed;
	float angle, minAngle, maxAngle;
	float scale, minScale, maxScale;
	Color colour, endColour;
	float drag;
	float xGravity, yGravity;

	bool onOff;
};