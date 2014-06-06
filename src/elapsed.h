/* Copyright (c) 2014, Klas Björkqvist, see COPYING */
#ifndef ELAPSED_H
#define ELAPSED_H

#include <pebble.h>

Layer *elapsed_create(const GPoint r, int ident);
void elapsed_destroy(Layer *l);

void elapsed_set_time(Layer *l, struct tm *time);

void elapsed_start_stop(Layer *l);
void elapsed_start(Layer *l);
void elapsed_stop(Layer *l);
void elapsed_reset(Layer *l);

#endif /* ELAPSED_H */

