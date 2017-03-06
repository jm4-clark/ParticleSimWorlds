#include "Particle3D.h"


Particle3D::Particle3D(std::string _fileName, ID3D11Device * _GD)
{
	m_alive = false;
	m_life = 0.0f;



}

Particle3D::~Particle3D()
{
}

void Particle3D::Spawn(Vector3 _pos, float _life, float _angle, float _speed, Vector3 _scale)
{
	m_alive = true;
	m_pos = _pos;
}

void Particle3D::Tick(GameData * _GD)
{

}

void Particle3D::Draw(DrawData * _DD)
{
	if (m_alive) //only draw particles if they are alive
	{
		CMOGO::Draw(_DD);
	}

}
