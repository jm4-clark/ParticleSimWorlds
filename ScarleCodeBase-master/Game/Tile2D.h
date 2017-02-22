#pragma once
#include "ImageGO2D.h"
#include "GameData.h"

class Tile2D : public GameObject2D
{
public:
	Tile2D(string _fileName, ID3D11Device* _GD);
	~Tile2D();

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);

private:
	ImageGO2D* sprite = nullptr;
};
