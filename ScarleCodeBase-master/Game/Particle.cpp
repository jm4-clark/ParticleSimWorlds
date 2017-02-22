#include "Particle.h"
#include "DDSTextureLoader.h"
#include "DrawData2D.h"
#include "helper.h"
#include <DirectXColors.h>

Particle::Particle(string _fileName, ID3D11Device * _GD, float x, float y, float life, float angle, float speed, Vector2 size)
{
	m_alive = false;
	m_pos.x = x;
	m_pos.y = y;

	m_originalLife = m_life = life;

	angleInRadians = angle * XM_PI / 180;

	m_originalScale = m_scale = size;

	m_vel = Vector2((speed * cos(angleInRadians)), (-speed * sin(angleInRadians)));

	sprite = new ImageGO2D(_fileName, _GD);

	//sprite->SetColour(Color(0, 0, 0));

}

Particle::~Particle()
{
	delete sprite;
}

void Particle::Tick(GameData * _GD)
{
	if (m_alive)
	{
		m_life -= _GD->m_dt;

		if (m_life > 0)
		{
			float ageRatio = m_life / m_originalLife;
			m_scale = m_originalScale * ageRatio;

			m_pos.x += m_vel.x * _GD->m_dt;
			m_pos.y += m_vel.y * _GD->m_dt;
		}
		else
		{
			m_alive = false;
		}
	}
}

void Particle::Draw(DrawData2D * _DD)
{
	//nullptr can be changed to a RECT* to define what area of this image to grab
	//you can also add an extra value at the end to define layer depth
	//right click and "Go to Defintion/Declaration" to see other version of this in DXTK
	if (m_alive) 
	{
		_DD->m_Sprites->Draw(sprite->getTexture(), m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
	}
}
