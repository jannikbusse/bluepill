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
    float amount;
    float range;
    mod_connection_side side;

};

typedef struct MOD_CONNECTION mod_connection;

 


/**
 * @brief Modulates a given value based on the modulation connection.
 *
 * This function applies modulation to the input value using the specified modulation connection.
 * The function is always inlined for performance reasons.
 *
 * @param mod Pointer to the modulation connection structure.
 * @param value The input value to be modulated.
 * @return value * mod->amount * mod-range * target scalar. Offset according to the mod side. For linear modulation between 0 and range, the target scalar should 1.
 */
inline float __attribute__((always_inline)) mod_modulate(mod_connection *mod, float value) 
{
    switch (mod->side)
    {
    case MOD_CONNECTION_SIDE_NO_MOD:
        return 0;
        break;
    case MOD_CONNECTION_SIDE_CENTER:
        return value * mod->amount * (mod->range * ( *(mod->target_scalar) - 0.5f));
        break;
    case MOD_CONNECTION_SIDE_RIGHT:
        return value * mod->amount * (mod->range * ( *(mod->target_scalar)));
        break;
    case MOD_CONNECTION_SIDE_LEFT:
        return value * mod->amount * (mod->range * ( 1-*(mod->target_scalar)));
        break;
    }
    return value;
}

/**
 * This function writes the modulated value into the target scalar
 */
void mod_write_to_mod_target(mod_connection *mod, float value);


void mod_init_mod_connection(mod_connection *mod, float *target_scalar, float range, mod_connection_side side);



#endif