 
 #include "modulator.h"



 
 void mod_init_mod_connection(mod_connection *mod, float *target_scalar, float range, mod_connection_side side){
    mod->target_scalar = target_scalar;
    mod->side = side;
    mod->amount = 1;
    mod->range = range;

}

