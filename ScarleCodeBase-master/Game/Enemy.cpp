#include "Enemy.h"
#include "DDSTextureLoader.h"
#include "DrawData2D.h"
#include "helper.h"
#include <time.h>

Enemy::Enemy(string _fileName, ID3D11Device * _GD, Vector2 startPoint, Vector2 endPoint)
{
	sprite = new ImageGO2D(_fileName, _GD);
	start = startPoint;
	end = endPoint;
	
	//create a unit vector to act as the velocity
	moveGradient = end - start;
	moveGradient.Normalize();

	distanceToMove = Vector2::Distance(start, end);
	m_pos = start;

}

Enemy::~Enemy()
{
	delete sprite;
}

void Enemy::Tick(GameData * _GD)
{	
	if (startToEnd)
	{
		m_pos += moveGradient * moveSpeed;
		distanceMoved = Vector2::Distance(m_pos, start);
	}
	else
	{
		m_pos -= moveGradient * moveSpeed;
		distanceMoved = Vector2::Distance(m_pos, end);
	}

	if (startToEnd && distanceMoved >= distanceToMove)
	{
		startToEnd = false;
		distanceMoved = 0;
	}
	else if ((!startToEnd) && distanceMoved >= distanceToMove)
	{
		startToEnd = true;
		distanceMoved = 0;
	}
	
}

void Enemy::Draw(DrawData2D * _DD)
{
	//nullptr can be changed to a RECT* to define what area of this image to grab
	//you can also add an extra value at the end to define layer depth
	//right click and "Go to Defintion/Declaration" to see other version of this in DXTK
	_DD->m_Sprites->Draw(sprite->getTexture(), m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
}
