#include "Tile2D.h"
#include "DDSTextureLoader.h"
#include "DrawData2D.h"
#include "Helper.h"

Tile2D::Tile2D(string _fileName, ID3D11Device* _GD)
{
	sprite = new ImageGO2D(_fileName, _GD);

}

Tile2D::~Tile2D()
{
	delete sprite;
}

void Tile2D::Tick(GameData* _GD)
{

}

void Tile2D::Draw(DrawData2D* _DD)
{
	//nullptr can be changed to a RECT* to define what area of this image to grab
	//you can also add an extra value at the end to define layer depth
	//right click and "Go to Defintion/Declaration" to see other version of this in DXTK
	_DD->m_Sprites->Draw(sprite->getTexture(), m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
}