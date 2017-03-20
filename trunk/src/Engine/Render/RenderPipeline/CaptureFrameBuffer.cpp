#include "CaptureFrameBuffer.h"
#include "CaptureFrameBufferTexture.h"
#include "XML\XML.h"

CCaptureFrameBuffer::CCaptureFrameBuffer():
    m_CapturedFrameBufferTexture(nullptr)
{}

CCaptureFrameBuffer::~CCaptureFrameBuffer() {}

/*Leerá el nodo :
<capture_frame_buffer name="capture_frame_buffer_fog" texture_width_as_frame_buffer="true"/>
<capture_texture name = "DeferredCapturedTexture" texture_width_as_frame_buffer = "true" / >
< / capture_frame_buffer>*/


bool CCaptureFrameBuffer::Load(const CXMLElement* aElement)
{
    bool lOk = CRenderStagedTexture::Load(aElement);
    if (lOk)
    {
        if (aElement->GetAttribute<bool>("texture_width_as_frame_buffer", true))
        {
            m_CapturedFrameBufferTexture->SetName(aElement->GetAttribute<std::string>("name", ""));
        }
    }
    return lOk;
}

void CCaptureFrameBuffer::Execute(CRenderManager& lRM)
{
    m_CapturedFrameBufferTexture->Capture(0);
}
