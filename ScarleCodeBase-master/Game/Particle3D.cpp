#include "Particle3D.h"
#include <dinput.h>
#include "GameData.h"
#include "DrawData.h"
#include "camera.h"

Particle3D::Particle3D(string _fileName, ID3D11Device * _pd3dDevice, IEffectFactory* _EF) : CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_alive = false;
	m_life = 0.0f;
	angleXYInRadians = 0.0f;
	m_scale = Vector3(0.0f, 0.0f, 0.0f);
	m_acc = Vector3(0.0f, 0.0f, 0.0f);

	//sprite = new ImageGO2D(_fileName, _pd3dDevice);
}

Particle3D::~Particle3D()
{
}

void Particle3D::Spawn(Vector3 _pos, float _life, float _angleXY, float _angleZ, float _speed, Vector3 _scale, float _drag, float _gravity)
{
	m_alive = true;
	m_pos = _pos;
	m_originalLife = m_life = _life;
	angleXYInRadians = _angleXY * XM_PI / 180;

	m_acc = Vector3(_speed * cos(angleXYInRadians), -_speed * sin(angleXYInRadians), _speed * sin(_angleZ* XM_PI / 180));//-_speed *cos(angleInRadians));
	//m_speed = _speed;
	m_scale = _scale;

	m_gravity = _gravity;
	m_drag = _drag + _scale.x;
	m_physicsOn = true;
}

void Particle3D::Tick(GameData * _GD)
{
	//m_acc = Vector3(m_acc.x, m_acc.y - (_GD->gravity *m_drag * m_gravity), m_acc.z);

	if (m_alive)
	{
		m_life -= _GD->m_dt;

		if (m_life > 0)
		{
						
			float ageRatio = m_life / m_originalLife;
			//m_pos.x += m_acc.x * _GD->m_dt;
			//m_pos.y += m_acc.y * _GD->m_dt;
			//m_pos.z += m_acc.z * _GD->m_dt;
		
		}
		else
		{
			m_alive = false;
		}

	}
	
	CMOGO::Tick(_GD);
}

void Particle3D::Draw(DrawData * _DD)
{
	if (m_alive) //only draw particles if they are alive
	{
		//Matrix m_view = Matrix::CreateBillboard(m_pos, _DD->m_cam->GetPos(),Vector3::Up, &Vector3::Forward);
		//_DD->m_cam->SetView(m_view);
		//_DD->m_pd3dImmediateContext->Draw(4, m_pos.x);
		CMOGO::Draw(_DD);
	}

}
