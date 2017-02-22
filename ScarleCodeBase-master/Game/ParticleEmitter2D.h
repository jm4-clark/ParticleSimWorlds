#pragma once
#include "GameData.h"
#include <list>

class ParticleEmitter2D : public GameObject2D
{
public:
	ParticleEmitter2D(ID3D11Device* _pd3dDevice);
	~ParticleEmitter2D();

	virtual void Draw();
	virtual void Tick();
protected:
	list<Particle*> m_particles;
};