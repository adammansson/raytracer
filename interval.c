#include "interval.h"

interval_t interval_new(double min, double max)
{
	interval_t res;

	res.min = min;
	res.max = max;

	return res;
}

double interval_size(interval_t i) { return i.max - i.min; }

int interval_contains(interval_t i, double x)
{
	return i.min <= x && x <= i.max;
}

int interval_surrounds(interval_t i, double x)
{
	return i.min < x && x < i.max;
}

double interval_clamp(interval_t ival, double x)
{
	if (x < ival.min)
		return ival.min;
	if (x > ival.max)
		return ival.max;
	return x;
}
