#ifndef MODMATRIX_H
#define MODMATRIX_H

#include "../music.h"
#include "modulator.h"
#include "envelope.h"
#include "lfo.h"
#include "../input.h"
#include "../osc.h"

void mod_init_mod_connections(void);
void mod_init_poti_connections(inputState_t *in);
#endif