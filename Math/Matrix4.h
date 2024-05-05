#pragma once

#include "Vector4.h"

class CMatrix4 
{
public:
	float Get(int element) { return m_matrix[element]; }
	auto &GetData() { return m_matrix; }
private:
	float m_matrix[4 * 4] = 
	{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
};