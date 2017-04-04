#pragma once
#include "GameData.h"
#include "CMOGO.h"
#include "VBGO.h"
#include "Camera.h"
#include <list>

class Particle3D;

enum EmitterType : int
{
	POINTEMITTER = 0,
	CUBE = 1
};

class ParticleEmitter3D : public VBGO
{
public:
	ParticleEmitter3D(string _fileName, ID3D11Device * _GD, IEffectFactory* _EF, Camera* _camPos,
		Vector3 _pos, float _life, float _lifeVar, float _angleXY, float _angleXYVar, float _angleZ, float _angleZVar, float _speed, float _speedVar, float _size, float _sizeVar, float _drag, float _gravity, int _particleNum); //: VBGO(_fileName, _GD, _EF)
	ParticleEmitter3D(string _fileName, ID3D11Device * _GD, IEffectFactory* _EF, Camera* _camPos, 
		Vector3 _pos, Vector3 _minXYZ, Vector3 _maxXYZ, float _life, float _lifeVar, float _angleXY, float _angleXYVar, float _angleZ, float _angleZVar, float _speed, float _speedVar, float _size, float _sizeVar, float _drag, float _gravity, int _particleNum);
	~ParticleEmitter3D() = default;

	std::list<Particle3D*> getParticles();

	virtual void Draw(DrawData* _DD);
	virtual void Tick(GameData* _GD);

	int particleNum;

	void SetParticleNum(const void *num) { maxPNum = *static_cast<const int *>(num); }
 	int GetParticleNum() { return maxPNum; }

	void SetCurrentPNum(int _num) { currentParticleNum = _num; }
	int GetCurrentPNum() { return currentParticleNum; }

	void SetParticleCol(const void *col) { colour = *static_cast<const Color *>(col); }
	Color GetParticleCol() { return colour; }

	void SetPScale(float _scale) { scale = _scale; }
	float GetPScale() { return scale; }

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
	float scale, minScale, maxScale, scaleVar;
	float drag, gravity;
	bool onOff = true;
	Color colour;
	Quaternion quatRot;
	
	EmitterType emitterType;

	int currentParticleNum;
	int maxPNum;


};