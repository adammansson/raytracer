#ifndef INTERVAL_H
#define INTERVAL_H

typedef struct interval {
	float min;
	float max;
} interval_t;

static inline float interval_size(interval_t i) { return i.max - i.min; }

static inline int interval_contains(interval_t i, float x)
{
	return i.min <= x && x <= i.max;
}

static inline int interval_surrounds(interval_t i, float x)
{
	return i.min < x && x < i.max;
}

static inline float interval_clamp(interval_t i, float x)
{
	if (x < i.min)
		return i.min;
	if (x > i.max)
		return i.max;
	return x;
}

#endif
