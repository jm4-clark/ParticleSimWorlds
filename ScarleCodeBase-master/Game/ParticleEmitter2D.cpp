#include "ParticleEmitter2D.h"
#include "DDSTextureLoader.h"
#include "DrawData2D.h"
#include "helper.h"
#include "Particle.h"
#include <DirectXColors.h>
#include <time.h>

ParticleEmitter2D::ParticleEmitter2D(ID3D11Device* _pd3dDevice, string _fileName, 
	float _x, float _y, float _minLife, float _maxLife, float _angleA, float _angleB, float _minSpeed, float _maxSpeed, float _minSize, float _maxSize, int _particleNum)
{
	for (int i = 0; i < _particleNum; i++)
	{
		m_particles.push_back(new Particle(_fileName, _pd3dDevice));
	}

	x = _x;
	y = _y;
	minLife = _minLife;
	maxLife = _maxLife;
	minSpeed = _minSpeed;
	maxSpeed = _maxSpeed;
	minScale = _minSize;
	maxScale = _maxSize;
	angleA = _angleA;
	angleB = _angleB;
}


list<Particle*> ParticleEmitter2D::getParticles()
{
	return m_particles;
}

void ParticleEmitter2D::Tick(GameData* _GD)
{
	float randLife = minLife + (rand()) / (RAND_MAX / (maxLife-minLife));
	float randAngle = angleA + (rand()) / (RAND_MAX / (angleB - angleA));;
	float randSpeed = minSpeed + (rand()) / (RAND_MAX / (maxSpeed - minSpeed));
	float randSize = minScale + (rand()) / (RAND_MAX / (maxScale - minScale));
	
	if ((_GD->m_keyboardState[DIK_Z] & 0x80) && !(_GD->m_prevKeyboardState[DIK_Z] & 0x80))
	{
		for (list<Particle *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
		{
			if (!(*it)->isAlive())
			{
				(*it)->Spawn(x, y, randLife, randAngle, randSpeed, Vector2(randSize, randSize));
				break;
			}
		}
	}

	if (_GD->m_keyboardState[DIK_X] & 0x80)
	{
		for (list<Particle *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
		{
			if (!(*it)->isAlive())
			{
				(*it)->Spawn(x, y, randLife, randAngle, randSpeed, Vector2(randSize, randSize));
				break;
			}
		}
	}

	for (list<Particle *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
	{
		(*it)->Tick(_GD);
	}
}

void ParticleEmitter2D::Draw(DrawData2D* _DD)
{
	for (list<Particle *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
	{
		(*it)->Draw(_DD);
	}
}


ParticleEmitter2D::~ParticleEmitter2D()
{

}

