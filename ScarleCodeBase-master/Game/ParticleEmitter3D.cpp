#include "ParticleEmitter3D.h"
#include "Particle3D.h"
#include "Helper.h"

ParticleEmitter3D::ParticleEmitter3D(string _fileName, ID3D11Device * _GD, IEffectFactory* _EF,
	Vector3 _pos, float _life, float _lifeVar, float _angle, float _angleVar, float _speed, float _speedVar, float _size, float _sizeVar, int _particleNum) : CMOGO(_fileName, _GD, _EF)
{
	for (int i = 0; i < _particleNum; i++)
	{
		m_particles.push_back(new Particle3D(_fileName, _GD, _EF));
	}
	m_pos = _pos;
	x = _pos.x;
	y = _pos.y;
	z = _pos.z;
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
}

std::list<Particle3D*> ParticleEmitter3D::getParticles()
{
	return m_particles;
}

void ParticleEmitter3D::Draw(DrawData * _DD)
{
	for (std::list<Particle3D *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
	{
		(*it)->Draw(_DD);
	}
}

void ParticleEmitter3D::Tick(GameData * _GD)
{
	float randLife = minLife + (rand()) / (RAND_MAX / (maxLife - minLife));
	float randAngle = minAngle + (rand()) / (RAND_MAX / (maxAngle - minAngle));;
	float randSpeed = minSpeed + (rand()) / (RAND_MAX / (maxSpeed - minSpeed));
	float randSize = minScale + (rand()) / (RAND_MAX / (maxScale - minScale));

	if (((_GD->m_keyboardState[DIK_B] & 0x80) && !(_GD->m_prevKeyboardState[DIK_B] & 0x80)) && onOff == false) //spawn one particle
	{
		for (list<Particle3D *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
		{
			if (!(*it)->isAlive())
			{
				(*it)->Spawn(Vector3(x, y, z), randLife, randAngle, randSpeed, Vector3(randSize, randSize, randSize));
				break;
			}
		}
	}

	if ((_GD->m_keyboardState[DIK_N] & 0x80) && onOff == false) //spawn all particles at once
	{
		for (list<Particle3D *>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
		{
			if (!(*it)->isAlive())
			{
				(*it)->Spawn(Vector3(x, y, z), randLife, randAngle, randSpeed, Vector3(randSize, randSize, randSize));
				break;
			}
		}
	}

	if ((_GD->m_keyboardState[DIK_M] & 0x80) && !(_GD->m_prevKeyboardState[DIK_M] & 0x80))
	{
		onOff = !onOff;
	}

	if (onOff == true)
	{
		for (list<Particle3D *>::iterator it = m_particles.begin(); it != m_particles.end(); it++) //once toggled on, continually spawns particles
		{
			if (!(*it)->isAlive())
			{
				(*it)->Spawn(Vector3(x, y, z), randLife, randAngle, randSpeed, Vector3(randSize, randSize, randSize));
				break;
			}
		}
	}

	for (list<Particle3D *>::iterator it = m_particles.begin(); it != m_particles.end(); it++) //tick through all particles
	{
		(*it)->Tick(_GD);
	}
}
