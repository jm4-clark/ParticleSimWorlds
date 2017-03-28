#include "Particle3D.h"
#include "vertex.h"
#include <dinput.h>
#include "GameData.h"
#include "DrawData.h"
#include "camera.h"

Particle3D::Particle3D(string _fileName, ID3D11Device * _pd3dDevice, IEffectFactory* _EF)// : CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_alive = false;
	m_life = 0.0f;
	angleXYInRadians = 0.0f;
	m_scale = Vector3(0.0f, 0.0f, 0.0f);
	m_acc = Vector3(0.0f, 0.0f, 0.0f);

	int numVerts = 6; 
	myVertex* vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];

	for (int i = 0; i < numVerts; i++)
	{
		indices[i] = i;
		vertices[i].texCoord = Vector2::One;
	}
	Matrix stepTrans = Matrix::CreateTranslation(0.0f, 1.0f, 0.0f);
	Matrix rotTrans = Matrix::CreateRotationY(1.0f);
	Matrix scaleTrans = Matrix::CreateScale(m_scale);
	Matrix baseTrans = scaleTrans *  rotTrans * stepTrans;

	Matrix* transforms = new Matrix[2];
	transforms[0] = Matrix::Identity;
	for (int i = 1; i < 2; i++)
	{
		transforms[i] = transforms[i - 1] * baseTrans;
	}

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
	m_speed = _speed;
	m_scale = _scale;
	m_pitch = 0;
	m_gravity = _gravity;
	m_drag = _drag + _scale.x;
	if (m_gravity != 0)
	{
		m_physicsOn = true;
	}
	else
	{
		m_physicsOn = false;
	}
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
			m_pitch += _GD->m_dt;
			/*if (!m_physicsOn)
			{
				m_pos.x += m_acc.x *_GD->m_dt;
				m_pos.y += m_acc.y *_GD->m_dt;
				m_pos.z += m_acc.z *_GD->m_dt;
			}*/
			
		
		}
		else
		{
			m_alive = false;
		}

	}
	_GD->gravity *= m_gravity;
	VBGO::Tick(_GD);
	//CMOGO::Tick(_GD);
}

void Particle3D::Draw(DrawData * _DD)
{
	if (m_alive) //only draw particles if they are alive
	{
		//Matrix m_view = Matrix::CreateBillboard(m_pos, _DD->m_cam->GetPos(),Vector3::Up, &Vector3::Forward);
		//_DD->m_cam->SetView(m_view);
		//_DD->m_pd3dImmediateContext->Draw(4, m_pos.x);
		
		VBGO::Draw(_DD);
		//CMOGO::Draw(_DD);
	}

}
