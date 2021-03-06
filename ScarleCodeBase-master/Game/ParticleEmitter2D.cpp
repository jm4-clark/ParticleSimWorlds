#include "ParticleEmitter2D.h"
#include "DDSTextureLoader.h"
#include "DrawData2D.h"
#include "helper.h"
#include "Particle.h"
#include <DirectXColors.h>
#include <time.h>

ParticleEmitter2D::ParticleEmitter2D(ID3D11Device* _pd3dDevice, string _fileName,
	float _x, float _y, float _life, float _lifeVar, float _angle, float _angleVar, float _speed, float _speedVar, float _size, float _sizeVar,
	Color _colour, Color _endColour, float _drag, float _xGravity, float _yGravity, int _particleNum)
{
	//initialise all particles
	for (int i = 0; i < _particleNum; i++)
	{
		m_particles.push_back(new Particle(_fileName, _pd3dDevice));
	}
	//set up all variables
	x = _x;
	y = _y;
	life = _life;
	speed = _speed;
	scale = _size;
	angle = _angle;
	minSpeed = speed - _speedVar;
	maxSpeed = speed + _speedVar;
	minLife = life - _lifeVar;
	maxLife = life + _lifeVar;
	minScale = scale - _sizeVar;
	maxScale = scale + _sizeVar;
	minAngle = angle - _angleVar;
	maxAngle = angle + _angleVar;
	colour = _colour;
	endColour = _endColour;
	drag = _drag;
	xGravity = _xGravity;
	yGravity = _yGravity;
	onOff = false;
}


list<Particle*> ParticleEmitter2D::getParticles()
{
	return m_particles;
}

void ParticleEmitter2D::Tick(GameData* _GD)
{
	float randLife = minLife + (rand()) / (RAND_MAX / (maxLife-minLife));
	float randAngle = minAngle + (rand()) / (RAND_MAX / (maxAngle - minAngle));;
	float randSpeed = minSpeed + (rand()) / (RAND_MAX / (maxSpeed - minSpeed));
	float randSize = minScale + (rand()) / (RAND_MAX / (maxScale - minScale));
	
	if (((_GD->m_keyboardState[DIK_Z] & 0x80) && !(_GD->m_prevKeyboardState[DIK_Z] & 0x80)) && onOff == false) //spawn one particle
	{
		for (list<Particle *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
		{
			if (!(*it)->isAlive())
			{
				(*it)->Spawn(x, y, randLife, randAngle, randSpeed, Vector2(randSize, randSize), colour, endColour, drag, xGravity, yGravity);
				break;
			}
		}
	}

	if ((_GD->m_keyboardState[DIK_X] & 0x80) && onOff == false) //spawn all particles at once
	{
		for (list<Particle *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
		{
			if (!(*it)->isAlive())
			{
				(*it)->Spawn(x, y, randLife, randAngle, randSpeed, Vector2(randSize, randSize), colour, endColour, drag, xGravity, yGravity);
				break;
			}
		}
	}

	if ((_GD->m_keyboardState[DIK_C] & 0x80) && !(_GD->m_prevKeyboardState[DIK_C] & 0x80)) 
	{
		onOff = !onOff;
	}

	if (onOff == true)//once toggled on, continually spawns particles
	{
		SpawnParticles(randLife, randAngle, randSpeed, randSize);
	}

	for (list<Particle *>::iterator it = m_particles.begin(); it != m_particles.end(); it++) //tick through all particles
	{
		(*it)->Tick(_GD);
	}
}

void ParticleEmitter2D::SpawnParticles(float randLife, float randAngle, float randSpeed, float randSize)
{
	for (list<Particle *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
	{
		if (!(*it)->isAlive())
		{
			(*it)->Spawn(x, y, randLife, randAngle, randSpeed, Vector2(randSize, randSize), colour, endColour, drag, xGravity, yGravity);
			break;
		}
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

