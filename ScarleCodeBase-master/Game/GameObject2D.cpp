#include "GameObject2D.h"

using namespace DirectX;

GameObject2D::GameObject2D()
{
	m_colour = Color(1.0f, 1.0f, 1.0f, 1.0f);
	m_pos = Vector2::Zero;
	m_rotation = 0.0f;
	m_scale = Vector2::One;
}

GameObject2D::~GameObject2D()
{

}

void GameObject2D::Tick(GameData* _GD)
{
	if (m_physicsOn)
	{
		Vector2 newVel = m_vel + _GD->m_dt * (m_acc - m_drag * m_vel);
		Vector2 newPos = m_pos + _GD->m_dt * m_vel;

		m_vel = newVel;
		m_pos = newPos;	
	}

	m_acc = Vector2::Zero;
}