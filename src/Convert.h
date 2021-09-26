#ifndef CONVERT_H
#define CONVERT_H

double convertClockTicksToSeconds(clock_t clk)
{
	return clk / (double)CLOCKS_PER_SEC;
}

#endif // CONVERT_H