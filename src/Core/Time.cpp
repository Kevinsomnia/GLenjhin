#include "Time.h"

Time* Time::m_Inst = nullptr;

Time::Time()
{
	m_Time = 0.0;
	m_DeltaTime = 0.0;
}

void Time::Init()
{
	if (!m_Inst)
		m_Inst = new Time();
}

double Time::GetTime()
{
	return m_Inst->m_Time;
}

double Time::GetDeltaTime()
{
	return m_Inst->m_DeltaTime;
}

void Time::Timestep(double deltaTime)
{
	m_Inst->m_Time += deltaTime;
	m_Inst->m_DeltaTime = deltaTime;
}
