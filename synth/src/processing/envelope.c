#include "envelope.h"


envelope envelopes[NR_ENVELOPES];


void env_init_env(envelope *env)
{
    env->state = ENV_IDLE;
    env->attack = attack_envelope_lin;
    env->decay = decay_envelope_lin;
    env->release = release_envelope_lin;
    env_set_attack(env, 0.0005f);
    env_set_decay(env, 0.2f);
    env_set_sustain(env, 1.f);
    env_set_release(env, 0.1f);

    env->time_since_last_state_change = 0;
}

void env_init_env_adsr(envelope *env, float a, float d, float s, float r)
{
    env->state = ENV_IDLE;
    env->attack = attack_envelope_lin;
    env->decay = decay_envelope_lin;
    env->release = release_envelope_lin;
    env_set_attack(env, a);
    env_set_decay(env, d);
    env_set_sustain(env, s);
    env_set_release(env, r);

    env->time_since_last_state_change = 0;
}

void env_set_attack(envelope *env, float duration)
{
    env->attack_duration = duration;
    if(duration <= 0.0001f)
    {
        env->one_by_attack_duration = 0.0001;
    }
    else
    {
        env->one_by_attack_duration = 1.f / env->attack_duration;
    }
}
void env_set_decay(envelope *env, float duration)
{
    env->decay_duration = duration;
    if(duration <= 0.0001f)
    {
        env->one_by_decay_duration = 0.0001;
    }
    else
    {
        env->one_by_decay_duration = 1.f / env->decay_duration;
    }
}

void env_set_sustain(envelope *env, float sustainLevel)
{
    env->sustainCeiling = sustainLevel;
}

void env_set_release(envelope *env, float duration)
{
    env->release_duration = duration;
    if(duration <= 0.0001f)
    {
        env->one_by_release_duration = 0.0001;
    }
    else
    {
        env->one_by_release_duration = 1.f / env->release_duration;
    }
}

float attack_envelope_lin (float t, float oneByDuration)
{
    return t * oneByDuration;
}
float decay_envelope_lin  (float t, float oneByDuration, float sustainCeiling)
{
    return 1 - (t * oneByDuration) * (1 - sustainCeiling);
}

float release_envelope_lin(float t, float oneByDuration, float sustainCeiling)
{
    return sustainCeiling * (1 - t * oneByDuration);
}

void env_update_envelope(envelope *env, key_event_t event)
{
    //handle inputs first
    if(event == KEY_EVENT_PRESSED)
    {
        env->state = ENV_ATTACK;
        env->time_since_last_state_change = 0;
    }else if(event == KEY_EVENT_RELEASED)
    {
        env->state = ENV_RELEASE;
        env->time_since_last_state_change = 0;
    }
    
    //In here is a bug that one tick is calculated after the time limit is reached ...
    env->time_since_last_state_change += s_PER_TICK;

    switch (env->state)
    {
    case ENV_ATTACK:
        env->current_scalar = env->attack(env->time_since_last_state_change, env->one_by_attack_duration);
        if (env->time_since_last_state_change > env->attack_duration)
        {
            env->state = ENV_DECAY;
            env->time_since_last_state_change = 0;
        }
        break;
    case ENV_DECAY:
        env->current_scalar = env->decay(env->time_since_last_state_change, env->one_by_decay_duration, env->sustainCeiling);
        if (env->time_since_last_state_change > env->decay_duration)
        {
            env->state = ENV_SUSTAIN;
            env->time_since_last_state_change = 0;
        }
        break;
    case ENV_SUSTAIN:
        env->current_scalar = env->sustainCeiling;
        break;
    case ENV_RELEASE:
        env->current_scalar = env->release(env->time_since_last_state_change, env->one_by_release_duration, env->sustainCeiling);
        if (env->time_since_last_state_change > env->release_duration)
        {
            env->state = ENV_IDLE;
            env->time_since_last_state_change = 0;
        }
        break;
    case ENV_IDLE:
        env->current_scalar = 0;
        break;
    }

}

void env_init_envelopes()
{
    env_init_env_adsr(&(envelopes[0]), 0.005, 0.17f, 0.0, 0);
    env_init_env_adsr(&(envelopes[1]), 0.005, 0.3f, 0.0f, 0);
    env_init_env_adsr(&(envelopes[2]), 0.005, 0.2f, 0.03, 0.5);
}