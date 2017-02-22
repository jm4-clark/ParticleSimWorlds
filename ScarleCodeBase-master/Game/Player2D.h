#pragma once
#include "ImageGO2D.h"
#include "Actor2D.h"

class Player2D : public Actor2D	//Might not be possible to inherit from this
{
public:
	Player2D(string _fileName, ID3D11Device* _GD, bool is_affected_by_gravity);
	~Player2D();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);

private:
	ImageGO2D* sprite = nullptr;
	int moveSpeed = 3;
};