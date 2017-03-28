#include "CaptureFrameBuffer.h"
#include "CaptureFrameBufferTexture.h"
#include "XML\XML.h"
#include "Engine/Engine.h"
#include "Graphics/Textures/TextureManager.h"

CCaptureFrameBuffer::CCaptureFrameBuffer(): m_CapturedFrameBufferTexture(nullptr) {}
CCaptureFrameBuffer::~CCaptureFrameBuffer() {}

/*
Leerá el nodo :
<capture_frame_buffer name="capture_frame_buffer_fog" texture_width_as_frame_buffer="true"/>
<capture_texture name = "DeferredCapturedTexture" texture_width_as_frame_buffer = "true" / >
< / capture_frame_buffer>
*/

bool CCaptureFrameBuffer::Load(const CXMLElement* aElement)
{
    bool lOk = false;

    //if (strcmp(aElement->Name(), "capture_frame_buffer") == 0)
    //{
    m_CapturedFrameBufferTexture = new CCaptureFrameBufferTexture(aElement);
    lOk = CEngine::GetInstance().GetTextureManager().AddTexture(*m_CapturedFrameBufferTexture);
    //}

    return lOk;
}

void CCaptureFrameBuffer::Execute(CRenderManager& lRM)
{
    m_CapturedFrameBufferTexture->Capture(0);
}
