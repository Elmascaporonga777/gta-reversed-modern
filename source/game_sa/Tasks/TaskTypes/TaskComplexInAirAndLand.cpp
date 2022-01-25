#include "StdInc.h"

#include "TaskComplexInAirAndLand.h"
#include "TaskSimpleGetUp.h"
#include "TaskSimpleFall.h"
#include "TaskSimpleLand.h"
#include "TaskSimpleClimb.h"

void CTaskComplexInAirAndLand::InjectHooks() {
    Install("CTaskComplexInAirAndLand", "CTaskComplexInAirAndLand", 0x678C80, &CTaskComplexInAirAndLand::Constructor);
    Install("CTaskComplexInAirAndLand", "CreateFirstSubTask", 0x67CC30, &CTaskComplexInAirAndLand::CreateFirstSubTask_Reversed);
    Install("CTaskComplexInAirAndLand", "CreateNextSubTask", 0x67CCB0, &CTaskComplexInAirAndLand::CreateNextSubTask_Reversed);
    Install("CTaskComplexInAirAndLand", "ControlSubTask", 0x67D230, &CTaskComplexInAirAndLand::ControlSubTask_Reversed);
}

CTaskComplexInAirAndLand* CTaskComplexInAirAndLand::Constructor(bool bUsingJumpGlide, bool bUsingFallGlide) {
    this->CTaskComplexInAirAndLand::CTaskComplexInAirAndLand(bUsingJumpGlide, bUsingFallGlide);
    return this;
}

// 0x678C80
CTaskComplexInAirAndLand::CTaskComplexInAirAndLand(bool bUsingJumpGlide, bool bUsingFallGlide) : CTaskComplex() {
    m_bUsingJumpGlide = bUsingJumpGlide;
    m_bUsingFallGlide = bUsingFallGlide;
    m_bInvalidClimb   = false;
}

// 0x67CC30
CTask* CTaskComplexInAirAndLand::CreateFirstSubTask(CPed* ped) {
    return CreateFirstSubTask_Reversed(ped);
}

// 0x67CCB0
CTask* CTaskComplexInAirAndLand::CreateNextSubTask(CPed* ped) {
    return CreateNextSubTask_Reversed(ped);
}

// 0x67D230
CTask* CTaskComplexInAirAndLand::ControlSubTask(CPed* ped) {
    return ControlSubTask_Reversed(ped);
}

CTask* CTaskComplexInAirAndLand::CreateFirstSubTask_Reversed(CPed* ped) {
    return new CTaskSimpleInAir(m_bUsingJumpGlide, m_bUsingFallGlide, false);
}

CTask* CTaskComplexInAirAndLand::CreateNextSubTask_Reversed(CPed* ped) {
    switch (m_pSubTask->GetTaskType()) {
    case TASK_SIMPLE_GET_UP:
    case TASK_SIMPLE_LAND:
    case TASK_SIMPLE_CLIMB:
        return nullptr;
    case TASK_SIMPLE_FALL:
        return new CTaskSimpleGetUp();
    case TASK_SIMPLE_IN_AIR: {
        auto subTask = reinterpret_cast<CTaskSimpleInAir*>(m_pSubTask);

        if (m_bUsingFallGlide) {
            if (subTask->m_pAnim) {
                subTask->m_pAnim->m_nFlags |= ANIM_FLAG_FREEZE_LAST_FRAME;
                subTask->m_pAnim->m_fBlendDelta = -8.0F;
                subTask->m_pAnim->SetFinishCallback(CDefaultAnimCallback::DefaultAnimCB, nullptr);
                subTask->m_pAnim = nullptr;
            }

            return new CTaskSimpleLand(ped->m_vecMoveSpeed.z < -0.1F ? ANIM_ID_KO_SKID_BACK : (AnimationId)-1);
        } else if (subTask->m_pAnim && subTask->m_pAnim->m_nAnimId == ANIM_ID_FALL_FALL) {
            CTask* pNewTask;

            if (subTask->m_fMinZSpeed < -0.4F)
                pNewTask = new CTaskSimpleFall(ANIM_ID_KO_SKID_BACK, ANIM_GROUP_DEFAULT, 700);
            else
                pNewTask = new CTaskSimpleLand(ANIM_ID_FALL_COLLAPSE);

            ped->m_pedAudio.AddAudioEvent(59, 0.0F, 1.0F, 0, 0, 0, 0);

            if (ped->m_pPlayerData) {
                CVector          empty{};
                CEventSoundQuiet eventSound(ped, ped->m_pPlayerData->m_pPedClothesDesc->GetIsWearingBalaclava() ? 75.0F : 60.0F, -1, empty);
                GetEventGlobalGroup()->Add(&eventSound, false);
            }

            return pNewTask;
        } else {
            AnimationId landAnimId;

            if (!ped->IsPlayer())
                landAnimId = ANIM_ID_FALL_LAND;
            else if (subTask->m_pAnim && subTask->m_pAnim->m_nAnimId == ANIM_ID_IDLE_HBHB_1)
                landAnimId = ANIM_ID_IDLE_TIRED;
            else {
                auto pad = ped->AsPlayerPed()->GetPadFromPlayer();
                if (ped->m_pPlayerData->m_fMoveBlendRatio > 1.5F && pad && (pad->GetPedWalkUpDown() != 0.0F || pad->GetPedWalkLeftRight() != 0.0F))
                    landAnimId = ANIM_ID_JUMP_LAND;
                else
                    landAnimId = ANIM_ID_FALL_LAND;
            }

            auto pNewTask = new CTaskSimpleLand(landAnimId);

            ped->m_pedAudio.AddAudioEvent(58, 0.0F, 1.0F, 0, 0, 0, 0);

            if (ped->m_pPlayerData) {
                CVector          empty{};
                CEventSoundQuiet eventSound(ped, ped->m_pPlayerData->m_pPedClothesDesc->GetIsWearingBalaclava() ? 70.0F : 55.0F, -1, empty);
                GetEventGlobalGroup()->Add(&eventSound, false);
            }

            return pNewTask;
        }
    }
    default:
        return nullptr;
    }
}

CTask* CTaskComplexInAirAndLand::ControlSubTask_Reversed(CPed* ped) {
    if (!m_bUsingFallGlide && m_pSubTask && m_pSubTask->GetTaskType() == TASK_SIMPLE_IN_AIR) {
        auto subTask = reinterpret_cast<CTaskSimpleInAir*>(m_pSubTask);

        if (!m_bInvalidClimb && subTask->m_pClimbEntity)
            return new CTaskSimpleClimb(
                subTask->m_pClimbEntity,
                subTask->m_vecPosn,
                subTask->m_fAngle,
                subTask->m_nSurfaceType,
                subTask->m_vecPosn.z - ped->GetPosition().z < CTaskSimpleClimb::ms_fMinForStretchGrab - 0.3F ? CLIMB_PULLUP : CLIMB_GRAB,
                false
            );
    }

    return m_pSubTask;
}
