#ifndef FULLCLOCK_H
#define FULLCLOCK_H

#include <pebble.h>
#include "bitmaps.h"

Layer *full_clock_create(const Texts title, const GPoint r);
void full_clock_destroy(Layer *l);

void full_clock_set_time(Layer *l, const struct tm *time);


#endif /* FULLCLOCK_H */
