#ifndef TIME_H
#define TIME_H

class Time
{
public:
	static void Init();
	static double GetTime();
	static double GetDeltaTime();
	static void Timestep(double deltaTime);
private:
	static Time* m_Inst;

	double m_Time;
	double m_DeltaTime;

	Time();
};

#endif // TIME_H
