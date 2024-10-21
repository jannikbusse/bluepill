 
 #include "modulator.h"
 
 void mod_init_mod_connection(mod_connection *mod, float *target_scalar, mod_connection_side side){
    mod->target_scalar = target_scalar;
    mod->side = side;
}