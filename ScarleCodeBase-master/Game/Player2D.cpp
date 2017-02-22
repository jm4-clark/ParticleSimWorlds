#include "Player2D.h"

Player2D::Player2D(string _fileName, ID3D11Device* _GD, bool gravity_bool)
{
	sprite = new ImageGO2D(_fileName, _GD);
	SetPhysicsOn(true);
	SetDrag(0.5);
	gravity_affected = gravity_bool;
}


Player2D::~Player2D()
{
	delete sprite;
}

void Player2D::Tick(GameData* _GD)
{
	float speed = 40.0f;
	if (gravity_affected)
	{
		m_acc = Vector2(0, 50.0f);
	}

	if (_GD->m_GS == GS_PLAY_MAIN_CAM)
	{
		if (_GD->m_keyboardState[DIK_W] & 0x80)
		{
			m_vel.y = -speed;
			//m_acc.y -= speed;
		}
		if (_GD->m_keyboardState[DIK_S] & 0x80)
		{
			m_vel.y = speed;
			//m_acc.y += speed;
		}
		if (_GD->m_keyboardState[DIK_D] & 0x80)
		{
			m_vel.x = speed;
			//m_acc.x += speed;
		}
		if (_GD->m_keyboardState[DIK_A] & 0x80)
		{
			m_vel.x = -speed;
			//m_acc.x -= speed;
		}
	}

	GameObject2D::Tick(_GD);

	//check collision with bottom of screen (change later to "solid" objects)
	if ((m_pos.y + sprite->getHeight()*m_scale.y) > _GD->window_height)
	{
		m_vel.y = 0;
	}
}

void Player2D::Draw(DrawData2D* _DD)
{
	//nullptr can be changed to a RECT* to define what area of this image to grab
	//you can also add an extra value at the end to define layer depth
	//right click and "Go to Defintion/Declaration" to see other version of this in DXTK
	_DD->m_Sprites->Draw(sprite->getTexture(), m_pos, nullptr, m_colour, m_rotation, m_origin, m_scale, SpriteEffects_None);
}