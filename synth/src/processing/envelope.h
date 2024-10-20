#ifndef ENV_H
#define ENV_H

#include <stdint.h>
#include "../common.h"

float attack_envelope_lin (float t, float oneByDuration);
float decay_envelope_lin  (float t, float oneByDuration, float sustainCeiling);
float sustain_envelope_lin(float t, float oneByDuration, float sustainCeiling);
float release_envelope_lin(float t, float oneByDuration, float sustainCeiling);

enum ENV_STATE {
    ENV_ATTACK,
    ENV_DECAY,
    ENV_SUSTAIN,
    ENV_RELEASE,
    ENV_IDLE
};typedef enum ENV_STATE env_state;



struct  ENVELOPE
{
    float (*attack) (float t, float oneByDuration);
    float (*decay)  (float t, float oneByDuration, float sustainCeiling);
    float (*release)(float t, float oneByDuration, float sustainCeiling);

    float one_by_attack_duration;
    float one_by_decay_duration;
    float one_by_release_duration;

    float attack_duration;
    float decay_duration;
    float release_duration;

    float time_since_last_state_change;
    env_state state;

    float sustainCeiling;
    float current_scalar;

};
typedef struct  ENVELOPE envelope;


void env_init_env(envelope *env);
void env_update_envelope(envelope *env, key_event event);
void env_init_env_adsr(envelope *env, float a, float d, float s, float r);
void env_set_attack(envelope *env, float duration);
void env_set_decay(envelope *env, float duration);
void env_set_sustain(envelope *env, float sustainLevel);
void env_set_release(envelope *env, float duration);



#endif