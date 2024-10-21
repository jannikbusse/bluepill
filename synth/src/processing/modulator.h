#ifndef MODULATOR_H
#define MODULATOR_H


enum MOD_CONNECTION_SIDE {
    MOD_CONNECTION_SIDE_NO_MOD,
    MOD_CONNECTION_SIDE_CENTER,
    MOD_CONNECTION_SIDE_RIGHT,
    MOD_CONNECTION_SIDE_LEFT
};
typedef enum MOD_CONNECTION_SIDE mod_connection_side;

struct MOD_CONNECTION {
    float *target_scalar;
    mod_connection_side side;

};

typedef struct MOD_CONNECTION mod_connection;

//this modulates the value by: (1 * range) * target_scalar
inline float __attribute__((always_inline)) mod_modulate(mod_connection *mod, float value, float range) {
    switch (mod->side)
    {
    case MOD_CONNECTION_SIDE_NO_MOD:
        return value;
        break;
    case MOD_CONNECTION_SIDE_CENTER:
        return value + (range * ( *(mod->target_scalar) - 0.5f));
        break;
    case MOD_CONNECTION_SIDE_RIGHT:
        return value + (range * ( *(mod->target_scalar)));
        break;
    case MOD_CONNECTION_SIDE_LEFT:
        return value + (range * ( 1-*(mod->target_scalar)));
        break;
    }
    return value;
}

void mod_init_mod_connection(mod_connection *mod, float *target_scalar, mod_connection_side side);


#endif