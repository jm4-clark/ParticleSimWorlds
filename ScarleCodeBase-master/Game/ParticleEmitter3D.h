#pragma once
#include "GameData.h"
#include "CMOGO.h"
#include "VBGO.h"
#include "TPSCamera.h"
#include <list>

class Particle3D;

class ParticleEmitter3D : public VBGO
{
public:
	ParticleEmitter3D(string _fileName, ID3D11Device * _GD, IEffectFactory* _EF, TPSCamera* _camPos,
		Vector3 _pos, float _life, float _lifeVar, float _angleXY, float _angleXYVar, float _angleZ, float _angleZVar, float _speed, float _speedVar, float _size, float _sizeVar, float _drag, float _gravity, int _particleNum); //: VBGO(_fileName, _GD, _EF)
	~ParticleEmitter3D() = default;

	std::list<Particle3D*> getParticles();

	virtual void Draw(DrawData* _DD);
	virtual void Tick(GameData* _GD);

	int particleNum;

	void SetParticleNum(const void *num) { particleNum = *static_cast<const int *>(num); }
 	int GetParticleNum() { return m_particles.size(); }

	void SetParticleCol(const void *col) { colour = *static_cast<const Color *>(col); }
	Color GetParticleCol() { return colour; }

	void BuildEmitter();

	Quaternion RotationFromAxisAngle(const Vector3& axis, float angle);
	float Length(const Vector3& a);
	
protected:
	std::list<Particle3D*> m_particles;
	float x, y, z;
	float life, minLife, maxLife;
	float speed, minSpeed, maxSpeed;
	float angleXY, minAngleXY, maxAngleXY;
	float angleZ, minAngleZ, maxAngleZ;
	float scale, minScale, maxScale;
	float drag, gravity;
	bool onOff = true;
	Color colour;
	Quaternion quatRot;
};