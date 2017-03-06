#include "ParticleEmitter3D.h"
#include "Particle3D.h"

ParticleEmitter3D::ParticleEmitter3D(ID3D11Device * _pd3dDevice, std::string _fileName, Vector3 _pos, float _life, float _lifeVar, float _angle, float _angleVar)
{

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
