#pragma once
#include "GameData.h"
#include "CMOGO.h"
#include "VBGO.h"
#include "vertex.h"
#include <string>
#include "TPSCamera.h"

class Particle3D : public VBGO
{
public:
	Particle3D(std::string _fileName, ID3D11Device * _GD, IEffectFactory* _EF, TPSCamera* _camPos);
	~Particle3D();

	void Spawn(Vector3 _pos, float _life, float _angleXY, float _angleZ, float _speed, Vector3 _scale, float _drag, float _gravity);

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData* _DD);

	bool isAlive() { return m_alive; };

protected:
	
	bool m_alive;
	float m_life;
	float m_originalLife;
	float m_speed;
	float angleXYInRadians;
	float m_gravity;
	Color colour;

	TPSCamera* m_camPos;

	int xPosVar;
	float yPosVar = 0;

	myVertex* m_vertices;
	int m_size;


	//ImageGO2D* sprite = nullptr;
};