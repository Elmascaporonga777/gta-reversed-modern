#include "StdInc.h"

#include "PedGroup.h"

// 0x5FC150
CPedGroup::CPedGroup() {
    m_groupMembership.m_pPedGroup = this;
    m_groupIntelligence.m_pPedGroup = this;
    m_bIsMissionGroup = false;
    m_pPed = nullptr;
    m_bMembersEnterLeadersVehicle = true;
}

// 0x5FC190
CPedGroup::~CPedGroup() {
    for (auto i = 0u; i < m_groupMembership.m_apMembers.size(); i++) {
        m_groupMembership.RemoveMember(i);
    }
}

//! @returns Distance of the furthers member from the leader
float CPedGroup::FindDistanceToFurthestMember() {
    return plugin::CallMethodAndReturn<float, 0x5FB010, CPedGroup*>(this);
    /*
    const auto leader = GetMembership().GetLeader();
    for (const auto& mem : GetMembership().GetMembers(true)) {

    }*/
}

float CPedGroup::FindNearestFollowerToLeader(CPed** ppOutNearestMember) {
    const auto [nearest, distSq] = GetMembership().FindClosestFollowerToLeader();
    if (nearest) {
        if (ppOutNearestMember) {
            *ppOutNearestMember = nearest;
        }
        return std::sqrt(distSq);
    }
    return 1.0e10;
}

void CPedGroup::Flush() {
    plugin::CallMethod<0x5FB790, CPedGroup*>(this);
}

CPed* CPedGroup::GetClosestGroupPed(CPed* ped, float* pOutDistance) {
    return plugin::CallMethodAndReturn<CPed*, 0x5FACD0, CPedGroup*, CPed*, float*>(this, ped, pOutDistance);
}

bool CPedGroup::IsAnyoneUsingCar(const CVehicle* vehicle) {
    return plugin::CallMethodAndReturn<bool, 0x5F7DB0, CPedGroup*, const CVehicle*>(this, vehicle);
}

void CPedGroup::PlayerGaveCommand_Attack(CPed* playerPed, CPed* ped) {
    plugin::CallMethod<0x5F7CC0, CPedGroup*, CPed*, CPed*>(this, playerPed, ped);
}

void CPedGroup::PlayerGaveCommand_Gather(CPed* ped) {
    plugin::CallMethod<0x5FAB60, CPedGroup*, CPed*>(this, ped);
}

void CPedGroup::Process() {
    plugin::CallMethod<0x5FC7E0, CPedGroup*>(this);
}

void CPedGroup::RemoveAllFollowers() {
    plugin::CallMethod<0x5FB7D0, CPedGroup*>(this);
}

void CPedGroup::Teleport(const CVector* pos) {
    plugin::CallMethod<0x5F7AD0, CPedGroup*, const CVector*>(this, pos);
}

int32 CPedGroup::GetId() const {
    return CPedGroups::GetGroupId(this);
}

void CPedGroup::InjectHooks() {
    RH_ScopedClass(CPedGroup);
    RH_ScopedCategory(); // TODO: Change this to the appropriate category!

    RH_ScopedInstall(Constructor, 0x5FC150);
    RH_ScopedInstall(Destructor, 0x5FC190);

    RH_ScopedInstall(Teleport, 0x5F7AD0, {.reversed = false});
    RH_ScopedInstall(PlayerGaveCommand_Attack, 0x5F7CC0, {.reversed = false});
    RH_ScopedInstall(IsAnyoneUsingCar, 0x5F7DB0, {.reversed = false});
    RH_ScopedInstall(GetClosestGroupPed, 0x5FACD0, {.reversed = false});
    RH_ScopedInstall(FindDistanceToFurthestMember, 0x5FB010, {.reversed = false});
    RH_ScopedInstall(FindNearestFollowerToLeader, 0x5FB0A0);
    RH_ScopedInstall(Flush, 0x5FB790, {.reversed = false});
    RH_ScopedInstall(Process, 0x5FC7E0, {.reversed = false});
}
