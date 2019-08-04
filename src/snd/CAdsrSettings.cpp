//
// Created by ddl_blue on 24.6.19.
//

#include "../../include/snd/CAdsrSettings.hpp"
#include "../../include/snd/NSndConfig.hpp"

using namespace NSnd;

/*----------------------------------------------------------------------*/
CAdsrSettings::CAdsrSettings(double attack, double decay, double sustain, double release) {
    SetAttack(attack);
    SetDecay(decay);
    SetSustain(sustain);
    SetRelease(release);
}


/*----------------------------------------------------------------------*/
void CAdsrSettings::SetAttack(double attack) {
    m_attack = (int32_t) (attack * CONVERSION_MULTIPLE);
}

/*----------------------------------------------------------------------*/
void CAdsrSettings::SetSustain(double sustain) {
    m_sustain = (int32_t) (sustain * CONVERSION_MULTIPLE);
}

/*----------------------------------------------------------------------*/
void CAdsrSettings::SetDecay(double decay) {
    m_decay = (int32_t) (decay * CONVERSION_MULTIPLE);
}

/*----------------------------------------------------------------------*/
void CAdsrSettings::SetRelease(double release) {
    m_release = (int32_t) (release * CONVERSION_MULTIPLE);
}

/*----------------------------------------------------------------------*/
double CAdsrSettings::GetAttack() {
    return ((double) m_attack) / CONVERSION_MULTIPLE;
}

/*----------------------------------------------------------------------*/
double CAdsrSettings::GetSustain() {
    return ((double) m_sustain) / CONVERSION_MULTIPLE;
}

/*----------------------------------------------------------------------*/
double CAdsrSettings::GetDecay() {
    return ((double) m_decay) / CONVERSION_MULTIPLE;
}

/*----------------------------------------------------------------------*/
double CAdsrSettings::GetRelease() {
    return ((double) m_release) / CONVERSION_MULTIPLE;
}

/*----------------------------------------------------------------------*/
uint32_t CAdsrSettings::GetAttackSampleLen() {
    return (uint32_t) (GetAttack() * SAMPLE_RATE);
}

/*----------------------------------------------------------------------*/
uint32_t CAdsrSettings::GetDecaySampleLen() {
    return (uint32_t) (GetDecay() * SAMPLE_RATE);
}

/*----------------------------------------------------------------------*/
uint32_t CAdsrSettings::GetReleaseSampleLen() {
    return (uint32_t) (GetRelease() * SAMPLE_RATE);
}