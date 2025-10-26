#ifndef INTERVAL_H
#define INTERVAL_H

typedef struct interval {
	double min;
	double max;
} interval_t;

interval_t interval_new(double min, double max);
double interval_size(interval_t ival);
int interval_contains(interval_t ival, double x);
int interval_surrounds(interval_t ival, double x);
double interval_clamp(interval_t ival, double x);

#endif
