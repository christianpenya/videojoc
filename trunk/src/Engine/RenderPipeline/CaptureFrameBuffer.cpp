#include "CaptureFrameBuffer.h"


CCaptureFrameBuffer::CCaptureFrameBuffer()
{

}

CCaptureFrameBuffer::~CCaptureFrameBuffer()
{

}

/*Leerá el nodo :
<capture_frame_buffer>
<capture_texture name = "DeferredCapturedTexture" texture_width_as_frame_buffer = "true" / >
< / capture_frame_buffer>*/


bool CCaptureFrameBuffer::Load(const CXMLElement* aElement)
{
    bool lOk = false;

    /*    CCaptureFrameBufferTexture * lCaptureFrameBufferTexture = new CCaptureFrameBufferTexture(aElement);
        if (lCaptureFrameBufferTexture->Load())
        {
            lOk = m_CapturedFrameBufferTexture.Add(aElement, lCaptureFrameBufferTexture);
        }
        else
        {
            base::utils::CheckedDelete(lCaptureFrameBufferTexture);
        }
    	*/
    return lOk;
}

void CCaptureFrameBuffer::Execute(CRenderManager& lRM)
{
    m_CapturedFrameBufferTexture->Capture(0);
}