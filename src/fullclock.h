/* Copyright (c) 2014, Klas Björkqvist, see COPYING */
#ifndef FULLCLOCK_H
#define FULLCLOCK_H

#include <pebble.h>
#include "bitmaps.h"

Layer *full_clock_create(const GPoint r, bool utc);
void full_clock_destroy(Layer *l);

void full_clock_set_time(Layer *l, struct tm *time);


#endif /* FULLCLOCK_H */
