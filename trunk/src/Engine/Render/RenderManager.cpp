#include "RenderManager.h"

void engine::render::CRenderManager::Init(HWND hWnd, int Width, int Height) {
	InitDevice_SwapChain_DeviceContext(hWnd, Width, Height);
	Get_RendertargetView();
	Create_DepthStencil(hWnd, Width, Height);
	Set_Viewport(Width, Height);
	SetRendertarget();

	CreateDebugShader();

	m_BackgroundColor = { 1, 0, 1, 1 };
}

bool engine::render::CRenderManager::InitDevice_SwapChain_DeviceContext(HWND hWnd, int Width, int Height) {

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = Width;
	sd.BufferDesc.Height = Height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;

	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels,
		D3D11_SDK_VERSION, &sd, &m_SwapChain, &m_Device, NULL, &m_DeviceContext))) {
		return false;
	}
}

bool engine::render::CRenderManager::Get_RendertargetView() {
	ID3D11Texture2D *pBackBuffer;
	if (FAILED(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)))
		return FALSE;
	HRESULT hr = m_Device->CreateRenderTargetView(pBackBuffer, NULL, &m_RenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return FALSE;
}

bool engine::render::CRenderManager::Create_DepthStencil(HWND hWnd, int Width, int Height) {

	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = Width;
	descDepth.Height = Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	HRESULT hr = m_Device->CreateTexture2D(&descDepth, NULL, &m_DepthStencil);
	
	if (FAILED(hr)) {
		return false;
	}
		
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = m_Device->CreateDepthStencilView(m_DepthStencil, &descDSV, &m_DepthStencilView);
	
	if (FAILED(hr)) {
		return false;
	}
}

void engine::render::CRenderManager::Set_Viewport(int Width, int Height) {
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)Width;
	vp.Height = (FLOAT)Height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_DeviceContext->RSSetViewports(1, &vp);
}

void engine::render::CRenderManager::SetRendertarget() {
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);
}

bool engine::render::CRenderManager::CreateDebugShader() {
	
// C++ macros are nuts
#define STRINGIFY(X) #X
#define TOSTRING(X) STRINGIFY(X)

const char debugRenderEffectCode[] =
		"#line " TOSTRING(__LINE__) "\n"
		"struct VS_OUTPUT\n"
		"{\n"
		"	float4 Pos : SV_POSITION;\n"
		"	float4 Color : COLOR0;\n"
		"};\n"
		"\n"
		"VS_OUTPUT VS(float4 Pos : POSITION, float4 Color : COLOR)\n"
		"{\n"
		"	VS_OUTPUT l_Output = (VS_OUTPUT)0;\n"
		"	l_Output.Pos = Pos;\n"
		"	l_Output.Color = Color;\n"
		"	return l_Output;\n"
		"}\n"
		"\n"
		"float4 PS(VS_OUTPUT IN) : SV_Target\n"		
		"{\n"
		"	//return float4(1,0,0,1);\n"
		"	//return m_BaseColor;\n"
		"	return IN.Color;\n"
		"}\n";

	ID3DBlob *vsBlob, *psBlob;
	ID3DBlob *errorBlob;

	HRESULT hr = D3DCompile(debugRenderEffectCode, sizeof(debugRenderEffectCode), __FILE__, nullptr, nullptr,
		"VS", "vs_4_0", 0, 0, &vsBlob, &errorBlob);

	if (FAILED(hr)) {
		if (errorBlob != NULL)
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		if (errorBlob)
			errorBlob->Release();
		return false;
	}

	hr = D3DCompile(debugRenderEffectCode, sizeof(debugRenderEffectCode), __FILE__, nullptr, nullptr,
		"PS", "ps_4_0", 0, 0, &psBlob, &errorBlob);

	if (FAILED(hr))	{
		if (errorBlob != NULL)
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		if (errorBlob)
			errorBlob->Release();
		return false;
	}

	hr = m_Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_DebugVertexShader);

	if (FAILED(hr))
		return false;

	hr = m_Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_DebugPixelShader);

	if (FAILED(hr))
		return false;

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = m_Device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_DebugVertexLayout);

	if (FAILED(hr))
		return false;

	D3D11_BUFFER_DESC l_BufferDescription;
	ZeroMemory(&l_BufferDescription, sizeof(l_BufferDescription));
	l_BufferDescription.Usage = D3D11_USAGE_DYNAMIC;
	l_BufferDescription.ByteWidth = sizeof(CDebugVertex)*DebugVertexBufferSize;
	l_BufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	l_BufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//D3D11_SUBRESOURCE_DATA InitData;
	//ZeroMemory(&InitData, sizeof(InitData));
	//InitData.pSysMem = Vtxs;
	hr = m_Device->CreateBuffer(&l_BufferDescription, nullptr, &m_DebugVertexBuffer);
	if (FAILED(hr)) {
		return false;
	}
}
		

// Rendering
void engine::render::CRenderManager::BeginRender() {
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, &m_BackgroundColor.x);
	m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
	SetSolidRenderState();
}

void engine::render::CRenderManager::SetSolidRenderState() {
	m_DeviceContext->RSSetState(m_SolidRenderState);
}

void engine::render::CRenderManager::DoRender() {
	Draw_Triangle();
}

void engine::render::CRenderManager::EndRender() {
	m_SwapChain->Present(1, 0);
}

void engine::render::CRenderManager::Draw_Triangle()
{
	CDebugVertex resultBuffer[] = {
		{ Vect4f(-0.5f, -0.5f, 0.0f, 1.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },
		{ Vect4f(+0.0f, +0.5f, 0.0f, 1.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },
		{ Vect4f(+0.5f, -0.5f, 0.0f, 1.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) }
	};

	// set vertex data
	D3D11_MAPPED_SUBRESOURCE resource;
	HRESULT hr = m_DeviceContext->Map(m_DebugVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);

	if (FAILED(hr))
		return; // TODO log

	#ifdef _DEBUG
		assert(3 * sizeof(CDebugVertex) < DebugVertexBufferSize * sizeof(CDebugVertex));
	#endif
	
	memcpy(resource.pData, resultBuffer, 3 * sizeof(CDebugVertex));

	m_DeviceContext->Unmap(m_DebugVertexBuffer, 0);

	UINT stride = sizeof(CDebugVertex);
	UINT offset = 0;
	m_DeviceContext->IASetVertexBuffers(0, 1, &m_DebugVertexBuffer, &stride, &offset);
	m_DeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_DeviceContext->IASetInputLayout(m_DebugVertexLayout);
	m_DeviceContext->VSSetShader(m_DebugVertexShader, NULL, 0);
	m_DeviceContext->PSSetShader(m_DebugPixelShader, NULL, 0);

	m_DeviceContext->Draw(3, 0);
}

void engine::render::CRenderManager::CreateDebugObjects()
{
	//AXIS
	static CDebugVertex l_AxisVtxs[6] =
	{
		{ Vect4f(0.0f, 0.0f, 0.0f, 1.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },
		{ Vect4f(1.0f, 0.0f, 0.0f, 1.0f), CColor(1.0f, 0.0f, 0.0f, 1.0f) },

		{ Vect4f(0.0f, 0.0f, 0.0f, 1.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },
		{ Vect4f(0.0f, 1.0f, 0.0f, 1.0f), CColor(0.0f, 1.0f, 0.0f, 1.0f) },

		{ Vect4f(0.0f, 0.0f, 0.0f, 1.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) },
		{ Vect4f(0.0f, 0.0f, 1.0f, 1.0f), CColor(0.0f, 0.0f, 1.0f, 1.0f) }
	};

	m_AxisRenderableVertexs = l_AxisVtxs;
	m_NumVerticesAxis = 6;


	//CUBE
	const float l_SizeCube = 1.0f;
	static CDebugVertex l_CubeVtxs[] =
	{
		{ Vect4f(-l_SizeCube / 2.0f, -l_SizeCube / 2.0f, -l_SizeCube / 2.0f, 1.0f), CColor(1.0f, 1.0f, 1.0f, 1.0f) },
		// TODO
	};

	m_CubeRenderableVertexs = l_CubeVtxs;
	m_NumVerticesCube = 8 * 3;

	//GRID
	float l_Size = 10.0f;
	const int l_Grid = 10;
	static CDebugVertex l_GridVtxs[(l_Grid + 1) * 2 * 2];
	for (int b = 0; b <= l_Grid; ++b)
	{
		l_GridVtxs[b * 2].Position = Vect4f(); // TODO
		l_GridVtxs[b * 2].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
		l_GridVtxs[(b * 2) + 1].Position = Vect4f(); // TODO
		l_GridVtxs[(b * 2) + 1].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	//LINEAS EN X
	for (int b = 0; b <= l_Grid; ++b)
	{
		l_GridVtxs[(l_Grid + 1) * 2 + (b * 2)].Position = Vect4f(); // TODO
		l_GridVtxs[(l_Grid + 1) * 2 + (b * 2)].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
		l_GridVtxs[(l_Grid + 1) * 2 + (b * 2) + 1].Position = Vect4f(); // TODO
		l_GridVtxs[(l_Grid + 1) * 2 + (b * 2) + 1].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
	}

	m_GridRenderableVertexs = l_GridVtxs;
	m_NumVerticesGrid = (l_Grid + 1) * 2 * 2;

	//SPHERE
	const int l_Aristas = 10;
	static CDebugVertex l_SphereVtxs[4 * l_Aristas*l_Aristas];
	for (int t = 0; t<l_Aristas; ++t)
	{
		float l_RadiusRing = sin(/*DEG2RAD*/(6.28318531f / 360.f) * (180.0f*((float)t)) / ((float)l_Aristas));
		for (int b = 0; b<l_Aristas; ++b)
		{
			l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 0].Position = Vect4f(l_RadiusRing*cos(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)b) / ((float)l_Aristas))), cos(/*DEG2RAD*/(6.28318531f / 360.f) *(180.0f*((float)t)) / ((float)l_Aristas)), l_RadiusRing*sin(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)b) / ((float)l_Aristas))), 1.0f);
			l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 0].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
			l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 1].Position = Vect4f(l_RadiusRing*cos(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)(b + 1)) / ((float)l_Aristas))), cos(/*DEG2RAD*/(6.28318531f / 360.f) *(180.0f*((float)t)) / ((float)l_Aristas)), l_RadiusRing*sin(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)(b + 1)) / ((float)l_Aristas))), 1.0f);
			l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 1].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);

			float l_RadiusNextRing = sin(/*DEG2RAD*/(6.28318531f / 360.f) *(180.0f*((float)(t + 1))) / ((float)l_Aristas));

			l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 2].Position = Vect4f(l_RadiusRing*cos(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)b) / ((float)l_Aristas))), cos(/*DEG2RAD*/(6.28318531f / 360.f) *(180.0f*((float)t)) / ((float)l_Aristas)), l_RadiusRing*sin(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)b) / ((float)l_Aristas))), 1.0f);
			l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 2].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
			l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 3].Position = Vect4f(l_RadiusNextRing*cos(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)b) / ((float)l_Aristas))), cos(/*DEG2RAD*/(6.28318531f / 360.f) *(180.0f*((float)(t + 1))) / ((float)l_Aristas)), l_RadiusNextRing*sin(/*DEG2RAD*/(6.28318531f / 360.f) *((float)(360.0f*(float)b) / ((float)l_Aristas))), 1.0f);
			l_SphereVtxs[(t*l_Aristas * 4) + (b * 4) + 3].Color = CColor(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	m_SphereRenderableVertexs = l_SphereVtxs;
	m_NumVerticesSphere = 4 * l_Aristas*l_Aristas;
}