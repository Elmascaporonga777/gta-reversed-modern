/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "eOnscreenCounter.h"

class COnscreenTimerEntry {
public:
    uint32_t        m_nVarId;
    char            m_szDescriptionTextKey[10];
    char            m_szDisplayedText[42]; // possibly 2b padding?
    bool            m_bEnabled;
    eTimerDirection m_nTimerDirection;
    uint32_t        m_nClockBeepCountdownSecs;

public:
    void Process();
    void ProcessForDisplayClock();
};

VALIDATE_SIZE(COnscreenTimerEntry, 0x40);
