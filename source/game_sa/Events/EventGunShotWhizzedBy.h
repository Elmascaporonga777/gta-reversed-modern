#pragma once

#include "EventGunShot.h"

class NOTSA_EXPORT_VTABLE CEventGunShotWhizzedBy : public CEventGunShot {
public:
    static void InjectHooks();

    CEventGunShotWhizzedBy(CEntity* entity, const CVector& startPoint, const CVector& endPoint, bool bHasNoSound);
    ~CEventGunShotWhizzedBy() override = default;

    eEventType GetEventType() const override { return EVENT_SHOT_FIRED_WHIZZED_BY; }
    int32 GetEventPriority() const override { return 36; }
    int32 GetLifeTime() override { return 0; }
    bool AffectsPed(CPed* ped) override;
    bool CanBeInterruptedBySameEvent() override { return true; }
    CEventEditableResponse* CloneEditable() override { return new CEventGunShotWhizzedBy(m_entity, m_startPoint, m_endPoint, m_bHasNoSound); }

private:
    CEventGunShotWhizzedBy* Constructor(CEntity* entity, const CVector& startPoint, const CVector& endPoint, bool bHasNoSound);
    bool AffectsPed_Reversed(CPed* ped);
};
