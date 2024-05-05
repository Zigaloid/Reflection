#pragma once

class CVector4 
{
public:
	float Get(int element) { return m_vector[element]; }
	auto& GetData() { return m_vector; }
private:
	float m_vector[4] = { 0.0f,0.0f,0.0f,0.0f };
};