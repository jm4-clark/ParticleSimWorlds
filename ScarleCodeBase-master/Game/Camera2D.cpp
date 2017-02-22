#include "camera2D.h"
#include "GameData.h"
#include "DrawData2D.h"

Camera2D::Camera2D(int _width, int _height, float _angle, DirectX::XMFLOAT3 _scaleFactors, GameObject2D * _target)
{
	width = _width;
	height = _height;
	angle = _angle;
	scaleFactors = _scaleFactors;

	target = _target;

	
	XMMatrixOrthographicLH(width, -height, 0.0f, 1.0f);
	XMMATRIX XMMatrixIdentity();
}

Camera2D::~Camera2D()
{

}

void Camera2D::Tick(GameData* _GD)
{
	int camX = width / 2;
	int	camY = height / 2;

	if (target)
	{
		camX = target->GetPos().x;
		camY = target->GetPos().y;
	}

	/*viewMatrix = XMMATRIX(
		scaleFactors.x * cos(angle), scaleFactors.x * sin(angle), 0, 0,
		-scaleFactors.y * sin(angle), scaleFactors.y * cos(angle), 0, 0,
		0, 0, scaleFactors.z, 0,
		-camX * scaleFactors.x * cos(angle) + camY * scaleFactors.y * sin(angle), -camX *scaleFactors.y * sin(angle) - camY * scaleFactors.y * cos(angle), 0, 1);*/
	m_viewMat
}

void Camera2D::Draw(DrawData2D * _DD)
{

}

void Camera2D::Follow(GameObject2D* _following)
{
	target = _following;
}

void Camera2D::Unfollow()
{
	target = nullptr;
}

bool Camera2D::isFollowing() const
{
	return target != nullptr;
}

void Camera2D::SetTransform(DrawData2D* _DD) const
{
	//m_pos = 
}