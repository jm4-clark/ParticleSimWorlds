#pragma once
#include "ImageGO2D.h"
#include "GameData.h"
#include "Actor2D.h"

class Enemy : public Actor2D
{
public:
	Enemy(string _fileName, ID3D11Device* _GD, Vector2 startPoint, Vector2 endPoint);
	~Enemy();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);

protected:
	ImageGO2D* sprite = nullptr;
	int moveSpeed = 2;

	Vector2 start;
	Vector2 end;
	Vector2 moveGradient;	//possible rename in future
	float distanceToMove;
	float distanceMoved = 0;
	bool startToEnd = true; //describes the direction the enemy is moving between start and end
};