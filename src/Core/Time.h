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

	Time();
	double m_Time;
	double m_DeltaTime;
};

#endif // TIME_H
