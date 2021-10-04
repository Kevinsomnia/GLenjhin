#ifndef TIME_H
#define TIME_H

class Time
{
public:
	static void init();
	static double getTime();
	static double getDeltaTime();
	static void timestep(double deltaTime);
private:
	static Time* m_Inst;

	double m_Time;
	double m_DeltaTime;

	Time();
};

#endif // TIME_H
