#include "ParticleEmitter3D.h"

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
