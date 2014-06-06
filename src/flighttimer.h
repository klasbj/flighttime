/* Copyright (c) 2014, Klas Björkqvist, see COPYING */
#ifndef FLIGHTTIMER_H
#define FLIGHTTIMER_H

#include <pebble.h>

Layer *flighttimer_create(const GPoint r);
void flighttimer_destroy(Layer *l);

void flighttimer_set_time(Layer *l, struct tm *time);

void flighttimer_start_stop(Layer *l);
void flighttimer_start(Layer *l);
void flighttimer_stop(Layer *l);
void flighttimer_reset(Layer *l);

#endif /* FLIGHTTIMER_H */

