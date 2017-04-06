#include "Particle3D.h"
#include "vertex.h"
#include <dinput.h>
#include "GameData.h"
#include "DrawData.h"
#include "Camera.h"

Particle3D::Particle3D(string _fileName, ID3D11Device * _pd3dDevice, IEffectFactory* _EF, Camera* _camPos)// : CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_alive = false;
	m_life = 0.0f;
	angleXYInRadians = 0.0f;
	m_scale = Vector3(0.0f, 0.0f, 0.0f);
	m_acc = Vector3(0.0f, 0.0f, 0.0f);
	colour = Color(0.8f, 1.0f, 0.8f, 1.0f);
	
	m_camPos = _camPos;

	m_size = 3;

	int numVerts = 6 * 6 * (m_size - 1) * (m_size-1); 
	m_numPrims = numVerts / 3;
	myVertex* m_vertices = new myVertex[numVerts];
	WORD* indices = new WORD[numVerts];
	
	//set text-coords somewhere safe
	for (int i = 0; i < numVerts; i++)
	{
		indices[i] = i;
		m_vertices[i].texCoord = Vector2::One;
	}


	//build vertices using transforms
	int vert = 0;
	for (int i = -(m_size - 1) / 2; i<(m_size - 1) / 2; i++)
	{
		for (int j = -(m_size - 1) / 2; j < (m_size - 1) / 2; j++)
		{			
			////front
			m_vertices[vert].Color = colour;//Color(0.0f, 1.0f, 1.0f, 1.0f);
			m_vertices[vert++].Pos = Vector3((float)j, (float)i, -0.5f * (float)(m_size - 1));
			m_vertices[vert].Color = colour;//Color(0.0f, 1.0f, 1.0f, 1.0f);
			m_vertices[vert++].Pos = Vector3((float)j, (float)(i + 1), -0.5f * (float)(m_size - 1));
			m_vertices[vert].Color = colour;//Color(0.0f, 1.0f, 1.0f, 1.0f);
			m_vertices[vert++].Pos = Vector3((float)(j + 1), (float)i, -0.5f * (float)(m_size - 1));
			
			m_vertices[vert].Color = colour;//Color(0.0f, 1.0f, 1.0f, 1.0f);
			m_vertices[vert++].Pos = Vector3((float)j, (float)(i + 1), -0.5f * (float)(m_size - 1));
			m_vertices[vert].Color = colour;//Color(0.0f, 1.0f, 1.0f, 1.0f);
			m_vertices[vert++].Pos = Vector3((float)(j + 1), (float)(i + 1), -0.5f * (float)(m_size - 1));
			m_vertices[vert].Color = colour;//Color(0.0f, 1.0f, 1.0f, 1.0f);
			m_vertices[vert++].Pos = Vector3((float)(j + 1), (float)i, -0.5f * (float)(m_size - 1));
		}
	}

	for (int i = 0; i < m_numPrims; i++)
	{
		WORD V1 = 3 * i;
		WORD V2 = 3 * i + 1;
		WORD V3 = 3 * i + 2;

		Vector3 norm;
		Vector3 vec1 = m_vertices[V1].Pos - m_vertices[V2].Pos;
		Vector3 vec2 = m_vertices[V3].Pos - m_vertices[V2].Pos;
		norm = vec1.Cross(vec2);
		norm.Normalize();

		m_vertices[V1].Norm = norm;
		m_vertices[V2].Norm = norm;
		m_vertices[V3].Norm = norm;
	}


	BuildIB(_pd3dDevice, indices);
	BuildVB(_pd3dDevice, numVerts, m_vertices);

	delete[] indices;
	delete[] m_vertices;
	m_vertices = nullptr;
	//sprite = new ImageGO2D(_fileName, _pd3dDevice);
}

Particle3D::~Particle3D()
{
}

void Particle3D::Spawn(Vector3 _pos, Color _colour, float _life, float _angleXY, float _angleZ, float _speed, Vector3 _scale, float _drag, float _gravity, ParticleEmitter3D* _emitter)
{
	emitter = _emitter;
	m_alive = true;
	m_pos = _pos;
	m_originalLife = m_life = _life;
	angleXYInRadians = _angleXY * XM_PI / 180;

	m_acc = Vector3(_speed * cos(angleXYInRadians), -_speed * sin(angleXYInRadians), _speed * sin(_angleZ* XM_PI / 180));//-_speed *cos(angleInRadians));
	m_speed = _speed;
	m_scale = _scale;
	m_pitch = 0;
	m_gravity = _gravity;
	m_drag = _drag;// +_scale.x;
	colour = _colour;

	if (m_gravity != 0)
	{
		m_physicsOn = true;
	}
	else
	{
		m_physicsOn = false;
	}
	//int vert = 0;
	//for (int i = -(m_size - 1) / 2; i < (m_size - 1) / 2; i++)
	//{
	//	for (int j = -(m_size - 1) / 2; j < (m_size - 1) / 2; j++)
	//	{
	//		m_vertices[vert].Color = colour;//Color(0.0f, 1.0f, 1.0f, 1.0f);
	//		m_vertices[vert++].Color = colour;//Color(0.0f, 1.0f, 1.0f, 1.0f);
	//		m_vertices[vert++].Color = colour;//Color(0.0f, 1.0f, 1.0f, 1.0f);

	//		m_vertices[vert++].Color = colour;//Color(0.0f, 1.0f, 1.0f, 1.0f);
	//		m_vertices[vert++].Color = colour;//Color(0.0f, 1.0f, 1.0f, 1.0f);
	//		m_vertices[vert++].Color = colour;//Color(0.0f, 1.0f, 1.0f, 1.0f);
	//	}
	//}
}

void Particle3D::Tick(GameData * _GD)
{
	m_acc = Vector3(m_acc.x, m_acc.y - (m_drag * m_gravity), m_acc.z);
	
	Matrix m_billMat = Matrix::CreateBillboard(m_pos, m_camPos->GetPos(), m_camPos->GetUp(), &m_camPos->GetForward());
	Matrix  scaleMat = Matrix::CreateScale(m_scale);
	if (m_alive)
	{
		m_life -= _GD->m_dt;

		if (m_life > 0)
		{
						
			float ageRatio = m_life / m_originalLife;
			colour.w *= ageRatio;
		//	m_scale *= ageRatio;
			//m_roll += _GD->m_dt;
			//m_pitch += _GD->m_dt;
			if (!m_physicsOn)
			{
				m_pos.x += m_acc.x *_GD->m_dt;
				m_pos.y += m_acc.y *_GD->m_dt;
				m_pos.z += m_acc.z *_GD->m_dt;
			}
			else
			{
				Vector3 newVel = m_vel + _GD->m_dt * (m_acc - m_drag * m_vel);
				Vector3 newPos = m_pos + _GD->m_dt * m_vel;
				//newPos.y -= _GD->gravity * _GD->m_dt;
				m_vel = newVel;
				m_pos = newPos;
			}
			
			
			//m_pos.y += yPosVar;
		
		}
		else
		{
			m_alive = false;
			emitter->SetCurrentPNum(emitter->GetCurrentPNum() - 1);
		}

	}
	_GD->gravity *= m_gravity;
	
	m_worldMat = m_fudge * scaleMat * m_billMat;

	m_acc = Vector3::Zero;
	//VBGO::Tick(_GD);
	//CMOGO::Tick(_GD);
}

void Particle3D::Draw(DrawData * _DD)
{
	if (m_alive) //only draw particles if they are alive
	{
		
		//_DD->m_cam->SetView(m_view);
		_DD->m_pd3dImmediateContext->Draw(4, m_pos.x);
		
		VBGO::Draw(_DD);
		//CMOGO::Draw(_DD);
	}

}
