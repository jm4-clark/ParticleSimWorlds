#include "TextGO2D.h"
#include "DrawData2D.h"
#include "helper.h"
#include "GameData.h"

TextGO2D::TextGO2D(string _text)
{
	m_text = _text;
}


void TextGO2D::Tick(GameData* _GD)
{
	if (_GD->m_GS == GS_PLAY_MAIN_CAM)
	{
		if (_GD->m_keyboardState[DIK_W] & 0x80)
		{
			m_pos += (Vector2(0, -1));
		}
		if (_GD->m_keyboardState[DIK_S] & 0x80)
		{
			m_pos += (Vector2(0, 1));
		}
		if (_GD->m_keyboardState[DIK_D] & 0x80)
		{
			m_pos += (Vector2(1, 0));
		}
		if (_GD->m_keyboardState[DIK_A] & 0x80)
		{
			m_pos += (Vector2(-1, 0));
		}
	}
}


void TextGO2D::Draw(DrawData2D* _DD)
{
	//right click and "Go to Defintion/Declaration" to see other version of this in DXTK
	_DD->m_Font->DrawString(_DD->m_Sprites.get(), Helper::charToWChar(m_text.c_str()), m_pos, m_colour,m_rotation,m_origin,m_scale);
}