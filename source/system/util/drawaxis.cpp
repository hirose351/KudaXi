#include	<DirectXMath.h>
#include	<vector>
#include	"line.h"
#include	"../dx11/Shader.h"


void drawaxis(const XMFLOAT4X4 &_mtx, float _length, const XMFLOAT3 &_pos) {
	// ê¸èâä˙âª
	static Line line;
	std::vector<Line::MyVertex> v;
	v.resize(2);

	static bool first = true;

	if (first)
	{
		// èâä˙âª
		v[0].vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);
		v[1].vertex = XMFLOAT3(0.0f, 0.0f, 0.0f);

		v[0].color = v[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);

		line.Init(v);

		first = false;
	}

	// Xé≤
	v[0].vertex.x = _pos.x + _mtx._11*-_length / 2.0f;
	v[0].vertex.y = _pos.y + _mtx._12*-_length / 2.0f;
	v[0].vertex.z = _pos.z + _mtx._13*-_length / 2.0f;

	v[1].vertex.x = _pos.x + _mtx._11*_length / 2.0f;
	v[1].vertex.y = _pos.y + _mtx._12*_length / 2.0f;
	v[1].vertex.z = _pos.z + _mtx._13*_length / 2.0f;

	v[0].color = v[1].color = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	line.SetVertex(v);
	line.Draw();

	// Yé≤
	v[0].vertex.x = _pos.x + _mtx._21*-_length / 2.0f;
	v[0].vertex.y = _pos.y + _mtx._22*-_length / 2.0f;
	v[0].vertex.z = _pos.z + _mtx._23*-_length / 2.0f;

	v[1].vertex.x = _pos.x + _mtx._21*_length / 2.0f;
	v[1].vertex.y = _pos.y + _mtx._22*_length / 2.0f;
	v[1].vertex.z = _pos.z + _mtx._23*_length / 2.0f;

	v[0].color = v[1].color = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);

	line.SetVertex(v);
	line.Draw();

	// Zé≤
	v[0].vertex.x = _pos.x + _mtx._31*-_length / 2.0f;
	v[0].vertex.y = _pos.y + _mtx._32*-_length / 2.0f;
	v[0].vertex.z = _pos.z + _mtx._33*-_length / 2.0f;

	v[1].vertex.x = _pos.x + _mtx._31*_length / 2.0f;
	v[1].vertex.y = _pos.y + _mtx._32*_length / 2.0f;
	v[1].vertex.z = _pos.z + _mtx._33*_length / 2.0f;

	v[0].color = v[1].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	line.SetVertex(v);
	line.Draw();
}