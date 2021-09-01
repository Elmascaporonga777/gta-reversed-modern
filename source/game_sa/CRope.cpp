#include "StdInc.h"
#include "CRope.h"

void CRope::InjectHooks() {
    using namespace ReversibleHooks;
    Install("CRope", "ReleasePickedUpObject", 0x556030, &CRope::ReleasePickedUpObject);
    // Install("CRope", "CreateHookObjectForRope", 0x556070, &CRope::CreateHookObjectForRope);
    // Install("CRope", "UpdateWeightInRope", 0x5561B0, &CRope::UpdateWeightInRope);
    // Install("CRope", "Remove", 0x556780, &CRope::Remove);
    Install("CRope", "Render", 0x556800, &CRope::Render);
    // Install("CRope", "PickUpObject", 0x5569C0, &CRope::PickUpObject);
    // Install("CRope", "Update", 0x557530, &CRope::Update);
}

// 0x556030
void CRope::ReleasePickedUpObject() {
    if (m_pRopeAttachObject) {
        m_pRopeAttachObject->physicalFlags.bAttachedToEntity = false;
        m_pRopeAttachObject->physicalFlags.b32 = false;
        m_pRopeAttachObject = 0;
    }
    m_pAttachedEntity->m_bUsesCollision = true;
    m_nFlags1 = 60; // 6th, 7th bits set
}

// 0x556070
void CRope::CreateHookObjectForRope() {
    plugin::CallMethod<0x556070, CRope*>(this);
}

// 0x5561B0
int8_t CRope::UpdateWeightInRope(float a2, float a3, float a4, int32 a5, float* a6) {
    return plugin::CallMethodAndReturn<int8_t, 0x5561B0, CRope*, float, float, float, int32, float*>(this, a2, a3, a4, a5, a6);
}

// 0x556780
void CRope::Remove() {
    plugin::CallMethod<0x556780, CRope*>(this);
}

// 0x556800
void CRope::Render() {
    // Note: Probably needs adjustments if `NUM_ROPE_SEGMENTS` is changed
    if (!TheCamera.IsSphereVisible(m_segments[NUM_ROPE_SEGMENTS / 2], 20.0f))
        return;

    if ((TheCamera.GetPosition() - m_segments[0]).Magnitude2D() >= 120.0f)
        return;

    DefinedState();

    const RwRGBA color = { 128, 0, 0, 0 };

    const auto GetVertex = [](unsigned i) {
        return &aTempBufferVertices[i];
    };

    RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void*)TRUE);
    RwRenderStateSet(rwRENDERSTATESRCBLEND, (void*)rwBLENDSRCALPHA);
    RwRenderStateSet(rwRENDERSTATEDESTBLEND, (void*)rwBLENDINVSRCALPHA);
    RwRenderStateSet(rwRENDERSTATETEXTUREFILTER, (void*)rwFILTERLINEAR);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void*)FALSE);

    for (unsigned i = 0; i < NUM_ROPE_SEGMENTS; i++) {
        RxObjSpace3DVertexSetPreLitColor(GetVertex(i), &color);
        RxObjSpace3DVertexSetPos(GetVertex(i), &m_segments[i]);
    }


    if (RwIm3DTransform(aTempBufferVertices, NUM_ROPE_SEGMENTS, 0, 0))
    {
        RxVertexIndex indices[] = {
            0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6,
            6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
            12, 12, 13, 13, 14, 14, 15, 15, 16,
            16, 17, 17, 18, 18, 19, 19, 20, 20,
            21, 21, 22, 22, 23, 23, 24, 24, 25,
            25, 26, 26, 27, 27, 28, 28, 29, 29,
            30, 30, 31
        };
        RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, indices, std::size(indices));
        RwIm3DEnd();
    }

    if (m_type == 6) {
        const CVector pos[] = { m_segments[0], {709.32f, 916.20f, 53.0f} };
        for (unsigned i = 0; i < 2; i++) {
            RxObjSpace3DVertexSetPreLitColor(GetVertex(i), &color);
            RxObjSpace3DVertexSetPos(GetVertex(i), &pos[i]);
        }
        if (RwIm3DTransform(aTempBufferVertices, 2, 0, 0))
        {
            RxVertexIndex indices[] = { 0, 1 };
            RwIm3DRenderIndexedPrimitive(rwPRIMTYPELINELIST, indices, 2);
            RwIm3DEnd();
        }
    }
}

// 0x5569C0
void CRope::PickUpObject(CEntity* a2) {
    plugin::CallMethod<0x5569C0, CRope*, CEntity*>(this, a2);
}

// 0x557530
void CRope::Update() {
    plugin::CallMethod<0x557530, CRope*>(this);
}
