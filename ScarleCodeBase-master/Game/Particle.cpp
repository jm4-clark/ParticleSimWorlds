#include "Particle.h"
#include "DDSTextureLoader.h"
#include "DrawData2D.h"
#include "helper.h"
#include <DirectXColors.h>

Particle::Particle(string _fileName, ID3D11Device * _GD)
{
	m_alive = false;
	m_life = 0.0f;
	angleInRadians = 0.0f;
	m_scale = Vector2(0.0f, 0.0f);
	m_vel = Vector2(0.0f, 0.0f);

	sprite = new ImageGO2D(_fileName, _GD);

	sprite->SetColour(Color(0, 0, 0));

}


void Particle::Spawn(float x, float y, float life, float angle, float speed, Vector2 size, Color colour, Color endColour, float drag, float xGravity, float yGravity)
{
	m_alive = true; //turn particle on
	m_pos.x = x; //set pos
	m_pos.y = y;
	m_originalLife = m_life = life; //set life
	angleInRadians = angle * XM_PI / 180;
	m_acc = Vector2((speed * cos(angleInRadians)), (-speed * sin(angleInRadians)));
	m_originalScale = m_scale = size;
	m_originalColour = m_colour = colour;
	m_endColour = endColour;
	m_originalAlpha = m_alpha = m_colour.w;

	m_xGravity = xGravity;
	m_yGravity = yGravity;
	//m_gravity = gravity;
	m_drag = drag * size.x;
}



Particle::~Particle()
{
	delete sprite;
}

bool Particle::isAlive()
{
	return m_alive;
}

void Particle::Tick(GameData * _GD)
{
	
	m_acc = Vector2(m_acc.x + (9.81 * m_xGravity), m_acc.y +(9.81 * m_yGravity));
	
	if (m_alive)
	{
		m_life -= _GD->m_dt;

		if (m_life > 0) //stuff to do when alive
		{
			float ageRatio = m_life / m_originalLife;
 			//m_alpha = m_originalAlpha * ageRatio;
			Color colorChange = Color((m_endColour.x - m_originalColour.x) / m_life,
				(m_endColour.y - m_originalColour.y) / m_life,
				(m_endColour.z - m_originalColour.z) / m_life,
				(m_endColour.w - m_originalColour.w) / m_life);
				//= (m_originalColour - m_endColour) / m_originalLife;

 			m_colour = Color(m_colour.x, m_colour.y, m_colour.z, m_alpha) + colorChange *_GD->m_dt; 
			

			m_pos.x += m_acc.x *_GD->m_dt;
			m_pos.y += m_acc.y * _GD->m_dt;
		}
		else //die when out of life
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

	if (m_alive) //only draw particles if they are alive
	{
		_DD->m_Sprites->Draw(sprite->getTexture(), m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
	}
}
