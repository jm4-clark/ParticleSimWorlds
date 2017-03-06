#pragma once

#include <d3d11_1.h>
#include <DirectXMath.h>

#include "SimpleMath.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

struct VertexIn
{
	Vector3 position;
	float size;
};

struct ParticlePointSprite
{
	Vector3 position;
	float size;

	//int stride = 
};

class InputLayoutDescriptions
{
	/*static InputElement[] ParticlePointSprite = {
		new InputElement("POSITION", 0, Format.R32G32B32_Float, 0, 0, D3D10_INPUT_CLASSIFICATION.PerVertexData, 0),
		new InputElement("SIZE", 0, Format.R32G32B32_Float, 12, 0, D3D10_INPUT_CLASSIFICATION>D3D10_INPUT_PER_VERTEX_DATA, 0)
	}*/
};

//http://richardssoftware.net/Home/Post/19