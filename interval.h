#ifndef INTERVAL_H
#define INTERVAL_H

typedef struct interval {
	double min;
	double max;
} interval_t;

static inline double interval_size(interval_t i) { return i.max - i.min; }

static inline int interval_contains(interval_t i, double x)
{
	return i.min <= x && x <= i.max;
}

static inline int interval_surrounds(interval_t i, double x)
{
	return i.min < x && x < i.max;
}

static inline double interval_clamp(interval_t i, double x)
{
	if (x < i.min)
		return i.min;
	if (x > i.max)
		return i.max;
	return x;
}

#endif
