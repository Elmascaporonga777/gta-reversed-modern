/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#include "StdInc.h"
#include <optional>
#include <WeaponInfo.h>
#include "Ped.h"

#include "PedType.h"
#include "Buoyancy.h"
#include "TaskSimpleSwim.h"
#include "PedStats.h"
#include "Conversations.h"
#include "TaskSimpleLand.h"
#include "AEAudioUtility.h"
#include "PedClothesDesc.h"
#include "TaskSimpleHoldEntity.h"
#include "TaskComplexGoPickUpEntity.h"
#include "Radar.h"
#include "PostEffects.h"
#include "PedStdBonePositions.h"
#include "TaskSimpleJetPack.h"
#include "PedSaveStructure.h"

void CPed::InjectHooks() {
    RH_ScopedClass(CPed);
    RH_ScopedCategory("Entity/Ped");

    // RH_ScopedInstall(Constructor, 0x5E8030);
    // Install("CPed", "~CPed", 0x5E8620, static_cast<CPed*(CPed::*)()>(&CPed::Destructor));

    RH_ScopedInstall(OurPedCanSeeThisEntity, 0x5E1660);
    RH_ScopedInstall(operator delete, 0x5E4760);
    RH_ScopedInstall(operator new, 0x5E4720);
    RH_ScopedInstall(SpawnFlyingComponent, 0x5F0190);
    RH_ScopedInstall(PedCanPickUpPickUp, 0x455560);
    RH_ScopedInstall(Update, 0x5DEBE0);
    RH_ScopedInstall(Initialise, 0x5DEBB0);
    RH_ScopedInstall(UpdateStatLeavingVehicle, 0x5E01B0);
    RH_ScopedInstall(UpdateStatEnteringVehicle, 0x5E01A0);
    RH_ScopedInstall(ShoulderBoneRotation, 0x5DF560);
    RH_ScopedInstall(RestoreHeadingRateCB, 0x5DFD70);
    RH_ScopedInstall(PedIsInvolvedInConversation, 0x43AB90);
    RH_ScopedInstall(ClearWeapons, 0x5E6320);
    RH_ScopedInstall(ClearWeapon, 0x5E62B0);
    RH_ScopedOverloadedInstall(SetCurrentWeapon, "WepType", 0x5E6280, void(CPed::*)(eWeaponType));
    RH_ScopedOverloadedInstall(SetCurrentWeapon, "Slot", 0x5E61F0, void(CPed::*)(int32));
    RH_ScopedInstall(GiveWeapon, 0x5E6080);
    RH_ScopedInstall(TakeOffGoggles, 0x5E6010);
    RH_ScopedInstall(AddWeaponModel, 0x5E5ED0);
    RH_ScopedInstall(PlayFootSteps, 0x5E57F0);
    // RH_ScopedInstall(DoFootLanded, 0x5E5380);
    RH_ScopedInstall(ClearAll, 0x5E5320);
    RH_ScopedInstall(CalculateNewOrientation, 0x5E52E0);
    // RH_ScopedInstall(CalculateNewVelocity, 0x5E4C50);
    RH_ScopedInstall(SetCharCreatedBy, 0x5E47E0);
    RH_ScopedInstall(SetPedState, 0x5E4500);
    RH_ScopedInstall(GiveObjectToPedToHold, 0x5E4390);
    RH_ScopedInstall(ClearLookFlag, 0x5E1950);
    RH_ScopedInstall(WorkOutHeadingForMovingFirstPerson, 0x5E1A00);
    // RH_ScopedInstall(UpdatePosition, 0x5E1B10);
    // RH_ScopedInstall(MakeTyresMuddySectorList, 0x6AE0D0);
    RH_ScopedInstall(IsPedInControl, 0x5E3960);
    RH_ScopedInstall(RemoveWeaponModel, 0x5E3990);
    // RH_ScopedInstall(RemoveWeaponWhenEnteringVehicle, 0x5E6370);
    RH_ScopedInstall(AddGogglesModel, 0x5E3A90);
    RH_ScopedInstall(SetWeaponSkill, 0x5E3C10);
    RH_ScopedInstall(ClearLook, 0x5E3FF0);
    RH_ScopedInstall(TurnBody, 0x5E4000);
    RH_ScopedInstall(IsPointerValid, 0x5E4220);
    RH_ScopedInstall(GetBonePosition, 0x5E4280);
    RH_ScopedInstall(PutOnGoggles, 0x5E3AE0);
    // RH_ScopedInstall(SortPeds, 0x5E17E0);
    RH_ScopedInstall(ReplaceWeaponWhenExitingVehicle, 0x5E6490);
    // RH_ScopedInstall(KillPedWithCar, 0x5F0360);
    RH_ScopedInstall(IsPedHeadAbovePos, 0x5F02C0);
    RH_ScopedInstall(RemoveWeaponAnims, 0x5F0250);
    RH_ScopedInstall(DoesLOSBulletHitPed, 0x5F01A0);
    RH_ScopedInstall(RemoveBodyPart, 0x5F0140);
    RH_ScopedInstall(Say, 0x5EFFE0);
    RH_ScopedInstall(SayScript, 0x5EFFB0);
    RH_ScopedInstall(CanPedHoldConversation, 0x5EFFA0);
    RH_ScopedInstall(EnablePedSpeechForScriptSpeech, 0x5EFF90);
    RH_ScopedInstall(DisablePedSpeechForScriptSpeech, 0x5EFF80);
    RH_ScopedInstall(EnablePedSpeech, 0x5EFF70);
    RH_ScopedInstall(DisablePedSpeech, 0x5EFF60);
    RH_ScopedInstall(GetPedTalking, 0x5EFF50);
    RH_ScopedInstall(GiveWeaponWhenJoiningGang, 0x5E8BE0);
    RH_ScopedInstall(GiveDelayedWeapon, 0x5E89B0);
    RH_ScopedOverloadedInstall(GetWeaponSkill, "", 0x5E6580, eWeaponSkill(CPed::*)());
    // RH_ScopedInstall(PreRenderAfterTest, 0x5E65A0);
    RH_ScopedInstall(SetIdle, 0x5E7980);
    RH_ScopedOverloadedInstall(SetLook, "Heading", 0x5E79B0, void(CPed::*)(float));
    RH_ScopedOverloadedInstall(SetLook, "Entity", 0x5E7A60, void(CPed::*)(CEntity *));
    RH_ScopedInstall(Look, 0x5E7B20);
    RH_ScopedInstall(ReplaceWeaponForScriptedCutscene, 0x5E6530);
    RH_ScopedInstall(RemoveWeaponForScriptedCutscene, 0x5E6550);
    RH_ScopedInstall(GiveWeaponAtStartOfFight, 0x5E8AB0);
    RH_ScopedInstall(ProcessBuoyancy, 0x5E1FA0);
    // RH_ScopedInstall(PositionPedOutOfCollision, 0x5E0820);
    RH_ScopedInstall(GrantAmmo, 0x5DF220);
    RH_ScopedInstall(GetWeaponSlot, 0x5DF200);
    RH_ScopedInstall(PositionAnyPedOutOfCollision, 0x5E13C0);
    RH_ScopedInstall(CanBeDeletedEvenInVehicle, 0x5DF150);
    RH_ScopedInstall(CanBeDeleted, 0x5DF100);
    RH_ScopedInstall(CanStrafeOrMouseControl, 0x5DF090);
    RH_ScopedInstall(CanBeArrested, 0x5DF060);
    RH_ScopedInstall(CanSetPedState, 0x5DF030);
    RH_ScopedInstall(CanPedReturnToState, 0x5DF000);
    RH_ScopedInstall(UseGroundColModel, 0x5DEFE0);
    RH_ScopedInstall(IsPedShootable, 0x5DEFD0);
    RH_ScopedInstall(GetLocalDirection, 0x5DEF60);
    RH_ScopedInstall(ClearAimFlag, 0x5DEF20);
    RH_ScopedOverloadedInstall(SetAimFlag, "Entity", 0x5DEED0, void(CPed::*)(CEntity *));
    RH_ScopedOverloadedInstall(SetLookFlag, "Entity", 0x5DEE40, void(CPed::*)(CEntity *, bool, bool));
    RH_ScopedOverloadedInstall(SetLookFlag, "Heading", 0x5DEDC0, void(CPed::*)(float, bool, bool));
    RH_ScopedInstall(CanUseTorsoWhenLooking, 0x5DED90);
    RH_ScopedInstall(PedIsReadyForConversation, 0x43ABA0);
    RH_ScopedInstall(CreateDeadPedMoney, 0x4590F0);
    RH_ScopedOverloadedInstall(CreateDeadPedPickupCoors, "", 0x459180, void(CPed::*)(float&, float&, float&));
    RH_ScopedInstall(CreateDeadPedWeaponPickups, 0x4591D0);
    RH_ScopedInstall(IsWearingGoggles, 0x479D10);
    RH_ScopedInstall(SetAmmo, 0x5DF290);
    RH_ScopedInstall(SetStayInSamePlace, 0x481090);
    RH_ScopedInstall(SetPedStats, 0x5DEBC0);
    RH_ScopedInstall(SetMoveState, 0x5DEC00);
    RH_ScopedInstall(SetMoveAnimSpeed, 0x5DEC10);
    RH_ScopedInstall(StopNonPartialAnims, 0x5DED10);
    RH_ScopedInstall(RestartNonPartialAnims, 0x5DED50);
    RH_ScopedInstall(DoWeHaveWeaponAvailable, 0x5DF300);
    RH_ScopedInstall(RemoveGogglesModel, 0x5DF170);
    RH_ScopedInstall(SetGunFlashAlpha, 0x5DF400);
    RH_ScopedInstall(CanSeeEntity, 0x5E0730);
    RH_ScopedInstall(SetPedDefaultDecisionMaker, 0x5E06E0);
    RH_ScopedInstall(GetWalkAnimSpeed, 0x5E04B0);
    RH_ScopedInstall(StopPlayingHandSignal, 0x5E0480);
    RH_ScopedInstall(IsPlayingHandSignal, 0x5E0460);
    RH_ScopedInstall(CanThrowEntityThatThisPedIsHolding, 0x5E0400);
    RH_ScopedInstall(DropEntityThatThisPedIsHolding, 0x5E0360);
    RH_ScopedInstall(GetEntityThatThisPedIsHolding, 0x5E02E0);
    RH_ScopedInstall(GetHoldingTask, 0x5E0290);
    RH_ScopedInstall(ReleaseCoverPoint, 0x5E0270);
    RH_ScopedInstall(DoGunFlash, 0x5DF340);
    RH_ScopedInstall(GetTransformedBonePosition, 0x5E01C0);
    RH_ScopedInstall(IsAlive, 0x5E0170);
    RH_ScopedInstall(DeadPedMakesTyresBloody, 0x6B4200);
    RH_ScopedInstall(Undress, 0x5E00F0);
    RH_ScopedInstall(SetLookTimer, 0x5DF8D0);
    RH_ScopedInstall(RestoreHeadingRate, 0x5DFD60);
    RH_ScopedInstall(Dress, 0x5E0130);
    RH_ScopedInstall(IsPlayer, 0x5DF8F0);
    RH_ScopedInstall(GetBikeRidingSkill, 0x5DF510);
    //RH_ScopedInstall(SetPedPositionInCar, 0x5DF910);
    RH_ScopedInstall(SetRadioStation, 0x5DFD90);
    // RH_ScopedInstall(PositionAttachedPed, 0x5DFDF0);
    RH_ScopedInstall(ResetGunFlashAlpha, 0x5DF4E0);
    RH_ScopedInstall(SetModelIndex_Reversed, 0x5E4880);
    RH_ScopedInstall(DeleteRwObject_Reversed, 0x5DEBF0);
    // RH_ScopedInstall(ProcessControl_Reversed, 0x5E8CD0);
    RH_ScopedInstall(Teleport_Reversed, 0x5E4110);
    // RH_ScopedInstall(SpecialEntityPreCollisionStuff_Reversed, 0x5E3C30);
    // RH_ScopedInstall(SpecialEntityCalcCollisionSteps_Reversed, 0x5E3E90);
    RH_ScopedInstall(PreRender_Reversed, 0x5E8A20);
    RH_ScopedInstall(Render_Reversed, 0x5E7680);
    RH_ScopedInstall(SetupLighting_Reversed, 0x553F00);
    RH_ScopedInstall(RemoveLighting_Reversed, 0x5533B0);
    RH_ScopedInstall(FlagToDestroyWhenNextProcessed_Reversed, 0x5E7B70);
    // RH_ScopedInstall(ProcessEntityCollision_Reversed, 0x5E2530);
    RH_ScopedInstall(SetMoveAnim_Reversed, 0x5E4A00);
    RH_ScopedInstall(Save_Reversed, 0x5D5730);
    RH_ScopedInstall(Load_Reversed, 0x5D4640);

    RH_ScopedGlobalInstall(SetPedAtomicVisibilityCB, 0x5F0060);
}

CPed::CPed(ePedType pedtype) : CPhysical(), m_aWeapons{ plugin::dummy, plugin::dummy, plugin::dummy,
plugin::dummy, plugin::dummy, plugin::dummy, plugin::dummy, plugin::dummy, plugin::dummy, plugin::dummy,
plugin::dummy, plugin::dummy, plugin::dummy }
{
    ((void(__thiscall *)(CPed*, ePedType))0x5E8030)(this, pedtype);
}

/*!
* @addr 0x5E4720
*/
void* CPed::operator new(uint32 size) {
    return CPools::GetPedPool()->New();
}

/*!
* @addr 0x5E4760
*/
void CPed::operator delete(void* data) {
    CPools::GetPedPool()->Delete((CPed*)data);
}

/*!
* @addr 0x5E4A00
*/
void CPed::SetMoveAnim() {
    if (!IsAlive() || bIsDucking || m_pAttachedTo) {
        return;
    }

    const auto DoUpdateMoveAnim = [this](auto* assoc) {
        if (!bMoveAnimSpeedHasBeenSetByTask) {
            SetMoveAnimSpeed(assoc);
        }
    };

    if (m_nSwimmingMoveState == m_nMoveState) {
        switch (m_nMoveState) {
        case eMoveState::PEDMOVE_WALK:
        case eMoveState::PEDMOVE_JOG:
        case eMoveState::PEDMOVE_RUN:
        case eMoveState::PEDMOVE_SPRINT: {
            const auto GetAnimId = [this] {
                switch (m_nMoveState) {
                case eMoveState::PEDMOVE_RUN:
                    return AnimationId::ANIM_ID_RUN;
                case eMoveState::PEDMOVE_SPRINT:
                    return AnimationId::ANIM_ID_SPRINT;
                }
                return AnimationId::ANIM_ID_WALK;
            };

            if (const auto assoc = RpAnimBlendClumpGetAssociation(m_pRwClump, GetAnimId())) {
                DoUpdateMoveAnim(assoc);
            }

            break;
        }
        }
    } else if (m_nMoveState != eMoveState::PEDMOVE_NONE) {
        m_nSwimmingMoveState = m_nMoveState;

        
        switch (m_nMoveState) { // TODO: What's happening here?
        case eMoveState::PEDMOVE_WALK:
        case eMoveState::PEDMOVE_RUN:
        case eMoveState::PEDMOVE_SPRINT: {
            for (auto assoc = RpAnimBlendClumpGetFirstAssociation(m_pRwClump, ANIM_FLAG_PARTIAL); assoc; assoc = RpAnimBlendGetNextAssociation(assoc, ANIM_FLAG_PARTIAL)) {
                if ((assoc->m_nFlags & ANIM_FLAG_UNLOCK_LAST_FRAME) == 0 && (assoc->m_nFlags & ANIM_FLAG_ADD_TO_BLEND) == 0) {
                    assoc->m_fBlendDelta = -2.f;
                    assoc->SetFlag(ANIM_FLAG_FREEZE_LAST_FRAME, true);
                }
            }

            ClearAimFlag();
            ClearLookFlag();

            break;
        }
        }

        // Do BlendAnimation and call `DoUpdateMoveAnim` aftewards
        const auto DoBlendAnim = [&, this](AssocGroupId grp, AnimationId animId, float blendDelta) {
            if (const auto assoc = CAnimManager::BlendAnimation(m_pRwClump, grp, animId, blendDelta)) {
                DoUpdateMoveAnim(assoc);
            }
        };
        
        switch (m_nMoveState) {
        case eMoveState::PEDMOVE_STILL:
            DoBlendAnim(m_nAnimGroup, AnimationId::ANIM_ID_IDLE, 4.f);
            return;

        case eMoveState::PEDMOVE_TURN_L:
            DoBlendAnim(ANIM_GROUP_DEFAULT, AnimationId::ANIM_ID_TURN_L, 16.f);
            return;

        case eMoveState::PEDMOVE_TURN_R:
            DoBlendAnim(ANIM_GROUP_DEFAULT, AnimationId::ANIM_ID_TURN_R, 16.f);
            return;

        case eMoveState::PEDMOVE_WALK:
            DoBlendAnim(m_nAnimGroup, AnimationId::ANIM_ID_WALK, 1.f);
            return;

        case eMoveState::PEDMOVE_RUN:
            DoBlendAnim(m_nAnimGroup, AnimationId::ANIM_ID_RUN, m_nPedState == ePedState::PEDSTATE_FLEE_ENTITY ? 3.f : 1.f);
            return;

        case eMoveState::PEDMOVE_SPRINT: {
            // If we're in a group, and our leader is sprinting as well sprinting should be played with a different anim group
            if (CPedGroups::IsInPlayersGroup(this)) {
                if (const auto leader = CPedGroups::GetPedsGroup(this)->GetMembership().GetLeader()) {
                    switch (leader->m_nMoveState) {
                    case eMoveState::PEDMOVE_RUN:
                    case eMoveState::PEDMOVE_SPRINT: {
                        DoBlendAnim(ANIM_GROUP_PLAYER, ANIM_ID_SPRINT, 1.f);
                        return;
                    }
                    }
                }
            }
            DoBlendAnim(m_nAnimGroup, AnimationId::ANIM_ID_SPRINT, 1.f);
            return;
        }
        }
    } else {
        m_nSwimmingMoveState = eMoveState::PEDMOVE_NONE;
    }
}

/*!
* @addr 0x5D5730
*/
bool CPed::Save() {
    CPedSaveStructure save;
    save.Construct(this);

    uint32 size{ sizeof(save) };
    CGenericGameStorage::SaveDataToWorkBuffer(&size, sizeof(size));
    CGenericGameStorage::SaveDataToWorkBuffer(&save, sizeof(save));

    return true;
}

/*!
* @addr 0x5D4640
*/
bool CPed::Load() {
    CPedSaveStructure save;
    uint32 size{};
    CGenericGameStorage::LoadDataFromWorkBuffer(&size, sizeof(size));
    CGenericGameStorage::LoadDataFromWorkBuffer(&save, sizeof(save));

    assert(size == sizeof(save)); // NOTSA
    save.Extract(this);

    return true;
}

/*!
* @addr 0x43AB90
*/
bool CPed::PedIsInvolvedInConversation() {
    return this == CPedToPlayerConversations::m_pPed;
}

/*!
* @addr 0x43ABA0
*/
bool CPed::PedIsReadyForConversation(bool checkLocalPlayerWantedLevel) {
    // We don't talk when we're behind the wheel! (Nor when we're fighting...)
    if (bInVehicle || GetIntelligence()->GetTaskFighting()) {
        return false;
    }
    
    if (checkLocalPlayerWantedLevel && FindPlayerPed()->GetWanted()->m_nWantedLevel) {
        return false;
    }

    // If we're doing any of these we don't have the mental power to chat...
    switch (m_nMoveState) {
    case eMoveState::PEDMOVE_JOG:
    case eMoveState::PEDMOVE_RUN:
    case eMoveState::PEDMOVE_SPRINT:
        return false;
    }

    if (!IsCreatedByMission()) { // Don't check if we've a chatting task/event if we're a mission ped
        if (GetIntelligence()->FindTaskByType(eTaskType::TASK_COMPLEX_PARTNER_CHAT)) {
            return false;
        }

        if (GetIntelligence()->m_eventGroup.GetEventOfType(eEventType::EVENT_CHAT_PARTNER)) {
            return false;
        }
    }

    return true;
}

/*!
* @addr 0x455560
*/
bool CPed::PedCanPickUpPickUp() {
    return FindPlayerPed(0)->GetTaskManager().FindActiveTaskFromList({ TASK_COMPLEX_ENTER_CAR_AS_DRIVER, TASK_COMPLEX_USE_MOBILE_PHONE });
}

/*!
* @addr 0x4590F0
*/
void CPed::CreateDeadPedMoney() {
    if (!CLocalisation::StealFromDeadPed()) {
        return;
    }

    switch (m_nPedType) {
    case ePedType::PED_TYPE_COP:
    case ePedType::PED_TYPE_MEDIC:
    case ePedType::PED_TYPE_FIREMAN:
        return;
    }

    if (IsCreatedByMission() && !bMoneyHasBeenGivenByScript) {
        return;
    }

    if (bInVehicle) {
        return;
    }

    if (m_nMoneyCount > 10) {
        CPickups::CreateSomeMoney(GetPosition(), m_nMoneyCount);
        m_nMoneyCount = 0;
    }
}

/*!
* @addr 0x459180
* @brief Created a pickup close to the ped's position (Using \r CPickups::CreatePickupCoorsCloseToCoors)
* @param [out] outPickupX, outPickupY, outPickupZ Position of the created pickup.
*/
void CPed::CreateDeadPedPickupCoors(float& outPickupX, float& outPickupY, float& outPickupZ) {
    CPickups::CreatePickupCoorsCloseToCoors(GetPosition(), outPickupX, outPickupY, outPickupZ);
}

/*!
* @address notsa
* @copybrief CPed::CreateDeadPedPickupCoors
* @param [out] pickupPos Position of the created pickup.
*/
void CPed::CreateDeadPedPickupCoors(CVector& pickupPos) {
    return CreateDeadPedPickupCoors(pickupPos.x, pickupPos.y, pickupPos.z);
}


/*!
* @addr 0x4591D0
* @brief Create weapon/ammo pickups for dead ped
*/
void CPed::CreateDeadPedWeaponPickups() {
    if (bInVehicle || bDoesntDropWeaponsWhenDead) {
        return;
    }

    for (auto& wep : m_aWeapons) {
        switch (wep.m_nType) {
        case eWeaponType::WEAPON_UNARMED:
        case eWeaponType::WEAPON_DETONATOR:
            continue;
        }
        
        if (!wep.m_nTotalAmmo && !wep.IsTypeMelee()) {
            continue;
        }

        // Now, create a pickup at close to our position
        CVector pickupPos{};
        CreateDeadPedPickupCoors(pickupPos);
        pickupPos.z += 0.3f;

        // No. of ammo the pickups will contain
        const auto pickupAmmo{ std::min(wep.m_nTotalAmmo, (uint32)AmmoForWeapon_OnStreet[(size_t)wep.m_nType] * 2) };

        if (CPickups::TryToMerge_WeaponType(
            pickupPos,
            wep.m_nType,
            ePickupType::PICKUP_ONCE_TIMEOUT,
            pickupAmmo,
            false
        )) {
            CPickups::GenerateNewOne_WeaponType(
                pickupPos,
                wep.m_nType,
                bDeathPickupsPersist ? ePickupType::PICKUP_ONCE_FOR_MISSION : ePickupType::PICKUP_ONCE_TIMEOUT,
                pickupAmmo,
                false,
                nullptr
            );
        }
    }
}

/*!
* @addr 0x5DEBB0
*/
void CPed::Initialise() {
    CPedType::Initialise();
    CCarEnterExit::SetAnimOffsetForEnterOrExitVehicle();
}

/*!
* @addr 0x5DEBC0
* @unused
*/
void CPed::SetPedStats(ePedStats statsType) {
    m_pStats = &CPedStats::ms_apPedStats[(size_t)statsType];
}

/*!
* @addr 0x5DEBE0
*/
void CPed::Update()
{
    // NOP
}

/*!
* @addr 0x5DEC00
*/
void CPed::SetMoveState(eMoveState moveState) {
    m_nMoveState = (int32)moveState;
}

/*!
* @addr 0x5DEC10
*/
void CPed::SetMoveAnimSpeed(CAnimBlendAssociation* association) {
    const auto pitchFactor = std::clamp(m_pedIK.m_fSlopePitch, -0.3f, 0.3f);
    if (IsCreatedByMission()) {
        association->m_fSpeed = pitchFactor + 1.f;
    } else {
        association->m_fSpeed = pitchFactor + 1.2f - m_nRandomSeed * RAND_MAX_FLOAT_RECIPROCAL * 0.4f;
    }
}

/*!
* @addr 0x5DED10
*/
void CPed::StopNonPartialAnims() {
    for (auto assoc = RpAnimBlendClumpGetFirstAssociation(m_pRwClump); assoc; assoc = RpAnimBlendGetNextAssociation(assoc)) {
        if ((assoc->m_nFlags & ANIM_FLAG_PARTIAL) == 0) {
            assoc->SetFlag(ANIM_FLAG_STARTED, false);
        }
    }
}

/*!
* @addr 0x5DED50
*/
void CPed::RestartNonPartialAnims() {
    for (auto assoc = RpAnimBlendClumpGetFirstAssociation(m_pRwClump); assoc; assoc = RpAnimBlendGetNextAssociation(assoc)) {
        if ((assoc->m_nFlags & ANIM_FLAG_PARTIAL) == 0) {
            assoc->SetFlag(ANIM_FLAG_STARTED, true);
        }
    }
}

/*!
* @addr 0x5DED90
*/
bool CPed::CanUseTorsoWhenLooking() {
    switch (m_nPedState) {
    case ePedState::PEDSTATE_DRIVING:
    case ePedState::PEDSTATE_DRAGGED_FROM_CAR:
        return false;
    }

    if (bIsDucking) {
        return false;
    }

    return true;
}

/*!
* @addr 0x5DEDC0
*/
void CPed::SetLookFlag(float lookHeading, bool unused, bool ignoreLookTime) {
    UNUSED(unused);

    if (m_nLookTime >= CTimer::GetTimeInMS() && !ignoreLookTime) {
        return;
    }

    bIsLooking = true;
    m_fLookDirection = lookHeading;
    m_nLookTime = 0;

    ClearReference(m_pLookTarget);

    if (CanUseTorsoWhenLooking()) {
        m_pedIK.bTorsoUsed = false;
    }
}

/*!
* @addr 0x5DEE40
* @brief Start looking at entity \a lookingTo
*/
void CPed::SetLookFlag(CEntity* lookingTo, bool unused, bool ignoreLookTime) {
    UNUSED(unused);

    if (m_nLookTime >= CTimer::GetTimeInMS() && !ignoreLookTime) {
        return;
    }

    bIsRestoringLook = false;
    bIsLooking = true;

    ChangeEntityReference(m_pLookTarget, lookingTo);

    m_fLookDirection = 999'999.f;
    m_nLookTime = 0;

    if (CanUseTorsoWhenLooking()) {
        m_pedIK.bTorsoUsed = false;
    }
}

/*!
* @addr 0x5DEED0
*/
void CPed::SetAimFlag(CEntity* aimingTo) {
    bIsAimingGun = true;
    bIsRestoringGun = false;
    ChangeEntityReference(m_pLookTarget, aimingTo);
    m_nLookTime = 0;
}

/*!
* @addr 0x5DEF20
* @brief Clear gun aiming flag
*/
void CPed::ClearAimFlag() {
    if (bIsAimingGun) {
        bIsAimingGun = false;
        bIsRestoringGun = true;
        m_pedIK.bUseArm = false;
        m_nLookTime = 0;
    }

    if (m_pPlayerData) {
        m_pPlayerData->m_fLookPitch = 0.f;
    }
}

/*!
* @addr 0x5DEF60
* @addr Get angle of point relative to the ped's current rotation
*/
float CPed::GetLocalDirection(const CVector2D& point) {
    auto angle = (float)atan2(-point.x, point.y) - m_fCurrentRotation + PI / 4.f;
    while (angle < 0.f) {
        angle += TWO_PI;
    }
    return angle;
}

/*!
* @addr 0x5DEFD0
*/
bool CPed::IsPedShootable() {
    // Not sure if they used a switch case or `<= PEDSTATE_STATES_CAN_SHOOT` originally, but I'll use a switch case.
    switch (m_nPedState) {
    case ePedState::PEDSTATE_NONE:
    case ePedState::PEDSTATE_IDLE:
    case ePedState::PEDSTATE_LOOK_ENTITY:
    case ePedState::PEDSTATE_LOOK_HEADING:
    case ePedState::PEDSTATE_WANDER_RANGE:
    case ePedState::PEDSTATE_WANDER_PATH:
    case ePedState::PEDSTATE_SEEK_POSITION:
    case ePedState::PEDSTATE_SEEK_ENTITY:
    case ePedState::PEDSTATE_FLEE_POSITION:
    case ePedState::PEDSTATE_FLEE_ENTITY:
    case ePedState::PEDSTATE_PURSUE:
    case ePedState::PEDSTATE_FOLLOW_PATH:
    case ePedState::PEDSTATE_SNIPER_MODE:
    case ePedState::PEDSTATE_ROCKETLAUNCHER_MODE:
    case ePedState::PEDSTATE_DUMMY:
    case ePedState::PEDSTATE_PAUSE:
    case ePedState::PEDSTATE_ATTACK:
    case ePedState::PEDSTATE_FIGHT:
    case ePedState::PEDSTATE_FACE_PHONE:
    case ePedState::PEDSTATE_MAKE_PHONECALL:
    case ePedState::PEDSTATE_CHAT:
    case ePedState::PEDSTATE_MUG:
    case ePedState::PEDSTATE_AIMGUN:
    case ePedState::PEDSTATE_AI_CONTROL:
    case ePedState::PEDSTATE_SEEK_CAR:
    case ePedState::PEDSTATE_SEEK_BOAT_POSITION:
    case ePedState::PEDSTATE_FOLLOW_ROUTE:
    case ePedState::PEDSTATE_CPR:
    case ePedState::PEDSTATE_SOLICIT:
    case ePedState::PEDSTATE_BUY_ICE_CREAM:
    case ePedState::PEDSTATE_INVESTIGATE_EVENT:
    case ePedState::PEDSTATE_EVADE_STEP:
    case ePedState::PEDSTATE_ON_FIRE:
    case ePedState::PEDSTATE_SUNBATHE:
    case ePedState::PEDSTATE_FLASH:
    case ePedState::PEDSTATE_JOG:
    case ePedState::PEDSTATE_ANSWER_MOBILE:
    case ePedState::PEDSTATE_HANG_OUT:
    case ePedState::PEDSTATE_STATES_NO_AI:
    case ePedState::PEDSTATE_ABSEIL_FROM_HELI:
    case ePedState::PEDSTATE_SIT:
    case ePedState::PEDSTATE_JUMP:
    case ePedState::PEDSTATE_FALL:
    case ePedState::PEDSTATE_GETUP:
    case ePedState::PEDSTATE_STAGGER:
    case ePedState::PEDSTATE_EVADE_DIVE:
    case ePedState::PEDSTATE_STATES_CAN_SHOOT:
    return true;
    }
    return false;
}

/*!
* @addr 0x5DEFE0
*/
bool CPed::UseGroundColModel() {
    switch (m_nPedState) {
    case ePedState::PEDSTATE_FALL:
    case ePedState::PEDSTATE_EVADE_DIVE:
    case ePedState::PEDSTATE_DIE:
    case ePedState::PEDSTATE_DEAD:
        return true;
    }
    return false;
}

/*!
* @addr 0x5DF000
*/
bool CPed::CanPedReturnToState()
{
    return
        m_nPedState <= PEDSTATE_STATES_NO_AI &&
        m_nPedState != PEDSTATE_AIMGUN &&
        m_nPedState != PEDSTATE_ATTACK &&
        m_nPedState != PEDSTATE_FIGHT &&
        m_nPedState != PEDSTATE_EVADE_STEP &&
        m_nPedState != PEDSTATE_SNIPER_MODE &&
        m_nPedState != PEDSTATE_LOOK_ENTITY;
}

/*!
* @addr 0x5DF030
*/
bool CPed::CanSetPedState() {
    switch (m_nPedState) {
    case ePedState::PEDSTATE_DIE:
    case ePedState::PEDSTATE_DEAD:
    case ePedState::PEDSTATE_ARRESTED:
    case ePedState::PEDSTATE_ENTER_CAR:
    case ePedState::PEDSTATE_CARJACK:
    case ePedState::PEDSTATE_STEAL_CAR:
        return true;
    }
    return false;
}

/*!
* @addr 0x5DF060
*/
bool CPed::CanBeArrested()
{
    return
        m_nPedState != PEDSTATE_DIE &&
        m_nPedState != PEDSTATE_DEAD &&
        m_nPedState != PEDSTATE_ARRESTED &&
        m_nPedState != PEDSTATE_ENTER_CAR &&
        m_nPedState != PEDSTATE_EXIT_CAR;
}

/*!
* @addr 5DF090
*/
bool CPed::CanStrafeOrMouseControl()
{
    return
        m_nPedState == PEDSTATE_IDLE ||
        m_nPedState == PEDSTATE_FLEE_ENTITY ||
        m_nPedState == PEDSTATE_FLEE_POSITION ||
        m_nPedState == PEDSTATE_NONE ||
        m_nPedState == PEDSTATE_AIMGUN ||
        m_nPedState == PEDSTATE_ATTACK ||
        m_nPedState == PEDSTATE_FIGHT ||
        m_nPedState == PEDSTATE_JUMP ||
        m_nPedState == PEDSTATE_ANSWER_MOBILE;
}

/*!
* @addr 0x5DF100
* @brief Check if ped can be deleted
* @returns Always false if ped is in vehicle or is follower of player's group.
*/
bool CPed::CanBeDeleted() {
    return !bInVehicle && !IsFollowerOfGroup(FindPlayerGroup()) && CanBeDeletedEvenInVehicle();
}

/*!
* @addr 0x5DF100
* @brief Check if ped can be deleted even if it's in a vehicle.
* @returns False only if created by PED_UNKNOWN or PED_MISSION, true otherwise.
*/
bool CPed::CanBeDeletedEvenInVehicle()
{
    switch (m_nCreatedBy) {
    case ePedCreatedBy::PED_MISSION:
    case ePedCreatedBy::PED_UNKNOWN:
        return false;
    }
    return true;
}

/*!
* @addr 0x5DF170
* @brief Remove goggles model, also disabled related PostFX.
*/
void CPed::RemoveGogglesModel() {
    if (!m_pGogglesObject) {
        return;
    }

    // Release model info
    CVisibilityPlugins::GetClumpModelInfo((RpClump*)m_pGogglesObject)->RemoveRef();

    // Remove atomics anim from skin
    if (const auto atomic = GetFirstAtomic((RpClump*)m_pGogglesObject)) {
        if (RpSkinGeometryGetSkin(RpAtomicGetGeometry(atomic))) {
            RpClumpForAllAtomics(m_pWeaponObject, AtomicRemoveAnimFromSkinCB, nullptr);
        }
    }

    // Destroy clump
    RpClumpDestroy(m_pWeaponObject);
    m_pWeaponObject = nullptr;

    // Disable FX's of the goggles. (See mem. var. `m_pGogglesState` in the header)
    if (m_pGogglesState) {
        *m_pGogglesState = false;
        m_pGogglesState = nullptr;
    }
}

/*!
* @addr   0x5DF200
* @return \a weaponType weapon's slot (\r CWeaponInfo::GetWeaponInfo()->slot)
*/
int32 CPed::GetWeaponSlot(eWeaponType weaponType)
{
    return CWeaponInfo::GetWeaponInfo(weaponType, eWeaponSkill::STD)->m_nSlot;
}

/*!
* @addr 0x5DF220
* @brief Set \a weaponType's slot totalAmmo to \a ammo. Also changes the gun's state to `READY`
*/
void CPed::GrantAmmo(eWeaponType weaponType, uint32 ammo) {
    const auto wepSlot = GetWeaponSlot(weaponType);
    if (wepSlot != -1) {
        auto& wepInSlot = GetWeaponInSlot(wepSlot);
        
        wepInSlot.m_nTotalAmmo = std::min(ammo, 99'999u); // Clamp upper

        // TODO: Inlined
        if (wepInSlot.m_nState == eWeaponState::WEAPONSTATE_OUT_OF_AMMO) {
            if (wepInSlot.m_nTotalAmmo > 0) {
                wepInSlot.m_nState = eWeaponState::WEAPONSTATE_READY;
            }
        }
    }
}

/*!
* @addr 0x5DF290
* @brief Im lazy to write it :D Similar to \r CPed::GrantAmmo
*/
void CPed::SetAmmo(eWeaponType weaponType, uint32 ammo) {
    const auto wepSlot = GetWeaponSlot(weaponType);
    if (wepSlot != -1) {
        auto& wepInSlot = GetWeaponInSlot(wepSlot);

        wepInSlot.m_nTotalAmmo = std::min(ammo, 99'999u);
        wepInSlot.m_nAmmoInClip = std::max(wepInSlot.m_nTotalAmmo, wepInSlot.m_nAmmoInClip);

        // TODO: Inlined
        if (wepInSlot.m_nState == eWeaponState::WEAPONSTATE_OUT_OF_AMMO) {
            if (wepInSlot.m_nTotalAmmo > 0) {
                wepInSlot.m_nState = eWeaponState::WEAPONSTATE_READY;
            }
        }
    }
}

/*!
* @addr 0x5DF300
* @brief Check if ped has a weapon of type \a weaponType
*/
bool CPed::DoWeHaveWeaponAvailable(eWeaponType weaponType) {
    const auto slot = GetWeaponSlot(weaponType);
    return slot != -1 && GetWeaponInSlot(slot).m_nType == weaponType;
}

/*!
* @addr 0x5DF340
* @brief Do gun flash by resetting it's alpha to max
* @returns Always true
*/
bool CPed::DoGunFlash(int32 arg0, bool bRightHand) {
    // Really elegant.. ;D
    if (bRightHand) {
        m_nWeaponGunflashAlphaMP2 = CPed::m_sGunFlashBlendStart;
        nm_fWeaponGunFlashAlphaProgMP2 = CPed::m_sGunFlashBlendStart / arg0;
    } else {
        m_nWeaponGunflashAlphaMP1 = CPed::m_sGunFlashBlendStart;
        nm_fWeaponGunFlashAlphaProgMP1 = CPed::m_sGunFlashBlendStart / arg0;
    }
    RwMatrixRotate(RwFrameGetMatrix(m_pGunflashObject), &CPedIK::XaxisIK, CGeneral::GetRandomNumberInRange(-360.f, 360.f), rwCOMBINEPRECONCAT);

    return true;
}

/*!
* @addr 0x5DF400
* @brief Set alpha of gun flash object
*/
void CPed::SetGunFlashAlpha(bool rightHand) {
    if (!m_pGunflashObject) {
        return;
    }

    if (m_nWeaponGunflashAlphaMP1 < 0 && m_nWeaponGunflashAlphaMP2 < 0) { // Reordered a little.
        return;
    }

    auto& gunFlashAlphaInHand = rightHand ? m_nWeaponGunflashAlphaMP2 : m_nWeaponGunflashAlphaMP1;

    if (auto atomic = (RpAtomic*)GetFirstObject(m_pGunflashObject)) {
        // They used a clever trick to not have to conver to float..
        // Then they converted to a float to check if the number is higher than 255.. XDDD
        if (gunFlashAlphaInHand < 0) {
            CVehicle::SetComponentAtomicAlpha(atomic, 0);
        } else {
            CVehicle::SetComponentAtomicAlpha(atomic, std::min(255, 350 * gunFlashAlphaInHand / m_sGunFlashBlendStart));
        }
        RpAtomicSetFlags(atomic, 4); // TODO: Use enum
    }

    if (!gunFlashAlphaInHand) {
        gunFlashAlphaInHand = (uint16)-1;
    }
}

/*!
* @addr 0x5DF4E0
* @brief Reset alpha of gun flash object
*/
void CPed::ResetGunFlashAlpha() {
    if (m_pGunflashObject) {
        if (auto atomic = (RpAtomic*)GetFirstObject(m_pGunflashObject)) {
            RpAtomicSetFlags(atomic, 4); // TODO: Use enum
            CVehicle::SetComponentAtomicAlpha(atomic, 0);
        }
    }
}

/*!
* @addr 0x5DF510
* @returns If ped is a player returns stat value BIKE_SKILL, otherwise 1 for mission peds and 0 for all others.
*/
float CPed::GetBikeRidingSkill() {
    if (m_pPlayerData) {
        return std::min(1000.f, CStats::GetStatValue(eStats::STAT_BIKE_SKILL) / 1000.f);
    }
    return IsCreatedByMission() ? 1.f : 0.f;
}

/*!
* @addr 0x5DF560
* @brief Deal with sholder bone (clavicle) roation based on arm and breast rotation
*/
void CPed::ShoulderBoneRotation(RpClump* clump) {
    // Note: Didn't use `GetBoneMatrix` here, because it would be slower
    // (Because it would call `GetAnimHierarchyFromClump` multiple tiems)
    auto GetMatrixOf = [hier = GetAnimHierarchyFromClump(clump)](ePedBones bone) mutable -> RwMatrix& {
        return (RpHAnimHierarchyGetMatrixArray(hier))[RpHAnimIDGetIndex(hier, (size_t)bone)];
    };

    constexpr struct { ePedBones breast, upperArm, clavicle; } bones[]{
        {ePedBones::BONE_L_BREAST, ePedBones::BONE_L_UPPER_ARM, ePedBones::BONE_L_CLAVICLE},
        {ePedBones::BONE_R_BREAST, ePedBones::BONE_R_UPPER_ARM, ePedBones::BONE_R_CLAVICLE},
    };

    // Update left, and right sides
    for (auto [breast, upperArm, clavicle] : bones) {
        auto& breastRwMat = GetMatrixOf(breast);

        // Update matrix of l_breast to be the same as l_upper_arm's
        breastRwMat = GetMatrixOf(upperArm);

        CMatrix breastMat{ &breastRwMat };
        CMatrix clavicleMat{ &GetMatrixOf(clavicle) };

        // Transform matrix from left_clavicle's space to left_breast's
        breastMat *= Invert(clavicleMat); 

        // Half it's X rotation

        float x, y, z;
        breastMat.ConvertToEulerAngles(&x, &y, &z, ORDER_ZYX | SWAP_XZ);
        // Originally there is an `if` check of a static bool value, which is always true.
        x /= 2.f;
        breastMat.ConvertFromEulerAngles(x, y, z, ORDER_ZYX | SWAP_XZ);

        // Transform it back into it's own space
        breastMat *= clavicleMat;

        // Finally, update it's RW associated matrix
        breastMat.UpdateRW();      
    }
}

/*!
* @addr 0x5DF8D0
* @brief Set look timer relative to now, but only if it has expired.
* @param time Time the timer ends relative to now
*/
void CPed::SetLookTimer(uint32 time) {
    if (CTimer::GetTimeInMS() > m_nLookTime) {
        m_nLookTime = CTimer::GetTimeInMS() + time;
    }
}

/*!
* @addr 0x5DF8F0
*/
bool CPed::IsPlayer() const
{
    return m_nPedType == PED_TYPE_PLAYER1 || m_nPedType == PED_TYPE_PLAYER2;
}

/*!
* @addr 0x5DF910
*/
void CPed::SetPedPositionInCar() {
    ((void(__thiscall *)(CPed*))0x5DF910)(this);
}

/*!
* @addr 0x5DFD60
* @brief Set head changing rate to value stored in m_pStats
*/
void CPed::RestoreHeadingRate() {
    m_fHeadingChangeRate = m_pStats->m_fHeadingChangeRate;
}

/*!
* @addr 0x5DFD70
*/
void CPed::RestoreHeadingRateCB(CAnimBlendAssociation* assoc, void* data) {
    UNUSED(assoc);

    auto& ped = *((CPed*)data);
    ped.m_fHeadingChangeRate = ped.m_pStats->m_fHeadingChangeRate;
}

/*!
* @addr 0x5DFD90
* @brief Set random radio station if ped is in car. The station is chosen randomly, and is either `m_nRadio1` or `m_nRadio2` from the ped's \r CPedModelInfo
*/
void CPed::SetRadioStation()
{
    if (!IsPlayer() && m_pVehicle) {
        if (m_pVehicle->m_pDriver == this) {
            const auto& mi = *(CPedModelInfo*)GetModelInfo();
            m_pVehicle->m_vehicleAudio.m_settings.m_nRadioID = (rand() <= RAND_MAX / 2) ? mi.m_nRadio1 : mi.m_nRadio2;
        }
    }
}

/*!
* @addr 0x5DFDF0
*/
void CPed::PositionAttachedPed()
{
    ((void(__thiscall *)(CPed*))0x5DFDF0)(this);
}

/*!
* @addr 0x5E00F0
* @brief Remove ped from the world, and request special model for it.
*/
void CPed::Undress(char* modelName) {
    DeleteRwObject();
    CStreaming::RequestSpecialModel(IsPlayer() ? 0 : m_nModelIndex, modelName, STREAMING_KEEP_IN_MEMORY | STREAMING_MISSION_REQUIRED);
    CWorld::Remove(this);
}

/*!
* @addr 0x5E0130
* @brief Re-add ped to the world
*/
void CPed::Dress() {
    SetModelIndex(m_nModelIndex);
    if (m_nPedState != ePedState::PEDSTATE_DRIVING) {
        m_nPedState = ePedState::PEDSTATE_IDLE;
    }
    CWorld::Add(this);
    RestoreHeadingRate();
}

/*!
* @addr 0x5E0170
*/
// Checks if the Pedestrian is still alive.
bool CPed::IsAlive()
{
    return m_nPedState != PEDSTATE_DIE && m_nPedState != PEDSTATE_DEAD;
}

/*!
* @addr 0x5E01A0
*/
void CPed::UpdateStatEnteringVehicle()
{
    // NOP
}

/*!
* @addr 0x5E01B0
*/
void CPed::UpdateStatLeavingVehicle()
{
    // NOP
}

/*!
* @addr 0x5E01C0
* @brief Transform \r inOffsetOutPosn into the given \r bone's space
*
* @param [in,out] inOffsetOutPosn The position to be transformed in-place.
* @param          updateSkinBones If `UpdateRpHAnim` should be called
*/
void CPed::GetTransformedBonePosition(RwV3d& inOffsetOutPosn, ePedBones bone, bool updateSkinBones) {
    // Pretty much the same as GetBonePosition..
    if (updateSkinBones) {
        if (!bCalledPreRender) {
            UpdateRpHAnim();
            bCalledPreRender = true;
        }
    } else if (!bCalledPreRender) { // Return static local bone position instead
        inOffsetOutPosn = MultiplyMatrixWithVector(*m_matrix, GetPedBoneStdPosition(bone));
        return;
    }

    // Return actual position
    RwV3dTransformPoints(&inOffsetOutPosn, &inOffsetOutPosn, 1, &GetBoneMatrix(bone));
}

/*!
* @addr 0x5E0270
* @brief Release current cover point
*/
void CPed::ReleaseCoverPoint() {
    if (m_pCoverPoint) {
        m_pCoverPoint->ReleaseCoverPointForPed(this);
        m_pCoverPoint = nullptr;
    }
}

/*!
* @addr 0x5E0290
* @returns Any active task of type HOLD_ENTITY, PICKUP_ENTITY, PUTDOWN_ENTITY.
*/
CTaskSimpleHoldEntity* CPed::GetHoldingTask() {
    // Man programming in C++03 must've been a pain.. if, if, if, if, if, if... IF. 
    if (const auto task = GetTaskManager().FindActiveTaskFromList({ TASK_SIMPLE_HOLD_ENTITY, TASK_SIMPLE_PICKUP_ENTITY, TASK_SIMPLE_PUTDOWN_ENTITY })) {
        return task->As<CTaskSimpleHoldEntity>();
    }
    return nullptr;
}

/*!
* @addr 0x5E02E0
*/
CEntity* CPed::GetEntityThatThisPedIsHolding()
{
    if (const auto task = GetHoldingTask()) {
        return task->m_pEntityToHold;
    }

    if (const auto task = GetTaskManager().FindActiveTaskByType(TASK_COMPLEX_GO_PICKUP_ENTITY)) {
        return task->As<CTaskComplexGoPickUpEntity>()->m_pEntity;
    }

    return nullptr;
}

/*!
* @addr   0x5E0360
* @brief Drop held entity, possibly deleting it.
*/
void CPed::DropEntityThatThisPedIsHolding(bool bDeleteHeldEntity) {
    if (const auto task = GetHoldingTask()) {
        // Drop the entity
        task->DropEntity(this, true);

        // Delete held entity (If any)
        if (bDeleteHeldEntity) {
            if (const auto heldEntity = task->m_pEntityToHold) {
                if (!heldEntity->IsObject() || !heldEntity->AsObject()->IsMissionObject()) {
                    heldEntity->DeleteRwObject(); // TODO; Are these 3 lines inlined?
                    CWorld::Remove(heldEntity);
                    delete heldEntity;
                }
            }
        }
    }
}

/*!
* @addr 0x5E0400
* @returns If there's a holding task and the held entity can be thrown.
*/
bool CPed::CanThrowEntityThatThisPedIsHolding() {
    if (const auto task = GetHoldingTask()) {
        return task->CanThrowEntity();
    }
    return false;
}

/*!
* @addr 0x5E0460
* @returns If there's a HANDSIGNAL task
*/
bool CPed::IsPlayingHandSignal() {
    return !!GetTaskManager().FindActiveTaskByType(eTaskType::TASK_COMPLEX_HANDSIGNAL_ANIM);
}

/*!
* @addr 0x5E0480
* @brief Stop the HANDSINGAL task
*/
void CPed::StopPlayingHandSignal() {
    if (const auto task = GetTaskManager().FindActiveTaskByType(eTaskType::TASK_COMPLEX_HANDSIGNAL_ANIM)) {
        task->MakeAbortable(this, ABORT_PRIORITY_URGENT, nullptr);
    }
}

/*!
* @addr 0x5E04B0
* @returns Get walk speed in units/s based on the ped's anim group's WALK anim.
*/
float CPed::GetWalkAnimSpeed() {
    auto hier = CAnimManager::GetAnimAssociation(m_nAnimGroup, nullptr)->m_pHierarchy;
    CAnimManager::UncompressAnimation(hier);
    auto& firstSequence = hier->GetSequences()[ANIM_ID_WALK];

    if (!firstSequence.m_nFrameCount) {
        return 0.f; // No frames
    }

    // NOTE: This is quite garbage, based on at least 5 assumptions, more of a hack than a solution from R*'s side.
    //       It won't work correctly if first frame is not a root frame, nor if the animation happens on any other axis than Y, etc..

    const auto lastFrame = firstSequence.GetUncompressedFrame(firstSequence.m_nFrameCount - 1);
    const auto lastFrameY = firstSequence.m_isRoot ? 
        lastFrame->m_vecTranslation.y :
        ((CAnimSequenceChildFrameUncompressed*)lastFrame)->m_quat.imag.y;

    return (lastFrameY - firstSequence.GetUncompressedFrame(0)->m_vecTranslation.y) / hier->m_fTotalTime;
}

/*!
* @addr 0x5E06E0
*/
void CPed::SetPedDefaultDecisionMaker() {
    if (!IsPlayer()) {
        GetIntelligence()->SetPedDecisionMakerType(IsCreatedByMission() ? -1 : m_pStats->m_nDefaultDecisionMaker);
    } else {
        GetIntelligence()->SetPedDecisionMakerType(-2);
    }
}

/*!
* @addr 0x5E0730
* @returns If entity is a given range angle relative to our current rotation given by \r limitAngle [-limitAngle, limitAngle]
*/
bool CPed::CanSeeEntity(CEntity* entity, float limitAngle) {

    // TODO: Inlined? 0x5E0780, 0x5E07BB
    const auto FixRadianAngle = [](float angle) {
        if (angle < TWO_PI) {
            if (angle < 0.f) {
                return angle + TWO_PI;
            }
        } else {
            return angle - TWO_PI;
        }
        return angle;
    };

    // R* used the degree returning function, and converted to radians, we just use the radian version directly
    const auto pointAngle = FixRadianAngle(CGeneral::GetRadianAngleBetweenPoints(entity->GetPosition2D(), GetPosition2D()));

    const auto delta = std::abs(m_fCurrentRotation - pointAngle);
    return delta < limitAngle || delta > TWO_PI - limitAngle;
}

/*!
* @addr 0x5E0820
*/
bool CPed::PositionPedOutOfCollision(int32 exitDoor, CVehicle* vehicle, bool findClosestNode)
{
    return ((bool(__thiscall *)(CPed*, int32, CVehicle*, bool))0x5E0820)(this, exitDoor, vehicle, findClosestNode);
}

/*!
* @addr     0x5E13C0
* @brief    Teleport ped to the furthest point from here that isn't colliding with a building or ped.
* @returns  If there was at lest one such point.
*/
bool CPed::PositionAnyPedOutOfCollision() {
    struct Point {
        CVector pos{};
        bool    found{};
        float   distSq{};
    };
    Point vehiclePoint{}, noCollPoint{};

    // Find 2 points furthest away from us:
    // - One that is colliding with a vehicle, (vehiclePoint)
    // - One that isn't colliding with one. (noCollPoint)
    // Neither point should be colliding with a building or ped.
    auto testPoint{ GetPosition() };
    for (auto y = 0; y < 15; y++) {
        testPoint.y -= 3.5f;
        for (auto x = 0; x < 15; x++) {
            testPoint.x -= 3.5f;

            // If we collide with a building or ped skip
            if (!CWorld::TestSphereAgainstWorld(testPoint, 0.6f, this, /*buildings: */true, false, false, true, false, false)) {
                continue;
            }

            const auto PossiblyUpdatePoint = [&, this](Point& p) {
                const auto dist{ (testPoint - GetPosition()).SquaredMagnitude() };
                if (dist > p.distSq) {
                    p.pos = testPoint;
                    p.distSq = dist;
                    p.found = true;
                }
            };

            // Check for collision with vehicles
            if (CWorld::TestSphereAgainstWorld(testPoint, 0.6f, this, false, /*vehicles: */true, false, false, false, false)) {
                PossiblyUpdatePoint(vehiclePoint);
            } else { // Not collided with anything
                PossiblyUpdatePoint(noCollPoint);
            }
        }
    }

    // Set our position to one of the point's
    if (noCollPoint.found) { // If there was a point not colliding with a vehicle use it
        SetPosn(noCollPoint.pos);
    } else if (vehiclePoint.found) { // Otherwise use the other one collided with one (if any)
        SetPosn(vehiclePoint.pos + CVector{ 0.f, 0.f, GetBoundingBox().m_vecMax.z });
    }

    return noCollPoint.found || vehiclePoint.found;
}

/*!
* @addr 0x5E1660
*/
bool CPed::OurPedCanSeeThisEntity(CEntity* entity, bool isSpotted) {
    if (!isSpotted) {
        const auto dir2D{ entity->GetPosition2D() - GetPosition2D() };
        if (   DotProduct2D(dir2D, m_matrix->GetForward()) < 0.f // Is behind us
            || dir2D.SquaredMagnitude() >= 40.f * 40.f           // Using SqMag instead of Mag
        ) {
            return false;
        }
    }

    auto target{entity->GetPosition()};
    if (entity->IsPed()) {
        target.z += 1.f; // Adjust for head pos?
    }
    
    // Seems like they explicitly use this one instead of `IsLineOfSightClear` because of the `shootThru` check.
    CColPoint cp{};
    CEntity* hitEntity{};
    return !CWorld::ProcessLineOfSight(GetPosition(), target, cp, hitEntity, true, false, false, isSpotted, false, false, false, isSpotted);
}

/*!
* @addr 0x5E17E0
* @unused
*/
void CPed::SortPeds(CPed** pedList, int32 arg1, int32 arg2)
{
    ((void(__thiscall *)(CPed*, CPed**, int32, int32))0x5E17E0)(this, pedList, arg1, arg2);
}

/*!
* @addr 0x5E1A00
* @brief Update `m_fFPSMoveHeading` depending on the ped's Up/Down, Left/Right control states.
*/
float CPed::WorkOutHeadingForMovingFirstPerson(float heading) {
    if (!IsPlayer() || !m_pPlayerData) {
        return 0.f; // Probably shouldn't ever happen, but okay
    }

    const auto walkUpDown = (float)CPad::GetPad()->GetPedWalkUpDown();
    const auto walkLeftRight = (float)CPad::GetPad()->GetPedWalkLeftRight();
    if (walkUpDown == 0.f) {
        if (walkLeftRight != 0.f) {
            m_pPlayerData->m_fFPSMoveHeading = walkLeftRight < 0.f ? HALF_PI : -HALF_PI;
        }
    } else {
        m_pPlayerData->m_fFPSMoveHeading = CGeneral::GetRadianAngleBetweenPoints(0.f, 0.f, -walkLeftRight, walkUpDown);
    }

    return CGeneral::LimitRadianAngle(heading + m_pPlayerData->m_fFPSMoveHeading);
}

/*!
* @addr 0x5E1B10
*/
void CPed::UpdatePosition()
{
    ((void(__thiscall *)(CPed*))0x5E1B10)(this);
}

/*!
* @addr 0x5E1FA0
*/
void CPed::ProcessBuoyancy()
{
    if (bInVehicle)
        return;

    float fBuoyancyMult = 1.1F;
    if (!IsAlive())
        fBuoyancyMult = 1.8F;

    float fBuoyancy = fBuoyancyMult * m_fMass / 125.0F;
    CVector vecBuoyancyTurnPoint;
    CVector vecBuoyancyForce;
    if (!mod_Buoyancy.ProcessBuoyancy(this, fBuoyancy, &vecBuoyancyTurnPoint, &vecBuoyancyForce)) {
        physicalFlags.bTouchingWater = false;
        auto swimTask = m_pIntelligence->GetTaskSwim();
        if (swimTask)
            swimTask->m_fSwimStopTime = 1000.0F;

        return;
    }

    if (bIsStanding) {
        auto& standingOnEntity = m_pContactEntity;
        if (standingOnEntity && standingOnEntity->IsVehicle()) {
            auto pStandingOnVehicle = standingOnEntity->AsVehicle();
            if (pStandingOnVehicle->IsBoat() && !pStandingOnVehicle->physicalFlags.bDestroyed) {
                physicalFlags.bSubmergedInWater = false;
                auto swimTask = m_pIntelligence->GetTaskSwim();
                if (!swimTask)
                    return;

                swimTask->m_fSwimStopTime += CTimer::GetTimeStep();
                return;
            }
        }
    }

    if (m_pPlayerData) {
        const auto& vecPedPos = GetPosition();
        float fCheckZ = vecPedPos.z - 3.0F;
        CColPoint lineColPoint;
        CEntity* colEntity;
        if (CWorld::ProcessVerticalLine(vecPedPos, fCheckZ, lineColPoint, colEntity, false, true, false, false, false, false, nullptr)) {
            if (colEntity->IsVehicle()) {
                auto colVehicle = colEntity->AsVehicle();
                if (colVehicle->IsBoat()
                    && !colVehicle->physicalFlags.bDestroyed
                    && colVehicle->GetMatrix().GetUp().z > 0.0F) {

                    physicalFlags.bSubmergedInWater = false;
                    return;
                }
            }
        }
    }

    // Those 4 are called for some reason
    /*
    CTimeCycle::GetAmbientRed();
    CTimeCycle::GetAmbientGreen();
    CTimeCycle::GetAmbientBlue();
    rand();
    */

    // Add splash particle if it's the first frame we're touching water, and
    // the movement of ped is downward, preventing particles from being created
    // if ped is standing still and water wave touches him
    if (!physicalFlags.bTouchingWater && m_vecMoveSpeed.z < -0.01F) {
        auto vecMoveDir = m_vecMoveSpeed * CTimer::GetTimeStep() * 4.0F;
        auto vecSplashPos = GetPosition() + vecMoveDir;
        float fWaterZ;
        if (CWaterLevel::GetWaterLevel(vecSplashPos.x, vecSplashPos.y, vecSplashPos.z, &fWaterZ, true, nullptr)) {
            vecSplashPos.z = fWaterZ;
            g_fx.TriggerWaterSplash(vecSplashPos);
            AudioEngine.ReportWaterSplash(this, -100.0F, true);
        }
    }

    physicalFlags.bTouchingWater = true;
    physicalFlags.bSubmergedInWater = true;
    ApplyMoveForce(vecBuoyancyForce);

    if (CTimer::GetTimeStep() / 125.0F < vecBuoyancyForce.z / m_fMass
        || GetPosition().z + 0.6F < mod_Buoyancy.m_fWaterLevel) {

        bIsStanding = false;
        bIsDrowning = true;

        bool bPlayerSwimmingOrClimbing = false;
        if (!IsPlayer()) {
            CEventInWater cEvent(0.75F);
            GetEventGroup().Add(&cEvent, false);
        }
        else {
            auto swimTask = m_pIntelligence->GetTaskSwim();
            if (swimTask) {
                swimTask->m_fSwimStopTime = 0.0F;
                bPlayerSwimmingOrClimbing = true;
            }
            else if (m_pIntelligence->GetTaskClimb()) {
                bPlayerSwimmingOrClimbing = true;
            }
            else {
                auto fAcceleration = vecBuoyancyForce.z / (CTimer::GetTimeStep() * m_fMass / 125.0F);
                CEventInWater cEvent(fAcceleration);
                GetEventGroup().Add(&cEvent, false);
            }

            if (bPlayerSwimmingOrClimbing)
                return;
        }

        float fTimeStep = pow(0.9F, CTimer::GetTimeStep());
        m_vecMoveSpeed.x *= fTimeStep;
        m_vecMoveSpeed.y *= fTimeStep;
        if (m_vecMoveSpeed.z < 0.0F)
            m_vecMoveSpeed.z *= fTimeStep;

        return;
    }

    auto swimTask = m_pIntelligence->GetTaskSwim();
    if (bIsStanding && swimTask)
    {
        swimTask->m_fSwimStopTime += CTimer::GetTimeStep();
        return;
    }

    if (m_pPlayerData) {
        CVector vecHeadPos(0.0F, 0.0F, 0.1F);
        GetTransformedBonePosition(vecHeadPos, ePedBones::BONE_HEAD, false);
        if (vecHeadPos.z < mod_Buoyancy.m_fWaterLevel) {
            AsPlayer()->HandlePlayerBreath(true, 1.0F);
        }
    }
}

/*!
* @addr 0x5E3960
* @returns If ped can be moved, that is: not in the air or landing, is not arrested/dead/dying.
*/
bool CPed::IsPedInControl()
{
    if (!bIsLanding && !bIsInTheAir)
    {
        if (m_nPedState != PEDSTATE_DIE && m_nPedState != PEDSTATE_DEAD && m_nPedState != PEDSTATE_ARRESTED)
            return true;
    }
    return false;
}

/*!
* @addr 0x5E3990
* @brief Remove current weapon's object model. \r modelIndex should be the same as with which the object was created with.
*/
void CPed::RemoveWeaponModel(int32 modelIndex) {

    // For players remove any attached FX (Created in `AddWeaponModel` for molotov)
    if (IsPlayer()) {
        auto& activeWep = GetActiveWeapon();
        if (activeWep.m_pFxSystem) {
            g_fxMan.DestroyFxSystem(activeWep.m_pFxSystem);
            activeWep.m_pFxSystem = nullptr;
        }
    }

    // Deal with weapon's loaded clump (if any)
    if (m_pWeaponObject) {
        if (   modelIndex == -1
            || CModelInfo::GetModelInfo(modelIndex) == CVisibilityPlugins::GetClumpModelInfo(m_pWeaponObject)
        ) {
            // Release model info
            CVisibilityPlugins::GetClumpModelInfo(m_pWeaponObject)->RemoveRef(); 

            // Remove atomics anim from skin
            if (const auto atomic = GetFirstAtomic(m_pWeaponObject)) {
                if (RpSkinGeometryGetSkin(RpAtomicGetGeometry(atomic))) {
                    RpClumpForAllAtomics(m_pWeaponObject, AtomicRemoveAnimFromSkinCB, nullptr);
                }
            }

            // Destroy clump
            RpClumpDestroy(m_pWeaponObject);
            m_pWeaponObject = nullptr;
            m_pGunflashObject = nullptr;
        }
    }

    m_nWeaponGunflashAlphaMP1 = 0;
    m_nWeaponGunflashAlphaMP2 = 0;
    m_nWeaponModelId = -1;
}

/*!
* @addr 0x5E3A90
* @brief Createa goggles model for current infrared/nightvision. See \r PutOnGoggles.
*/
void CPed::AddGogglesModel(int32 modelIndex, bool & inOutGogglesState) {
    if (modelIndex != -1) {
        m_pGogglesObject = (RwFrame*)CModelInfo::GetModelInfo(modelIndex)->CreateInstanceAddRef();

        m_pGogglesState = &inOutGogglesState;
        inOutGogglesState = true;
    }
}

/*!
* @addr 0x5E3AE0
* @brief Puts on goggles if current weapon is infrared/nightvision. (Also removes weapon model from hand and enabled corresponding PostFX)
*/
void CPed::PutOnGoggles() {
    auto& wepInSlot = GetWeaponInSlot(GetWeaponSlot(eWeaponType::WEAPON_INFRARED));

    // Game checks if wepInSlot.m_nType != UNARMED here, not sure why? Probably compiler mistake on switch case codegen.. 

    switch (wepInSlot.m_nType) {
    case eWeaponType::WEAPON_INFRARED:
    case eWeaponType::WEAPON_NIGHTVISION: {

        // Add(load) googles model and enable PostFX 
        const auto DoAddGogglesModel = [&, this](bool& state) {
            AddGogglesModel(wepInSlot.GetWeaponInfo().m_nModelId1, state);
        };

        switch (wepInSlot.m_nType) {
        case eWeaponType::WEAPON_INFRARED:
            DoAddGogglesModel(CPostEffects::m_bInfraredVision);
            break;
        case eWeaponType::WEAPON_NIGHTVISION:
            DoAddGogglesModel(CPostEffects::m_bNightVision);
            break;
        }

        // Make sure weapon model doesn't get loaded (Because we've put the them on)
        wepInSlot.m_bNoModel = true;

        // If it was the active weapon: unload it's weapon model
        if (&wepInSlot == &GetActiveWeapon()) {
            RemoveWeaponModel(wepInSlot.GetWeaponInfo().m_nModelId1);
        }

        break;
    }
    }
}

/*!
* @addr    0x5E6580
* @returns Weapon skill with current weapon
*/
eWeaponSkill CPed::GetWeaponSkill()
{
    return GetWeaponSkill(m_aWeapons[m_nActiveWeaponSlot].m_nType);
}

/*!
* @addr     0x5E3B60
* @returns Skill with \a weaponType. In case we're a player this is the stat with the given weapon, otherwise the mem. var. `m_nWeaponSkill`.
*/
eWeaponSkill CPed::GetWeaponSkill(eWeaponType weaponType)
{
    if ( weaponType < WEAPON_PISTOL || weaponType > WEAPON_TEC9 )
        return eWeaponSkill::STD;

    if (!m_nPedType || m_nPedType == PED_TYPE_PLAYER2)
    {
        int32 skillStat = CWeaponInfo::GetSkillStatIndex(weaponType);
        CWeaponInfo* pGolfClubWeaponInfo = CWeaponInfo::GetWeaponInfo(weaponType, eWeaponSkill::PRO);
        float golfClubStatLevel = static_cast<float>(pGolfClubWeaponInfo->m_fReqStatLevel);
        if (golfClubStatLevel <= CStats::GetStatValue((eStats)skillStat))
            return eWeaponSkill::PRO;

        CWeaponInfo* brassKnuckleWeaponInfo = CWeaponInfo::GetWeaponInfo(weaponType, eWeaponSkill::STD);
        float brassKnuckleStatLevel = static_cast<float>(brassKnuckleWeaponInfo->m_fReqStatLevel);
        if (brassKnuckleStatLevel > CStats::GetStatValue((eStats)skillStat))
            return eWeaponSkill::POOR;

        return eWeaponSkill::STD;
    }

    if (weaponType != WEAPON_PISTOL || m_nPedType != PED_TYPE_COP)
        return m_nWeaponSkill;

    return eWeaponSkill::COP;
}

/*!
* @addr 0x5E3C10
* @brief Set weapon skill, unless we're a player ped (\r IsPlayer)
*/
void CPed::SetWeaponSkill(eWeaponType weaponType, eWeaponSkill skill)
{
    if (!IsPlayer()) {
        m_nWeaponSkill = skill;
    }
}

/*!
* @addr 0x5E1950
* @brief Clear ped look, and start restoing it
*/
void CPed::ClearLookFlag() {
    if (!bIsLooking) {
        return;
    }

    // Originally there's a do-while loop, but it will never iterate more than once, so I won't add it.
    // do { ...

    bIsLooking = false;
    bIsDrowning = false;
    bIsRestoringLook = true;

    if (CanUseTorsoWhenLooking()) {
        m_pedIK.bTorsoUsed = false;
    }

    m_nLookTime = CTimer::GetTimeInMS() + (IsPlayer() ? 4000 : 2000);

    // .. } while ((PEDSTATE_LOOK_HEADING || PEDSTATE_LOOK_ENTITY) && bIsLooking), but `bIsLooking` will never be true at this point.
}

/*!
* @addr 0x5E3FF0
* @brief Just calls \r ClearLookFlag
*/
void CPed::ClearLook() {
    ClearLookFlag();
}

/*!
* @addr 0x5E4000
*/
/*!
* @addr    0x5E4000
* @brief   Turns ped to look at `m_pLookTarget`.
* @returns If `m_fCurrentRotation` changed.
*/
bool CPed::TurnBody() {
    if (m_pLookTarget) {
        m_fLookDirection = CGeneral::GetRadianAngleBetweenPoints(m_pLookTarget->GetPosition2D(), GetPosition2D());
    }

    m_fLookDirection = CGeneral::LimitRadianAngle(m_fLookDirection);

    // Some logic to make sure `m_fCurrentRotation` is always in the range [-PI, PI] or [0, 2PI] ? Not sure.. TODO.
    if (m_fCurrentRotation + PI >= m_fLookDirection) {
        if (m_fCurrentRotation - PI > m_fLookDirection) {
            m_fCurrentRotation += PI;
        }
    } else {
        m_fCurrentRotation -= PI;
    }

    m_fAimingRotation = m_fLookDirection;

    if (std::abs(m_fCurrentRotation - m_fLookDirection) <= 0.05f) {
        return true;
    } else {
        m_fCurrentRotation -= (m_fCurrentRotation - m_fLookDirection) / 5.f;
        return false;
    }
}

/*!
* @addr 0x5E4220
* @brief Check if `this` is valid. Probably used by scripts?
*/
bool CPed::IsPointerValid() {
    return CPools::GetPedPool()->IsObjectValid(this) && (!m_pCollisionList.IsEmpty() || this == FindPlayerPed());
}

/*!
* @addr 0x5E4280
* @brief Retrieve object-space position of the given \a bone.
* @param updateSkinBones if not already called `UpdateRpHAnim` will be called. If this param is not set, and the latter function wasn't yet called a default position will be returned.
*/
void CPed::GetBonePosition(RwV3d& outPosition, ePedBones bone, bool updateSkinBones) {
    if (updateSkinBones) {
        if (!bCalledPreRender) {
            UpdateRpHAnim();
            bCalledPreRender = true;
        }
    } else if (!bCalledPreRender) { // Return static local bone position instead
        outPosition = MultiplyMatrixWithVector(*m_matrix, GetPedBoneStdPosition(bone));
        return;
    }

    if (const auto hier = GetAnimHierarchyFromSkinClump(m_pRwClump)) { // Use position of bone matrix from anim hierarchy (if any)
        RwV3dAssign(&outPosition, RwMatrixGetPos(&RpHAnimHierarchyGetMatrixArray(hier)[bone])); // IMPORTANT NOTE: And C fanboys consider this readable..
    } else { // Not sure when can this happen.. GetTransformedBonePosition doesn't check this case.
        outPosition = GetPosition(); // Return something close to valid..
        assert(0); // NOTSA: Let's see if this is possible at all. 
    }
}

/*!
* @addr 0x5E4390
* @brief Give ped an object to hold. If he already has one and \a replace is `true` they will drop it, otherwise do nothing.
*/
void CPed::GiveObjectToPedToHold(int32 modelIndex, uint8 replace) {

    // Deal with ped already holding an entity.
    // If `replace` is `true`, just drop the entity, otherwise do nothing.
    if (const auto task = GetTaskManager().FindActiveTaskByType(eTaskType::TASK_SIMPLE_HOLD_ENTITY)) {
        if (!GetEntityThatThisPedIsHolding() || !replace) {
            return;
        }
        DropEntityThatThisPedIsHolding(true);
    }

    // Create object
    auto object = new CObject(modelIndex, false);
    object->SetPosn(GetPosition());
    CWorld::Add(object);

    // Create task
    CVector pos{ 0.f, 0.f, 0.f };
    GetTaskManager().SetTaskSecondary(new CTaskSimpleHoldEntity(object, &pos, PED_NODE_RIGHT_HAND), TASK_SECONDARY_PARTIAL_ANIM);
}

/*!
* @addr  0x5E4500
* @brief Set ped's state. If he's now !IsAlive() blip is deleted (if `bClearRadarBlipOnDeath` is set) and `ReleaseCoverPoint()` is called.
*/
void CPed::SetPedState(ePedState pedState) {
    m_nPedState = pedState;
    if (!IsAlive()) {
        ReleaseCoverPoint();
        if (bClearRadarBlipOnDeath) {
            CRadar::ClearBlipForEntity(BLIP_CHAR, CPools::GetPedPool()->GetRef(this));
        }
    }
}

/*!
* @addr 0x5E47E0
* @brief Set ped's created by. If created by mission, set it's hearing and seeing range to 30.
*/
void CPed::SetCharCreatedBy(ePedCreatedBy createdBy) {
    m_nCreatedBy = createdBy;

    SetPedDefaultDecisionMaker();

    if (IsCreatedByMission()) {
        auto intel = GetIntelligence();

        intel->SetSeeingRange(30.f);
        intel->SetHearingRange(30.f);
        if (!IsPlayer()) {
            intel->m_fDmRadius = 0.f;
            intel->m_nDmNumPedsToScan = 0;
        }
    }
}

/*!
* @addr 0x5E4C50
*/
void CPed::CalculateNewVelocity()
{
    ((void(__thiscall *)(CPed*))0x5E4C50)(this);
}

/*!
* @addr 0x5E52E0
*/
void CPed::CalculateNewOrientation() {
    if (CReplay::Mode != REPLAY_MODE_1 && IsPedInControl()) {
        SetOrientation(0.f, 0.f, m_fCurrentRotation);
    }
}

/*!
* @addr 0x5E5320
*/
void CPed::ClearAll() {
    if (IsPedInControl() || IsStateDead()) {
        bRenderPedInCar = true;
        bHitSteepSlope = false;
        bCrouchWhenScared = false;
        m_nPedState = ePedState::PEDSTATE_NONE;
        m_nMoveState = eMoveState::PEDMOVE_NONE;
        m_pEntityIgnoredCollision = nullptr;
    }
}

/*!
* @addr 0x5E5380
*/
void CPed::DoFootLanded(bool leftFoot, uint8 arg1)
{
    ((void(__thiscall *)(CPed*, bool, uint8))0x5E5380)(this, leftFoot, arg1);
}

/*!
* @addr 0x5E57F0
*/
void CPed::PlayFootSteps() {
    auto& anim = *RpAnimBlendClumpGetFirstAssociation(m_pRwClump);

    const auto IsWalkRunSprintAnim = [&] {
        switch (anim.m_nAnimId) {
        case AnimationId::ANIM_ID_WALK:
        case AnimationId::ANIM_ID_RUN:
        case AnimationId::ANIM_ID_SPRINT: {
            return true;
        }
        }
        return false;
    };

    if (bDoBloodyFootprints) {
        if (m_nDeathTime && m_nDeathTime < 300) {
            m_nDeathTime -= 1;
            if (!m_nDeathTime) {
                bDoBloodyFootprints = false;
            }
        }
    }

    if (!bIsStanding) {
        return;
    }

    // 0x5E58FB, 0x5E5A0B, 0x5E5AB9, 0x5E5E64, 0x5E5D87
    const auto DoProcessLanding = [this] {
        if (bIsLanding) { // Redudant check.. probably inlined function?
            if (const auto task = GetTaskManager().GetSimplestActiveTask(); task->GetTaskType() == eTaskType::TASK_SIMPLE_LAND) {
                auto landedTask = task->As<CTaskSimpleLand>();
                if (landedTask->RightFootLanded()) {
                    DoFootLanded(false, true);
                } else if (landedTask->RightFootLanded()) {
                    DoFootLanded(true, true);
                }
            }
        }
    };

    // TODO: Is this inlined? What is this doing exactly? (Anim stuff)
    float blendAmount1{}, blendAmount2{};
    CAnimBlendAssociation* lastAssocWithFlag100{};
    auto* lastAssoc = &anim;
    do { // 0x5E58A1
        if (lastAssoc->m_nFlags & ANIM_FLAG_100) {
            blendAmount1 += lastAssoc->m_fBlendAmount;
            lastAssocWithFlag100 = lastAssoc;
        } else {
            if ((lastAssoc->m_nFlags & ANIM_FLAG_ADD_TO_BLEND) == 0) {
                if (lastAssoc->m_nAnimId != ANIM_ID_FIGHT_IDLE) {
                    if (lastAssoc->m_nFlags & ANIM_FLAG_PARTIAL || bIsDucking) {
                        blendAmount2 += lastAssoc->m_fBlendAmount;
                    }
                }
            }
        }

        lastAssoc = RpAnimBlendGetNextAssociation(lastAssoc);
    } while (lastAssoc);


    if (!lastAssocWithFlag100 || blendAmount1 <= 0.5f || blendAmount2 >= 1.f) { // 0x5E58FB
        DoProcessLanding();
        return;
    }

    auto* lastAssocHier = lastAssocWithFlag100->m_pHierarchy;

    float minAnimTime = lastAssocHier->m_fTotalTime / 15.f;
    float maxAnimTime = lastAssocHier->m_fTotalTime / 2.f + minAnimTime; // Weird.. Why adding `minAnimTime` to it?

    if (bIsDucking) {
        minAnimTime += 0.2f;
        maxAnimTime += 0.2f;
    }

    if (m_pStats == &CPedStats::ms_apPedStats[STAT_BURGULAR_STATUS]) { // 0X5E5968

        // NOTE: The number `15` seems to be reoccuring, it's used above as well.
        const float animTimeMult = lastAssocWithFlag100->m_nAnimId != AnimationId::ANIM_ID_WALK ? 8.f / 15.f : 5.f / 15.f;

        float adhisionMult{ 1.f };
        switch (g_surfaceInfos->GetAdhesionGroup(m_nContactSurface)) {
        case eAdhesionGroup::ADHESION_GROUP_SAND: { // 0X5E599F
            if (rand() % 64) {
                m_vecAnimMovingShiftLocal *= 0.2f;
            }

            DoProcessLanding();
            return;
        }
        case eAdhesionGroup::ADHESION_GROUP_WET: { // 0X5E59B1
            m_vecAnimMovingShiftLocal *= 0.3f;
            DoProcessLanding();
            return;
        }
        case eAdhesionGroup::ADHESION_GROUP_LOOSE: { // 0x5E5A25
            if (rand() % 128) {
                m_vecAnimMovingShiftLocal *= 0.5f;
            }
            adhisionMult = 0.5f;
            break;
        }
        }

        if (m_pedAudio.field_7C) { // Move condition out here, but originally it was at 0x5E5AFA and 0x5E5A68
            const auto DoAddSkateAE = [&, this](eAudioEvents ae) {
                // 0x5E5AB4
                m_pedAudio.AddAudioEvent(
                    ae,
                    CAEAudioUtility::AudioLog10(adhisionMult) * 20.f,
                    lastAssocWithFlag100->m_nAnimId == AnimationId::ANIM_ID_WALK ? 1.f : 0.75f
                );
            };

            if (   lastAssocWithFlag100->m_fCurrentTime <= 0.f
                || lastAssocWithFlag100->m_fCurrentTime - lastAssocWithFlag100->m_fTimeStep > 0.f
            ) {
                if (   adhisionMult > 0.2f
                    && lastAssocWithFlag100->m_fCurrentTime > animTimeMult
                    && lastAssocWithFlag100->m_fCurrentTime - lastAssocWithFlag100->m_fTimeStep <= animTimeMult
                ) {
                    // 0x5E5B46
                    DoAddSkateAE(eAudioEvents::AE_PED_SKATE_RIGHT);
                }
            } else {
                // 0x5E5AB4
                DoAddSkateAE(eAudioEvents::AE_PED_SKATE_LEFT);
            }
        }

        DoProcessLanding();
        return;
    }

    // 0x5E5E56 and 0x5E5D57.. Seems like inlined?
    const auto DoFootStepAE = [&, this](bool isLeftFoot) {
        if (m_pedAudio.field_7C) {
            const auto DoAddFootStepAE = [&, this](float volume, float speed) {
                m_pedAudio.AddAudioEvent(isLeftFoot ? eAudioEvents::AE_PED_FOOTSTEP_RIGHT : eAudioEvents::AE_PED_FOOTSTEP_LEFT, volume, speed);
            };

            if (bIsDucking) {
                DoAddFootStepAE(-18.f, 0.8f);
            } else {
                const auto DoAddMovingFootStepAE = [&, this](float volume, float speed) {
                    if (m_nAnimGroup == ANIM_GROUP_PLAYERSNEAK) {
                        DoAddFootStepAE(volume - 6.f, speed - 0.1f);
                    } else {
                        DoAddFootStepAE(volume, speed);
                    }
                };

                switch (m_nMoveState) {
                case eMoveState::PEDMOVE_RUN:
                    DoAddMovingFootStepAE(-6.f, 1.1f);
                    break;
                case eMoveState::PEDMOVE_SPRINT:
                    DoAddMovingFootStepAE(0.f, 1.2f);
                    break;
                default:
                    DoAddMovingFootStepAE(-12.f, 0.9f);
                    break;
                }
            }
        }
        DoFootLanded(isLeftFoot, IsWalkRunSprintAnim());
    };

    // 0x5E5B50
    if (   minAnimTime > lastAssocWithFlag100->m_fCurrentTime
        || lastAssocWithFlag100->m_fCurrentTime - lastAssocWithFlag100->m_fTimeStep >= minAnimTime
    ) {
        // 0x5E5D8E
        if (lastAssocWithFlag100->m_fCurrentTime >= (double)maxAnimTime
            && lastAssocWithFlag100->m_fCurrentTime - lastAssocWithFlag100->m_fTimeStep < maxAnimTime)
        {
            // 0x5E592B - 0x5E5E56
            DoFootStepAE(false); // Do right foot step AE
            DoProcessLanding();
            return;
        }
    }

    if (IsPlayer()) { // 0x5E5B79
        if (const auto pd = m_pPlayerData) {
            const auto DoEventSoundQuiet = [this](float volume) {
                // 0x5E5BCB
                CEventSoundQuiet event{ this, volume, (uint32)-1, {0.f, 0.f, 0.f}};
                GetEventGlobalGroup()->Add(&event);
            };

            const auto isWearingBalaclava = pd->m_pPedClothesDesc->GetIsWearingBalaclava();
            switch (m_nMoveState) {
            case eMoveState::PEDMOVE_JOG:   // 0x5E5BAD -- hehehe, 0x 5E5 - BAD
            case eMoveState::PEDMOVE_RUN: { // 0x5E5BB6
                // 0x5E5BDD
                if (pd->m_fMoveBlendRatio >= 2.f) {
                    DoEventSoundQuiet(isWearingBalaclava ? 55.f : 45.f);
                } else {
                    const auto DoEventSoundQuiet_MoveBlendFactor = [&](float moveBlendFactor) {
                        // 0x5E5C31
                        if (const auto volume = (pd->m_fMoveBlendRatio - 1.f) * moveBlendFactor + 30.f; volume > 0.f) {
                            DoEventSoundQuiet(volume);
                        }
                    };

                    // 0x05E5BF3
                    if (isWearingBalaclava && pd->m_fMoveBlendRatio > 1.1f) {
                        DoEventSoundQuiet_MoveBlendFactor(20.f);
                    } else if (pd->m_fMoveBlendRatio > 1.5f) {
                        DoEventSoundQuiet_MoveBlendFactor(15.f);
                    }
                }
                break;
            }
            case eMoveState::PEDMOVE_SPRINT: { // 0x5E5BBB 
                DoEventSoundQuiet(isWearingBalaclava ? 65.f : 55.f);
                break;
            }
            }
            if (pd->m_pPedClothesDesc->GetIsWearingBalaclava()) {
            }
        }
    }

    // 0x5E5C8D
    DoFootStepAE(true); // Do left foot step AE
    DoFootLanded(true, IsWalkRunSprintAnim());
    DoProcessLanding();
}

/*!
* @addr 0x5E5ED0
* @brief Create model for current active weapon. Also creates FX for molotov if `this->IsPlayer()`.
* @param modelIndex Model that should be created for the current weapon.
*/
void CPed::AddWeaponModel(int32 modelIndex) {
   if (modelIndex == -1) {
       return;
   }

   // Make sure this weapon is supposed to have a model
   // (May be set to false even if it does, eg.: in case of infrared or night googles, see `TakeOffGoggles`)
   auto& activeWep = GetActiveWeapon();
   if (activeWep.m_bNoModel) {
       return;
   }

   // Remove old model (if any)
   if (m_pWeaponObject) {
       RemoveWeaponModel(-1);
   }

   // Create clump for model
   auto& wepMI = *CModelInfo::GetModelInfo(modelIndex);
   m_pWeaponObject = (RpClump*)wepMI.CreateInstance();
   m_pGunflashObject = m_pWeaponObject ? CClumpModelInfo::GetFrameFromName(m_pWeaponObject, "gunflash") : nullptr;
   wepMI.AddRef();

   m_nWeaponModelId = modelIndex;

   // If player and model is molotov create FX for it.
   if (IsPlayer()) {
       if (activeWep.m_nType == eWeaponType::WEAPON_MOLOTOV
           && modelIndex == eModelID::MODEL_MOLOTOV
           && !activeWep.m_pFxSystem
        ) {
           CVector pos{ 0.f, 0.f, 0.f };
           activeWep.m_pFxSystem = g_fxMan.CreateFxSystem("molotov_flame", &pos, &GetBoneMatrix(ePedBones::BONE_R_HAND), false);
           if (const auto fx = activeWep.m_pFxSystem) {
               fx->SetLocalParticles(true);
               fx->CopyParentMatrix();
               fx->Play();
           }
       }
   }
}

/*!
* @addr 0x5E6010
* @brief Takoe off goggles (Infrared/Nightvision weapon)
*/
void CPed::TakeOffGoggles()
{
    auto& wepInSlot = GetWeaponInSlot(GetWeaponSlot(eWeaponType::WEAPON_INFRARED));

    // Game checks if wepInSlot.m_nType != UNARMED here, not sure why? Probably compiler mistake on switch case codegen..

    switch (wepInSlot.m_nType) {
    case eWeaponType::WEAPON_INFRARED:
    case eWeaponType::WEAPON_NIGHTVISION: {
        // Remove googles model
        RemoveGogglesModel();

        wepInSlot.m_bNoModel = false;

        // Since we've took off the goggles we might have to load it's weapon model
        if (&wepInSlot == &GetActiveWeapon()) {
            AddWeaponModel(wepInSlot.GetWeaponInfo().m_nModelId1);
        }

        break;
    }
    }
}

/*!
* @addr 0x5E6080
* @brief Give ped weapon \a weaponType with ammo \a ammo. If ped has already the same weapon, just add the ammo to the weapon's current total ammo, and reaload it.
*/
void CPed::GiveWeapon(eWeaponType weaponType, uint32 ammo, bool likeUnused) {
    const auto givenWepInfo = CWeaponInfo::GetWeaponInfo(weaponType);
    auto& wepInSlot = GetWeaponInSlot(givenWepInfo->m_nSlot);
    const auto wepSlot = (eWeaponSlot)givenWepInfo->m_nSlot;

    if (wepInSlot.m_nType != weaponType) { // Another weapon in the slot, remove it, and set this weapon

        // Remove previous weapon (and possibly add any ammo it had to `ammo`)
        if (wepInSlot.m_nType != eWeaponType::WEAPON_UNARMED) {
            switch (wepSlot) {
            case eWeaponSlot::SHOTGUN:
            case eWeaponSlot::SMG:
            case eWeaponSlot::RIFLE: {
                ammo += wepInSlot.m_nTotalAmmo;
                break;
            }
            }

            RemoveWeaponModel(wepInSlot.GetWeaponInfo().m_nModelId1);

            if (givenWepInfo->m_nSlot == CWeaponInfo::GetWeaponInfo(eWeaponType::WEAPON_INFRARED)->m_nSlot) {
                RemoveGogglesModel();
            }

            wepInSlot.Shutdown();
        }

        // Give new
        wepInSlot.Initialise(weaponType, ammo, this);

        // Now `wepInSlot` is the weapon we've given to the player

        if (givenWepInfo->m_nSlot == m_nActiveWeaponSlot && !bInVehicle) {
            AddWeaponModel(givenWepInfo->m_nModelId1);
        }
    } else { // Same weapon already in the slot, update its ammo count and `Reload()` it
        if (wepSlot == eWeaponSlot::GIFT) { // Gifts have no ammo :D
            return;
        }

        wepInSlot.m_nTotalAmmo = std::min(99'999u, wepInSlot.m_nTotalAmmo + ammo);
        wepInSlot.Reload(this);

        // TODO: Inlined
        if (wepInSlot.m_nState == eWeaponState::WEAPONSTATE_OUT_OF_AMMO) {
            if (wepInSlot.m_nTotalAmmo > 0) {
                wepInSlot.m_nState = eWeaponState::WEAPONSTATE_READY;
            }
        }
    }

    if (wepInSlot.m_nState != eWeaponState::WEAPONSTATE_OUT_OF_AMMO) {
        wepInSlot.m_nState = eWeaponState::WEAPONSTATE_READY;
    }
}

/*!
* @notsa
*/
void CPed::GiveWeaponSet1() {
    GiveWeapon(WEAPON_BRASSKNUCKLE, 1, true);
    GiveWeapon(WEAPON_BASEBALLBAT, 1, true);
    GiveWeapon(WEAPON_MOLOTOV, 10, true);
    GiveWeapon(WEAPON_PISTOL, 100, true);
    GiveWeapon(WEAPON_SHOTGUN, 50, true);
    GiveWeapon(WEAPON_MICRO_UZI, 150, true);
    GiveWeapon(WEAPON_AK47, 120, true);
    GiveWeapon(WEAPON_COUNTRYRIFLE, 25, true);
    GiveWeapon(WEAPON_RLAUNCHER, 200, true);
    GiveWeapon(WEAPON_SPRAYCAN, 200, true);
}

/*!
* @notsa
*/
void CPed::GiveWeaponSet2() {
    GiveWeapon(WEAPON_KNIFE, 0, true);
    GiveWeapon(WEAPON_GRENADE, 10, true);
    GiveWeapon(WEAPON_DESERT_EAGLE, 40, true);
    GiveWeapon(WEAPON_SAWNOFF_SHOTGUN, 40, true);
    GiveWeapon(WEAPON_TEC9, 150, true);
    GiveWeapon(WEAPON_M4, 150, true);
    GiveWeapon(WEAPON_SNIPERRIFLE, 21, true);
    GiveWeapon(WEAPON_FLAMETHROWER, 500, true);
    GiveWeapon(WEAPON_EXTINGUISHER, 200, true);
}

/*!
* @notsa
*/
void CPed::GiveWeaponSet3() {
    GiveWeapon(WEAPON_CHAINSAW, 0, true);
    GiveWeapon(WEAPON_REMOTE_SATCHEL_CHARGE, 5, true);
    GiveWeapon(WEAPON_PISTOL_SILENCED, 40, true);
    GiveWeapon(WEAPON_SPAS12_SHOTGUN, 30, true);
    GiveWeapon(WEAPON_MP5, 100, true);
    GiveWeapon(WEAPON_M4, 150, true);
    GiveWeapon(WEAPON_RLAUNCHER_HS, 200, true);
}

/*!
* @addr 0x5E61F0
* @brief Set current weapon to be the one in \a slot
*/
void CPed::SetCurrentWeapon(int32 slot) {
    if (slot == -1) {
        return;
    }

    // Remove current weapon's model (if any)
    if (const auto currWepType = GetActiveWeapon().m_nType; currWepType != eWeaponType::WEAPON_UNARMED) {
        RemoveWeaponModel(CWeaponInfo::GetWeaponInfo(currWepType)->m_nModelId1);
    }

    // Set as active slot
    m_nActiveWeaponSlot = slot;

    // Set chosen weapon in player data
    if (const auto playerData = AsPlayer()->m_pPlayerData) {
        playerData->m_nChosenWeapon = slot;
    }

    // Load weapon in this slot (if any)
    if (const auto wepInSlotType = m_aWeapons[slot].m_nType; wepInSlotType != eWeaponType::WEAPON_UNARMED) {
        AddWeaponModel(CWeaponInfo::GetWeaponInfo(wepInSlotType)->m_nModelId1);
    }
}

/*!
* @addr 0x5E6280
* @brief Set current weapon as \a weaponType
*/
void CPed::SetCurrentWeapon(eWeaponType weaponType) {
    SetCurrentWeapon(GetWeaponSlot(weaponType));
}

/*!
* @addr  0x5E62B0
* @brief If ped's current weapon is \a weaponType clear it, and set current weapon as UNARMED
*/
void CPed::ClearWeapon(eWeaponType weaponType)
{
    auto wepSlot = CWeaponInfo::GetWeaponInfo(weaponType)->m_nSlot;
    if (wepSlot == -1) {
        return; // Weapon has no slot. (How could this happen?)
    }

    auto& wep = m_aWeapons[wepSlot];
    if (wep.m_nType != weaponType) {
        return; // Slot doesn't contain the given weapon - Might happen as some weapons share slots.
    }

    if (m_nActiveWeaponSlot == wepSlot) {
        SetCurrentWeapon(eWeaponType::WEAPON_UNARMED);
    }

    wep.Shutdown();

    switch (weaponType) {
    case eWeaponType::WEAPON_NIGHTVISION:
    case eWeaponType::WEAPON_INFRARED:
        RemoveGogglesModel();
        break;
    }
}

/*!
* @addr 0x5E6320
* @brief Clears every weapon from the ped.
*/
void CPed::ClearWeapons()
{
    CPed::RemoveWeaponModel(-1);
    CPed::RemoveGogglesModel();
    for (auto& m_aWeapon : m_aWeapons) {
        m_aWeapon.Shutdown();
    }
    CWeaponInfo* getWeaponInfo = CWeaponInfo::GetWeaponInfo(WEAPON_UNARMED, eWeaponSkill::STD);
    SetCurrentWeapon(getWeaponInfo->m_nSlot);
}

/*!
* @addr 0x5E6370
*/
void CPed::RemoveWeaponWhenEnteringVehicle(int32 arg0) {
    ((void(__thiscall *)(CPed*, int32))0x5E6370)(this, arg0);

    // Missing some code below (Had too many jumps and I was lazy to deal with it all)
    /*if (m_pPlayerData) {
        m_pPlayerData->m_bInVehicleDontAllowWeaponChange = true;
    }

    if (m_nSavedWeapon != eWeaponType::WEAPON_UNIDENTIFIED) {
        return;
    }

    const auto RemoveActiveWepModel = [this] {
        RemoveWeaponModel(GetActiveWeapon().GetWeaponInfo().m_nModelId1);
    };

    if (!IsPlayer() || !AsPlayer()->GetPlayerInfoForThisPlayerPed()->m_bCanDoDriveBy) {
        RemoveActiveWepModel();
        return;
    }


    const auto SaveCurrentSetActiveWeapon = [](eWeaponType wt) {

    };

    if (arg0) {
        const auto IsWeaponInSlotWithAmmo = [this](eWeaponSlot slot, eWeaponType wt) {
            const auto& wepInSlot = GetWeaponInSlot(slot);
            return wepInSlot.m_nType == wt && wepInSlot.m_nTotalAmmo > 0;
        };

        if (   IsWeaponInSlotWithAmmo(eWeaponSlot::SHOTGUN, eWeaponType::WEAPON_SAWNOFF_SHOTGUN)
            || IsWeaponInSlotWithAmmo(eWeaponSlot::HANDGUN, eWeaponType::WEAPON_PISTOL)
        ) {
            SaveCurrentSetActiveWeapon(eWeaponType::WEAPON_GOLFCLUB);
        }
    }*/
}

/*!
* @addr  0x5E6490
* @brief If ped has no saved weapon and is not a player load current weapon's model, otherwise set saved weapon as current.
*/
void CPed::ReplaceWeaponWhenExitingVehicle() {
    if (m_pPlayerData) {
        m_pPlayerData->m_bInVehicleDontAllowWeaponChange = false;
    }

    if (!IsPlayer() || m_nSavedWeapon == eWeaponType::WEAPON_UNIDENTIFIED) { // Not player, or has no saved weapon
        AddWeaponModel(GetActiveWeapon().GetWeaponInfo().m_nModelId1); // Load current active weapon
    } else { // Is player and has saved weapon, set it - TODO: Invert `if` logic to be more clear.. Eg.: if (IsPlayer() && m_nSavedWeapon != eWeaponType::WEAPON_UNIDENTIFIED)
        SetCurrentWeapon((eWeaponType)m_nSavedWeapon);
        m_nSavedWeapon = eWeaponType::WEAPON_UNIDENTIFIED;
    }
}

/*!
* @addr 0x5E6530
* @brief Save ped's current weapon and set current to UNARMED.
*/
void CPed::ReplaceWeaponForScriptedCutscene()
{
    m_nSavedWeapon = m_aWeapons[m_nActiveWeaponSlot].m_nType;
    SetCurrentWeapon(0);
}

/*!
* @addr 0x5E6550
* @brief Set ped's current weapon to saved weapon
*/
void CPed::RemoveWeaponForScriptedCutscene()
{
    if (m_nSavedWeapon != WEAPON_UNIDENTIFIED)
    {
        CWeaponInfo* weaponInfo = CWeaponInfo::GetWeaponInfo(m_nSavedWeapon, eWeaponSkill::STD);
        CPed::SetCurrentWeapon(weaponInfo->m_nSlot);
        m_nSavedWeapon = WEAPON_UNIDENTIFIED;
    }
}

/*!
* @addr 0x5E65A0
*/
void CPed::PreRenderAfterTest()
{
    ((void(__thiscall *)(CPed*))0x5E65A0)(this);
}

/*!
* @addr 0x5E7980
* @brief Set ped's state to IDLE
*/
void CPed::SetIdle() {
    switch (m_nPedState) {
    case ePedState::PEDSTATE_IDLE:
    case ePedState::PEDSTATE_MUG:
    case ePedState::PEDSTATE_FLEE_ENTITY:
        break;

    case ePedState::PEDSTATE_AIMGUN:
        m_nPedState = ePedState::PEDSTATE_IDLE;
        [[fallthrough]];
    default:
        m_nMoveState = eMoveState::PEDMOVE_STILL;
        break;
    }
}

/*!
* @addr 0x5E79B0
* @brief Set ped to look in direction \a heading
*/
void CPed::SetLook(float heading) {
    if (!IsPedInControl()) {
        return;
    }

    m_nPedState = ePedState::PEDSTATE_LOOK_HEADING;

    if (m_nLookTime >= CTimer::GetTimeInMS()) {
        return;
    }

    bIsLooking = true;
    m_fLookDirection = heading;
    m_nLookTime = 0;

    ClearReference(m_pLookTarget);

    if (CanUseTorsoWhenLooking()) {
        m_pedIK.bTorsoUsed = false;
    }
}

/*!
* @addr 0x5E7A60
* @brief Set ped to look at \a entity
*/
void CPed::SetLook(CEntity* entity) {
    if (IsPedInControl()) {
        m_nPedState = ePedState::PEDSTATE_LOOK_ENTITY;
        SetLookFlag(entity, false, false);
    }
}

/*!
* @addr 0x5E7B20
* @copydoc TurnBody
*/
void CPed::Look() {
    TurnBody();
}

/*!
* @addr 0x5E7CB0
* @brief Attach a weapon to us or something, not sure.
* @returns \a entity
*/
CEntity* CPed::AttachPedToEntity(CEntity* entity, CVector offset, uint16 turretAngleA, float turretAngleB, eWeaponType weaponType) {
    if (!entity || bInVehicle) {
        return nullptr;
    }

    // BUG/NOTE: ClearReference not called here?
    m_pAttachedTo = entity->AsPhysical();
    m_pAttachedTo->RegisterReference(reinterpret_cast<CEntity**>(&m_pAttachedTo));

    m_vecTurretOffset = offset;
    m_fTurretAngleB = turretAngleB;
    m_fTurretAngleA = turretAngleA;

    // Deal collision with `entity`
    if (!IsPlayer()) {
        if (entity->IsVehicle()) {
            m_pEntityIgnoredCollision = entity->AsPhysical();
        }
    } else { // For player just disable collision
        m_bUsesCollision = false;
    }

    if (m_nSavedWeapon == eWeaponType::WEAPON_UNIDENTIFIED) {
        m_nSavedWeapon = GetActiveWeapon().m_nType;
        m_nTurretAmmo = GetActiveWeapon().m_nTotalAmmo;
    }

    if (!IsPlayer()) {
        GiveWeapon(weaponType, 30'000, true);
        SetCurrentWeapon(weaponType);
        PositionAttachedPed();
    } else {
        if (weaponType != eWeaponType::WEAPON_UNARMED) {
            GiveWeapon(weaponType, 30'000, true);
        }

        m_pPlayerData->m_nChosenWeapon = weaponType;
        
        if (weaponType == eWeaponType::WEAPON_CAMERA) {
            TheCamera.SetNewPlayerWeaponMode(eCamMode::MODE_CAMERA);
        } else {
            // With the pool cue we can aim as well, so it needs a different cam mode.
            if (   entity->m_nModelIndex == eModelID::MODEL_POOLCUE
                && !CWeaponInfo::GetWeaponInfo(weaponType)->flags.b1stPerson
            ) {
                TheCamera.SetNewPlayerWeaponMode(eCamMode::MODE_AIMWEAPON_ATTACHED);
                m_pPlayerData->m_bFreeAiming = true;
            } else {
                TheCamera.SetNewPlayerWeaponMode(eCamMode::MODE_HELICANNON_1STPERSON);
            }
        }
        m_nPedState = ePedState::PEDSTATE_SNIPER_MODE;
        PositionAttachedPed();
    }

    return entity;
}

/*!
* @addr 0x5E7E60
*/
CEntity* CPed::AttachPedToBike(CEntity* entity, CVector offset, uint16 turretAngleA, float turretAngleB, float turretPosnMode, eWeaponType weaponType) {
    if (AttachPedToEntity(entity, offset, turretAngleA, turretAngleB, weaponType)) {
        m_nTurretPosnMode = turretPosnMode;
    }
}

/*!
* @addr 0x5E7EC0
*/
void CPed::DettachPedFromEntity(){
    const auto wasAttachedTo = m_pAttachedTo;

    // BUG/NOTE: Again, ClearOldReference not called.
    m_pAttachedTo = nullptr;

    switch (m_nPedState) {
    case ePedState::PEDSTATE_DIE: {
        m_pEntityIgnoredCollision = wasAttachedTo;
        ApplyMoveForce(wasAttachedTo->GetMatrix().GetForward() * -4.f);
        bIsStanding = false;
        break;
    }
    case ePedState::PEDSTATE_DEAD: // Skip this
        break;

    default: {
        CAnimManager::BlendAnimation(m_pRwClump, m_nAnimGroup, AnimationId::ANIM_ID_IDLE, 1000.f);
        bIsStanding = true;

        // Restore old weapon if any
        if (m_nSavedWeapon != eWeaponType::WEAPON_UNIDENTIFIED) {
            GetActiveWeapon().m_nAmmoInClip = 0;
            GetActiveWeapon().m_nTotalAmmo = 0;

            SetCurrentWeapon(m_nSavedWeapon);
            GetActiveWeapon().m_nTotalAmmo = (uint32)m_nTurretAmmo;

            m_nSavedWeapon = eWeaponType::WEAPON_UNIDENTIFIED;
        }

        if (IsPlayer()) {
            AsPlayer()->ClearWeaponTarget();
        }

        break;
    }
    }

}

/*!
* @addr 0x5E8830
*/
void CPed::SetAimFlag(float heading) {
    bIsAimingGun = true;
    bIsRestoringGun = false;
    ClearReference(m_pLookTarget);
    m_nLookTime = 0;

    if (bIsDucking) {
        m_pedIK.bUseArm = false;
    }

    m_pedIK.bUseArm = GetActiveWeapon().GetWeaponInfo(this).flags.bAimWithArm;
}

/*!
* @addr 0x5E88E0
* @todo
*/
bool CPed::CanWeRunAndFireWithWeapon()
{
    return ((bool(__thiscall *)(CPed*))0x5E88E0)(this);
}

/*!
* @addr 0x5E8910
* @todo
*/
void CPed::RequestDelayedWeapon()
{
    ((void(__thiscall *)(CPed*))0x5E8910)(this);
}

/*!
* @addr 0x5E89B0
* @brief Set delayed weapon (If ped doesn't already have one). Also drops entity in hand unless ped is a player.
*/
void CPed::GiveDelayedWeapon(eWeaponType weaponType, uint32 ammo) {
    // If not a player drop entity in ped's hand (if any)
    if (!IsPlayer()) {
        if (const auto task = (CTaskSimpleHoldEntity*)GetIntelligence()->GetTaskHold(false)) {
            if (task->m_pEntityToHold) {
                if (task->m_nBoneFrameId == ePedNode::PED_NODE_RIGHT_HAND) {
                    DropEntityThatThisPedIsHolding(true);
                }
            }
        }
    }

    // Set delayed weapon (If ped doesn't already have one)
    if (m_nDelayedWeapon == eWeaponType::WEAPON_UNIDENTIFIED) {
        m_nDelayedWeaponAmmo = ammo;
        m_nDelayedWeapon = weaponType;
        RequestDelayedWeapon();
    }
}

/*!
* @addr 0x5E8A30
*/
bool IsPedPointerValid(CPed* ped)
{
    return ((bool(__cdecl *)(CPed*))0x5E8A30)(ped);
}

/*!
* @addr 0x5E8AB0
*/
void CPed::GiveWeaponAtStartOfFight()
{
    if (m_nCreatedBy != PED_MISSION && GetActiveWeapon().m_nType == eWeaponType::WEAPON_UNARMED)
    {
        const auto GiveRandomWeaponByType = [this](eWeaponType type, uint16 maxRandom)
        {
            if ((m_nRandomSeed % 1024) >= maxRandom)
                return;

            if (m_nDelayedWeapon != eWeaponType::WEAPON_UNIDENTIFIED)
                return;

            GiveDelayedWeapon(type, 50);
            SetCurrentWeapon(GetWeaponSlot(type));
        };

        switch (m_nPedType)
        {
            case PED_TYPE_GANG1:
            case PED_TYPE_GANG2:
            case PED_TYPE_GANG3:
            case PED_TYPE_GANG4:
            case PED_TYPE_GANG5:
            case PED_TYPE_GANG6:
            case PED_TYPE_GANG7:
            case PED_TYPE_GANG8:
            case PED_TYPE_GANG9:
            case PED_TYPE_GANG10:
                GiveRandomWeaponByType(eWeaponType::WEAPON_PISTOL, 400);
                break;
            case PED_TYPE_DEALER:
            case PED_TYPE_CRIMINAL:
            case PED_TYPE_PROSTITUTE:
                GiveRandomWeaponByType(eWeaponType::WEAPON_KNIFE, 200);
                GiveRandomWeaponByType(eWeaponType::WEAPON_PISTOL, 400);
                break;
            default:
                break;
        }
    }
}

/*!
* @addr 5E8BE0
* @brief If ped has no weapons give them one. (AK-47 if `CHEAT_NO_ONE_CAN_STOP_US` is active, RLauncher if `CHEAT_ROCKET_MAYHEM` is active, or a pistol otherwise)
*/
void CPed::GiveWeaponWhenJoiningGang()
{
    if (m_aWeapons[m_nActiveWeaponSlot].m_nType == WEAPON_UNARMED && m_nDelayedWeapon == WEAPON_UNIDENTIFIED) {
        if (CCheat::IsActive(CHEAT_NO_ONE_CAN_STOP_US)) {
            GiveDelayedWeapon(WEAPON_AK47, 200);
            SetCurrentWeapon(CWeaponInfo::GetWeaponInfo(WEAPON_AK47, eWeaponSkill::STD)->m_nSlot);
        }
        else {
            CWeaponInfo* pWeaponInfo = nullptr;
            if (CCheat::IsActive(CHEAT_ROCKET_MAYHEM)) {
                GiveDelayedWeapon(WEAPON_RLAUNCHER, 200);
                pWeaponInfo = CWeaponInfo::GetWeaponInfo(WEAPON_RLAUNCHER, eWeaponSkill::STD);
            }
            else {
                CPed::GiveDelayedWeapon(WEAPON_PISTOL, 200);
                pWeaponInfo = CWeaponInfo::GetWeaponInfo(WEAPON_PISTOL, eWeaponSkill::STD);
            }
            CPed::SetCurrentWeapon(pWeaponInfo->m_nSlot);
        }
    }
}

/*!
* @addr 0x5EFF50
*/
bool CPed::GetPedTalking() {
    return m_pedSpeech.GetPedTalking();
}

/*!
* @addr 0x5EFF60
*/
void CPed::DisablePedSpeech(bool stopCurrentSpeech) {
    m_pedSpeech.DisablePedSpeech(stopCurrentSpeech);
}

/*!
* @addr 0x5EFF70
*/
void CPed::EnablePedSpeech() {
    m_pedSpeech.EnablePedSpeech();
}

/*!
* @addr 0x5EFF80
*/
void CPed::DisablePedSpeechForScriptSpeech(bool stopCurrentSpeech)
{
    m_pedSpeech.DisablePedSpeechForScriptSpeech(stopCurrentSpeech);
}

/*!
* @addr 0x5EFF90
*/
void CPed::EnablePedSpeechForScriptSpeech() {
    m_pedSpeech.EnablePedSpeechForScriptSpeech();
}

/*!
* @addr 0x5EFFA0
*/
bool CPed::CanPedHoldConversation() {
    return m_pedSpeech.CanPedHoldConversation();
}

/*!
* @addr 0x5EFFB0
*/
void CPed::SayScript(int32 arg0, uint8 arg1, uint8 arg2, uint8 arg3)
{
    m_pedSpeech.AddScriptSayEvent(eAudioEvents::AE_SCRIPT_SPEECH_PED, arg0, arg1, arg2, arg3);
}

/*!
* @addr 0x5EFFE0
*/
void CPed::Say(uint16 arg0, uint32 arg1, float arg2, uint8 arg3, uint8 arg4, uint8 arg5)
{
    if (arg0) {
        m_pedSpeech.AddSayEvent(eAudioEvents::AE_SPEECH_PED, arg0, arg1, arg2, arg3, arg4, arg5);
    }
}

/*!
* @addr 0x5F0060
*/
RwObject* SetPedAtomicVisibilityCB(RwObject* rwObject, void* data) {
    if (!data) {
        rwObjectSetFlags(rwObject, 0); // TODO: Figure out what the flag is
    }
    return rwObject;
}

/*!
* @addr 0x5F0140
* @brief Remove body part
* @todo See if it works with body parts other than the head
*/
void CPed::RemoveBodyPart(ePedNode pedNode, char localDir) {
    UNUSED(localDir);

    if (m_apBones[pedNode]->m_pIFrame) {
        if (CLocalisation::ShootLimbs()) {
            bRemoveHead = true;
            m_nBodypartToRemove = pedNode;
        }
    } else {
        printf("Trying to remove ped component");
    }
}

/*!
* @addr 0x5F0190
*/
void CPed::SpawnFlyingComponent(int32 arg0, char arg1)
{
    // NOP
}

/*!
* @addr 0x5F01A0
*/
/*!
* @addr 0x5F01A0
* @brief Check if line of sight bullet would hit the ped (Does a basic check of colpoint.point.z against head position)
* @returns 0, 1 - Yes , 2 - No. Always `1` if ped is falling.
*/
uint8 CPed::DoesLOSBulletHitPed(CColPoint& colPoint) {
    // TODO: Below is just a copy of the code in `IsPedHeadAbovePos` - A separate function should be made.
    RwV3d zero{}; // Placeholder - 0, 0, 0
    RwV3d headPos{};

    // TODO: Doesn't this just return the position of the matrix? Eg.: `BoneMatrix.pos` ?
    RwV3dTransformPoint(&headPos, &zero, &GetBoneMatrix((ePedBones)m_apBones[ePedNode::PED_NODE_HEAD]->m_nNodeId));

    if (m_nPedState == ePedState::PEDSTATE_FALL || colPoint.m_vecPoint.z < headPos.z) { // Ped falling, adjust 
        return 1;
    } else if (headPos.z + 0.2f <= colPoint.m_vecPoint.z) {
        return 0;
    } else {
        return 2;
    }
}

/*!
* @addr  0x5F0250
* @brief Remove all weapon animations by blending them into the IDLE animation.
* @param likeUnused Unused
*/
void CPed::RemoveWeaponAnims(int32 likeUnused, float blendDelta) {
    UNUSED(likeUnused);

    bool bFoundNotPartialAnim{};
    for (auto i = 0; i < 34; i++) { // TODO: Magic number `34`
        if (const auto assoc = RpAnimBlendClumpGetAssociation(m_pRwClump, AnimationId::ANIM_ID_FIRE)) {
            assoc->m_nFlags |= ANIM_FLAG_FREEZE_LAST_FRAME;
            if ((assoc->m_nFlags & ANIM_FLAG_PARTIAL)) {
                assoc->m_fBlendDelta = blendDelta;
            } else {
                bFoundNotPartialAnim = true;
            }
        }
    }

    if (bFoundNotPartialAnim) {
        CAnimManager::BlendAnimation(m_pRwClump, m_nAnimGroup, ANIM_ID_IDLE, -blendDelta);
    }
}


/*!
* @addr 0x5F02C0
* @returns If world space \a zPos is above ped's head
*/
bool CPed::IsPedHeadAbovePos(float zPos) {
    RwV3d zero{}; // Placeholder - 0, 0, 0
    RwV3d headPos{};

    // TODO: Doesn't this just return the position of the matrix? Eg.: `BoneMatrix.pos` ?
    RwV3dTransformPoint(&headPos, &zero, &GetBoneMatrix((ePedBones)m_apBones[ePedNode::PED_NODE_HEAD]->m_nNodeId));

    return zPos + GetPosition().z < headPos.z;
}

/*!
* @addr 0x5F0360
*/
void CPed::KillPedWithCar(CVehicle* car, float fDamageIntensity, bool bPlayDeadAnimation)
{
    ((void(__thiscall *)(CPed*, CVehicle*, float, bool))0x5F0360)(this, car, fDamageIntensity, bPlayDeadAnimation);
}

/*!
* @addr 0x6AE0D0
*/
void CPed::MakeTyresMuddySectorList(CPtrList& ptrList)
{
    ((void(__thiscall *)(CPed*, CPtrList&))0x6AE0D0)(this, ptrList);
}

/*!
* @addr  0x6B4200
* @brief Do sector list processing in a range of -/+2 (Calls \r MakeTyresMuddySectorList)
*/
void CPed::DeadPedMakesTyresBloody() {
    CWorld::IncrementCurrentScanCode();

    const auto& pos = GetPosition();

    const float minX = pos.x - 2.f;
    const float maxX = pos.x + 2.f;
    const float minY = pos.y - 2.f;
    const float maxY = pos.y + 2.f;

    const int32 startSectorX = std::max(CWorld::GetLodSectorX(minX), 0);
    const int32 startSectorY = std::max(CWorld::GetLodSectorY(minY), 0);
    const int32 endSectorX = std::min(CWorld::GetLodSectorX(maxX), MAX_LOD_PTR_LISTS_X - 1);
    const int32 endSectorY = std::min(CWorld::GetLodSectorY(maxY), MAX_LOD_PTR_LISTS_Y - 1);

    for (int32 sy = startSectorY; sy <= endSectorY; ++sy) {
        for (int32 sx = startSectorX; sx <= endSectorX; ++sx) {
            MakeTyresMuddySectorList(GetRepeatSector(sx, sy)->GetList(REPEATSECTOR_VEHICLES));
        }
    }
}

/*!
* @notsa
* @returns If player is in a vehicle and is it's driver
*/
bool CPed::IsInVehicleThatHasADriver()
{
    if (bInVehicle) {
        if (m_pVehicle && m_pVehicle->IsPassenger(this) && m_pVehicle->m_pDriver)
            return true;
    }
    return false;
}

/*!
* @notsa
* @returns If ped is follower of \a group
*/
bool CPed::IsFollowerOfGroup(const CPedGroup& group) {
    return group.GetMembership().IsFollower(this);
}

/*!
* @notsa
* @returns Bone transformation matrix in object space. To transform to world space ped's matrix must be used as well.
*/
RwMatrix& CPed::GetBoneMatrix(ePedBones bone) const { 
    const auto hierarchy = GetAnimHierarchyFromClump(m_pRwClump);
    return RpHAnimHierarchyGetMatrixArray(hierarchy)[RpHAnimIDGetIndex(hierarchy, (size_t)bone)];
}

/*!
* @addr 0x5E4880
* @brief Set model index (Also re-inits animblend, MoneyCount, and default decision-marker)
*/
void CPed::SetModelIndex(uint32 modelIndex) {
    m_bIsVisible = true;

    CEntity::SetModelIndex(modelIndex);

    RpAnimBlendClumpInit(m_pRwClump);
    RpAnimBlendClumpFillFrameArray(m_pRwClump, m_apBones);

    auto& mi = *(CPedModelInfo*)GetModelInfo();

    SetPedStats(mi.m_nStatType);
    RestoreHeadingRate();

    SetPedDefaultDecisionMaker();

    // Set random money count
    const auto GetRandomMoneyCount = [this] {
        if (CGeneral::GetRandomNumberInRange(0.f, 100.f) < 3.f) { // Moved up here
            return 400;
        } else if (CPopCycle::IsPedInGroupTheseGroups(m_nModelIndex, { POPCYCLE_GROUP_BUSINESS, POPCYCLE_GROUP_CASUAL_RICH })) {
            return rand() % 50 + 20;
        } else {
            return rand() % 25;
        }
    };
    m_nMoneyCount = GetRandomMoneyCount();

    m_nAnimGroup = mi.m_nAnimType;
    CAnimManager::AddAnimation(m_pRwClump, m_nAnimGroup, AnimationId::ANIM_ID_IDLE);

    if (CanUseTorsoWhenLooking()) {
        m_pedIK.bTorsoUsed = true;
    }

    // Deal with animation stuff once again
    RpClumpGetAnimBlendClumpData(m_pRwClump)->m_pvecPedPosition = (CVector*)&m_vecAnimMovingShiftLocal; // TODO: Is this correct?

    // Create hit col model
    if (!mi.m_pHitColModel) {
        mi.CreateHitColModelSkinned(m_pRwClump);
    }

    // And finally update our rph anim
    UpdateRpHAnim();
}

/*!
* @addr 0x5DEBF0
*/
void CPed::DeleteRwObject()
{
    CEntity::DeleteRwObject();
}

/*!
* @addr 0x5E8CD0
*/
void CPed::ProcessControl()
{
    plugin::CallMethod<0x5E8CD0, CPed*>(this);
}

/*!
* @addr 0x5E4110
* @brief Set player's position (Also resets move/turn speed, and cancels all tasks)
*/
void CPed::Teleport(CVector destination, bool resetRotation) {
    UNUSED(resetRotation);

    if (IsPlayer() || GetTaskManager().FindActiveTaskByType(eTaskType::TASK_COMPLEX_LEAVE_CAR)) {
        GetIntelligence()->FlushImmediately(true);
    }

    CWorld::Remove(this);
    SetPosn(destination);
    bIsStanding = false;
    ClearReference(m_pDamageEntity);
    CWorld::Add(this);

    m_vecMoveSpeed.Reset();
    m_vecTurnSpeed.Reset();
}

/*!
* @addr 0x5E3C30
*/
void CPed::SpecialEntityPreCollisionStuff(CPhysical* colPhysical,
                                          bool  bIgnoreStuckCheck,
                                          bool& bCollisionDisabled,
                                          bool& bCollidedEntityCollisionIgnored,
                                          bool& bCollidedEntityUnableToMove,
                                          bool& bThisOrCollidedEntityStuck)
{
    plugin::CallMethod<0x5E3C30, CPed*, CPhysical*, bool, bool&, bool&, bool&, bool&>(this, colPhysical, bIgnoreStuckCheck, bCollisionDisabled, bCollidedEntityCollisionIgnored, bCollidedEntityUnableToMove, bThisOrCollidedEntityStuck);
}

/*!
* @addr 0x5E3E90
*/
uint8 CPed::SpecialEntityCalcCollisionSteps(bool& bProcessCollisionBeforeSettingTimeStep, bool& unk2)
{
    return plugin::CallMethodAndReturn<uint8, 0x5E3E90, CPed*, bool&, bool&>(this, bProcessCollisionBeforeSettingTimeStep, unk2);
}

/*!
* @addr 0x5E8A20
*/
void CPed::PreRender()
{
    if (m_nPedState != PEDSTATE_DRIVING)
        PreRenderAfterTest();
}

/*!
* @addr 0x5E7680
*/
void CPed::Render() {
    if (!bDontRender && !(m_bIsVisible || CMirrors::ShouldRenderPeds())) {
        return;
    }

    // Save alpha fn. ref for player peds
    uint32 storedAlphaRef{};
    if (IsPlayer()) {
        RwRenderStateGet(rwRENDERSTATEALPHATESTFUNCTIONREF, RWRSTATE(&storedAlphaRef));
    }

    // Moved early out to top from here

    // Now do some extra checks if in vehicle (possibly early out)
    if (   bInVehicle
        && m_pVehicle
        && !GetTaskManager().FindActiveTaskByType(TASK_COMPLEX_LEAVE_CAR)
        && !GetTaskManager().FindActiveTaskByType(TASK_COMPLEX_CAR_SLOW_BE_DRAGGED_OUT_AND_STAND_UP)
    ) {
        if (!bRenderPedInCar) {
            return;
        }

        if (   !m_pVehicle->IsBike()
            && !m_pVehicle->IsSubQuad()
            && !IsPlayer()
        ) {
            // Okay, let's check if the ped is close enough to the camera

            const auto IsPedInRangeOfCamera = [this](auto range) {
                const auto distSq = (TheCamera.GetPosition() - GetPosition()).SquaredMagnitude();
                const auto finalRange = range * TheCamera.m_fLODDistMultiplier;
                return distSq < finalRange * finalRange;
            };
            if (!IsPedInRangeOfCamera(m_pVehicle->IsBoat() ? 40.f : 25.f)) { // Boats have bigger range
                return;
            }
        }
    }

    // Render us (And any extra FX)
    if (CPostEffects::IsVisionFXActive()) {
        CPostEffects::InfraredVisionStoreAndSetLightsForHeatObjects(this);
        CPostEffects::NightVisionSetLights();
        CEntity::Render();
        CPostEffects::InfraredVisionRestoreLightsForHeatObjects();
    } else {
        CEntity::Render();
    }

    // Render weapon (and gun flash) as well. (Done for local player only if flag is set.)
    if (!m_pPlayerData || m_pPlayerData->m_bRenderWeapon) { 
        if (m_pWeaponObject) {
            if (!bInVehicle || !GetIntelligence()->GetTaskSwim() || GetIntelligence()->GetTaskHold(false)) {
                weaponPedsForPc_Insert(this);
                if (m_nWeaponGunflashAlphaMP1 > 0 || m_nWeaponGunflashAlphaMP2 > 0) {
                    ResetGunFlashAlpha();
                }
            }
        }
    }

    // Render goggles object
    if (m_pGogglesObject) {
        auto& headMat = GetBoneMatrix(ePedBones::BONE_HEAD);

        // Update goggle's matrix with head's
        *RwFrameGetMatrix(m_pGogglesObject) = headMat; // TODO: Is there a better way to do this?
        
        // Calculate it's position
        RwV3d pos{ 0.f, 0.84f, 0.f }; // Offset
        RwV3dTransformPoints(&pos, &pos, 1, &headMat);

        RwV3dAssign(RwMatrixGetPos(RwFrameGetMatrix(m_pGogglesObject)), &pos); // IMPORTANT NOTE: I still don't agree with C fanboys, this is not readable.

        RwFrameUpdateObjects(m_pGogglesObject);
        RpClumpRender((RpClump*)m_pGogglesObject);
    }

    // Render JetPack (if any)
    if (const auto task = GetIntelligence()->GetTaskJetPack()) {
        task->Process();
    }

    bIsCached = true; // TODO/NOTE: Hmm.. Seems like the flag name is a little misleading.

    // Restore alpha test fn for player peds
    if (IsPlayer()) {
        RwRenderStateSet(rwRENDERSTATEALPHATESTFUNCTIONREF, RWRSTATE(storedAlphaRef));
    }
}

/*!
* @addr 0x553F00
*/
bool CPed::SetupLighting() {
  ActivateDirectional();
  return CRenderer::SetupLightingForEntity(this);
}

/*!
* @addr 0x5533B0
*/
void CPed::RemoveLighting(bool bRemove) {
    UNUSED(bRemove);

    if (!physicalFlags.bDestroyed) {
        CPointLights::RemoveLightsAffectingObject();
    }

    SetAmbientColours();
    DeActivateDirectional();
}

/*!
* @addr 0x5E7B70
*/
void CPed::FlagToDestroyWhenNextProcessed() {
    if (!bInVehicle) {
        return;
    }

    if (!m_pVehicle) {
        return;
    }

    if (m_pVehicle->IsDriver(this)) {
        ClearReference(m_pVehicle->m_pDriver);
        if (IsPlayer() || m_pVehicle->m_nStatus != STATUS_WRECKED) {
            m_pVehicle->m_nStatus = STATUS_ABANDONED;
        }
    } else {
        m_pVehicle->RemovePassenger(this);
    }
    bInVehicle = false;

    if (IsVehiclePointerValid(m_pVehicle)) {
        ClearReference(m_pVehicle);
    }
    m_pVehicle = nullptr;

    SetPedState(IsCreatedByMission() ? ePedState::PEDSTATE_DEAD : ePedState::PEDSTATE_NONE);
}

/*!
* @addr 0x5E2530
*/
int32 CPed::ProcessEntityCollision(CEntity* entity, CColPoint* colPoint)
{
    return plugin::CallMethodAndReturn<int32, 0x5E2530, CPed*, CEntity*, CColPoint*>(this, entity, colPoint);
}

// NOTSA
bool CPed::IsInVehicleAsPassenger() const noexcept {
    return bInVehicle && m_pVehicle && m_pVehicle->m_pDriver != this;
}
