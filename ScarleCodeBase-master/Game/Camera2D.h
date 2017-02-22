#ifndef _CAMERA2D_H_
#define _CAMERA2D_H_
#include "GameObject2D.h"
#include <DirectXMath.h>

struct GameData;

class Camera2D : public GameObject2D
{
public:
	//Camera2D(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, Vector2 _target = Vector2::Zero);
	Camera2D(int _width, int _height, float _angle, DirectX::XMFLOAT3 _scaleFactors, GameObject2D* _target);
	~Camera2D();

	virtual void Tick(GameData* _GD) override;
	virtual void Draw(DrawData2D* _DD) override;

	void Follow(GameObject2D* _following);
	void Unfollow();
	bool isFollowing() const;
	void SetTransform(DrawData2D* _DD) const;

protected:
	float angle;
	DirectX::XMFLOAT3 scaleFactors;
	Matrix m_orthoMat;
	//Matrix identityMatrix;
	XMMATRIX XMMatrixIdentity;
	Matrix m_viewMat;
	

	GameObject2D* target;

	int width;
	int height;

};

// www.youtube.com/watch?v=vn9az8950jE
#endif